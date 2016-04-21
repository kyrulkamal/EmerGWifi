/**
 * \file appIb.c
 *
 * \brief Information Base implementation
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
 * $Id: appIb.c 8086 2013-05-29 22:56:48Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "sysTypes.h"
#include "config.h"
#include "appEeprom.h"
#include "appIb.h"
#include "PNEWELSRFInit.h"
#include "PNEWELS.h"
#include "memoryConfig.h"
#include "PNE_Factory.h"
#include "appNwk.h"

/*- Definitions ------------------------------------------------------------*/
#define APP_IB_EEPROM_OFFSET   0

/*- Variables --------------------------------------------------------------*/
AppIb_t appIb;

/*- Implementations --------------------------------------------------------*/

/*****************************************************************************
*****************************************************************************/
void APP_IbSaveSettings(void)
{
  appIb.magic = APP_EEPROM_MAGIC;
  APP_EepromWrite(APP_IB_EEPROM_OFFSET, (uint8_t *)&appIb, sizeof(appIb));
}

/*****************************************************************************
*****************************************************************************/
void APP_IbLoadSettings(void)
{
  APP_EepromRead(APP_IB_EEPROM_OFFSET, (uint8_t *)&appIb, sizeof(appIb));

  if (APP_EEPROM_MAGIC != appIb.magic)
  {
    char hex[] = "0123456789abcdef";

/*-------------Initialize RF parameter----------------*/
    appIb.addr = rf_init(DEVICE_ADDRESS_L,PNEWELS_Buffer.deviceAddress);
    appIb.panId = rf_init(DEVICE_PAN_L,PNEWELS_Buffer.devicePAN);
    appIb.channel = rf_init(DEVICE_CHANNEL_L,PNEWELS_Buffer.deviceChannel);
//------------------------------------------------------
/*-------------Initialize Device parameter----------------*/
	/*PNEWELS_Buffer.groupID = readE2prom(GROUP_ID);*/

    memset(appIb.name, ' ', sizeof(appIb.name));
    memcpy(appIb.name, "Device_xxxx", strlen("Device_xxxx"));
    appIb.name[7] = hex[(appIb.addr >> 12) & 0x0f];
    appIb.name[8] = hex[(appIb.addr >> 8) & 0x0f];
    appIb.name[9] = hex[(appIb.addr >> 4) & 0x0f];
    appIb.name[10] = hex[appIb.addr & 0x0f];
  }
}

void APP_IbLoadSettings_WPS(void)
{
	APP_EepromRead(APP_IB_EEPROM_OFFSET, (uint8_t *)&appIb, sizeof(appIb));

	if (APP_EEPROM_MAGIC != appIb.magic)
	{
		char hex[] = "0123456789abcdef";

		/*-------------Initialize RF parameter----------------*/
		appIb.addr = WPS_ADDRESS;
		appIb.panId = WPS_PANID;
		appIb.channel = WPS_CHANNEL;
		//------------------------------------------------------

		memset(appIb.name, ' ', sizeof(appIb.name));
		memcpy(appIb.name, "Device_xxxx", strlen("Device_xxxx"));
		appIb.name[7] = hex[(appIb.addr >> 12) & 0x0f];
		appIb.name[8] = hex[(appIb.addr >> 8) & 0x0f];
		appIb.name[9] = hex[(appIb.addr >> 4) & 0x0f];
		appIb.name[10] = hex[appIb.addr & 0x0f];
	}
	APP_NwkInit(); //execute new setting
}
