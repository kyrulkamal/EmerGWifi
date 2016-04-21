/**
 * \file appEeprom.c
 *
 * \brief EEPROM access functions
 *
 * Copyright (C) 2012 Atmel Corporation. All rights reserved.
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
 * $Id: appEeprom.c 8088 2013-05-29 22:58:14Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include "config.h"
#include "hal.h"
#include "sys.h"
#include "appEeprom.h"

/*- Implementations --------------------------------------------------------*/

/*****************************************************************************
*****************************************************************************/
void APP_EepromWriteByte(uint16_t address, uint8_t data)
{
  while (EECR & (1 << EEPE));

  EEAR = address;
  EEDR = data;
  ATOMIC_SECTION_ENTER
    EECR |= (1 << EEMPE);
    EECR |= (1 << EEPE);
  ATOMIC_SECTION_LEAVE
}

/*****************************************************************************
*****************************************************************************/
uint8_t APP_EepromReadByte(uint16_t address)
{
  while (EECR & (1 << EEPE));

  EEAR = address;
  EECR |= (1 << EERE);
  return EEDR;
}

/*****************************************************************************
*****************************************************************************/
void APP_EepromWrite(uint16_t address, uint8_t *data, uint16_t size)
{
  for (uint16_t i = 0; i < size; i++)
  {
    if (APP_EepromReadByte(address + i) != data[i])
      APP_EepromWriteByte(address + i, data[i]);
  }
}

/*****************************************************************************
*****************************************************************************/
void APP_EepromRead(uint16_t address, uint8_t *data, uint16_t size)
{
  for (uint16_t i = 0; i < size; i++)
  {
    data[i] = APP_EepromReadByte(address + i);
  }
}
