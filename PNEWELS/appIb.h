/**
 * \file appIb.h
 *
 * \brief Information Base interface
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
 * $Id: appIb.h 8086 2013-05-29 22:56:48Z ataradov $
 *
 */

#ifndef _APP_IB_H_
#define _APP_IB_H_

/*- Definitions ------------------------------------------------------------*/
#define APP_NAME_SIZE           32
#define APP_EEPROM_MAGIC        0x78656c41

/*- Types ------------------------------------------------------------------*/
typedef struct AppIb_t
{
  uint16_t     addr;
  uint16_t     channel;
  uint16_t     panId;
  uint8_t      name[APP_NAME_SIZE];
  uint32_t     magic;
} AppIb_t;

/*- Prototypes -------------------------------------------------------------*/
void APP_IbSaveSettings(void);
void APP_IbLoadSettings(void);
void APP_IbLoadSettings_WPS(void);

/*- Variables --------------------------------------------------------------*/
extern AppIb_t appIb;

#endif // _APP_IB_H_
