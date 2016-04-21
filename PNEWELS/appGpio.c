/**
 * \file appGpio.c
 *
 * \brief GPIO routines implementation
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
 * $Id: appGpio.c 8086 2013-05-29 22:56:48Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include "config.h"
#include "hal.h"
#include "halGpio.h"
#include "sys.h"
#include "appCommand.h"

/*- Definitions ------------------------------------------------------------*/
HAL_GPIO_PIN(LED,    B, 4);
HAL_GPIO_PIN(BUTTON, E, 4);

/*- Types ------------------------------------------------------------------*/
typedef enum
{
  APP_GPIO_INITIAL_STATE,
  APP_GPIO_NORMAL_OPERATION_STATE,
} AppGpioState_t;

/*- Prototypes -------------------------------------------------------------*/

/*- Variables --------------------------------------------------------------*/
static AppGpioState_t appGpioState = APP_GPIO_INITIAL_STATE;
static uint8_t appGpioLedState = 0;
static uint8_t appGpioButtonState = 0;

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
static void appGpioScan()
{
  uint8_t state;

  state = HAL_GPIO_BUTTON_read() ? 0 : 1;

  if (appGpioButtonState != state)
  {
    appGpioButtonState = state;
    APP_CommandGpioChange(appGpioButtonState);
  }
}

/*************************************************************************//**
*****************************************************************************/
void APP_GpioInit(void)
{
  HAL_GPIO_LED_out();
  HAL_GPIO_LED_set();
  appGpioLedState = 0;

  HAL_GPIO_BUTTON_in();
  HAL_GPIO_BUTTON_pullup();
  appGpioButtonState = HAL_GPIO_BUTTON_read() ? 0 : 1;

  appGpioState = APP_GPIO_NORMAL_OPERATION_STATE;
}

/*************************************************************************//**
*****************************************************************************/
uint8_t APP_GpioLedGet(void)
{
  return appGpioLedState;
}

/*************************************************************************//**
*****************************************************************************/
void APP_GpioLedSet(uint8_t value)
{
  appGpioLedState = value;

  if (value)
    HAL_GPIO_LED_clr();
  else
    HAL_GPIO_LED_set();
}

/*************************************************************************//**
*****************************************************************************/
uint8_t APP_GpioButtonGet(void)
{
  return appGpioButtonState;
}

/*************************************************************************//**
*****************************************************************************/
void APP_GpioTaskHandler(void)
{
  switch (appGpioState)
  {
    case APP_GPIO_INITIAL_STATE:
      break;

    case APP_GPIO_NORMAL_OPERATION_STATE:
    {
      appGpioScan();
    } break;
  }
}
