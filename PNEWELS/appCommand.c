/**
 * \file appCommand.c
 *
 * \brief Commands routines implementation
 *
 * Copyright (C) 2012-2013, Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 * $Id: appCommand.c 8119 2013-06-05 21:05:45Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "hal.h"
#include "sys.h"
#include "appIb.h"
#include "appNwk.h"
#include "appGpio.h"
#include "appCommand.h"

/*- Definitions ------------------------------------------------------------*/

/*- Types ------------------------------------------------------------------*/
typedef enum
{
  APP_COMMAND_SCAN         = 0x00,
  APP_COMMAND_REPORT       = 0x01,
  APP_COMMAND_NAME_SET     = 0x02,
  APP_COMMAND_LED_SET      = 0x03,
  APP_COMMAND_LED_GET      = 0x04,
  APP_COMMAND_LED_STATE    = 0x05,
  APP_COMMAND_BUTTON_GET   = 0x06,
  APP_COMMAND_BUTTON_STATE = 0x07,
  APP_COMMAND_UART_DATA    = 0x08,
} AppCommand_t;

/*- Prototypes -------------------------------------------------------------*/
static void appCommandSendStateCommand(uint8_t id, uint8_t state);

/*- Variables --------------------------------------------------------------*/

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
void APP_CommandInit(void)
{
}

/*************************************************************************//**
*****************************************************************************/
bool APP_CommandReceived(uint8_t *data, uint8_t size)
{
  uint8_t id = data[0];
  uint8_t value, len;

  if (APP_COMMAND_SCAN == id)
  {
    APP_CommandReport();
  }

  else if (APP_COMMAND_NAME_SET == id)
  {
    if (data[1] > APP_NAME_SIZE)
      len = APP_NAME_SIZE;
    else
      len = data[1];

    memset(appIb.name, ' ', sizeof(appIb.name));
    memcpy(appIb.name, &data[2], len);
    APP_IbSaveSettings();
    APP_CommandReport();
  }

  else if (APP_COMMAND_LED_SET == id)
  {
    APP_GpioLedSet(data[1]);
    value = APP_GpioLedGet();
    appCommandSendStateCommand(APP_COMMAND_LED_STATE, value);
  }

  else if (APP_COMMAND_LED_GET == id)
  {
    value = APP_GpioLedGet();
    appCommandSendStateCommand(APP_COMMAND_LED_STATE, value);
  }

  else if (APP_COMMAND_BUTTON_GET == id)
  {
    value = APP_GpioButtonGet();
    appCommandSendStateCommand(APP_COMMAND_BUTTON_STATE, value);
  }

  else if (APP_COMMAND_UART_DATA == id)
  {
    //APP_UartSend(&data[2], data[1]);
  }

  else
    return false;

  (void)size;

  return true;
}

/*************************************************************************//**
*****************************************************************************/
void APP_CommandGpioChange(uint8_t state)
{
  appCommandSendStateCommand(APP_COMMAND_BUTTON_STATE, state);
}

/*************************************************************************//**
*****************************************************************************/
void APP_CommandReport(void)
{
  AppNwkBuffer_t *buf;

  if (NULL == (buf = APP_NwkGetBuffer()))
    return;

  buf->size = 0;
  buf->data[buf->size++] = APP_COMMAND_REPORT;

  memcpy(&buf->data[buf->size], appIb.name, APP_NAME_SIZE);
  buf->size += APP_NAME_SIZE;

  buf->data[buf->size++] = APP_GpioButtonGet();
  buf->data[buf->size++] = APP_GpioLedGet();

  APP_NwkSendBuffer(buf);
}

/*************************************************************************//**
*****************************************************************************/
static void appCommandSendStateCommand(uint8_t id, uint8_t state)
{
  AppNwkBuffer_t *buf;

  if (NULL == (buf = APP_NwkGetBuffer()))
    return;

  buf->size = 0;
  buf->data[buf->size++] = id;
  buf->data[buf->size++] = state;

  APP_NwkSendBuffer(buf);
}
