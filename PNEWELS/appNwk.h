/**
 * \file appNwk.h
 *
 * \brief Network routines interface
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
 * $Id: appNwk.h 8035 2013-05-17 22:04:06Z ataradov $
 *
 */

#ifndef _APP_NWK_H_
#define _APP_NWK_H_

#include "nwk.h"

/*- Definitions ------------------------------------------------------------*/
#define APP_NWK_BUFFER_SIZE    NWK_MAX_PAYLOAD_SIZE

/*- Types ------------------------------------------------------------------*/
typedef struct
{
  bool           busy;
  uint8_t        retries;
  NWK_DataReq_t  req;
  uint8_t        data[APP_NWK_BUFFER_SIZE];
  uint8_t        size;
} AppNwkBuffer_t;

/*- Prototypes -------------------------------------------------------------*/
void APP_NwkInit(void);
AppNwkBuffer_t *APP_NwkGetBuffer(void);
void APP_NwkSendBuffer(AppNwkBuffer_t *buf);

#endif // _APP_NWK_H_
