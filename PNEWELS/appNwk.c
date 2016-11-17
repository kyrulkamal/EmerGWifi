/**
 * \file appNwk.c
 *
 * \brief Network routines implementation
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
 * $Id: appNwk.c 8119 2013-06-05 21:05:45Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include <stdlib.h>
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "appIb.h"
#include "appNwk.h"
//#include "appCommand.h"
#include "PNEWELS.h"

/*- Definitions ------------------------------------------------------------*/

/*- Types ------------------------------------------------------------------*/

/*- Prototypes -------------------------------------------------------------*/

/*- Variables --------------------------------------------------------------*/
static AppNwkBuffer_t appNwkBuffer[APP_NWK_BUFFERS_SIZE];

/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
static bool appNwkDataInd(NWK_DataInd_t *ind)
{
  //return APP_CommandReceived(ind->data, ind->size);
  return pneIncomingData(ind->data, ind->size);
}

/*************************************************************************//**
*****************************************************************************/
static void appNwkDataConf(NWK_DataReq_t *req)
{
  uint8_t idx;

  for (idx = 0; idx < APP_NWK_BUFFERS_SIZE; idx++)
    if (req == &appNwkBuffer[idx].req)
      break;

  if (NWK_SUCCESS_STATUS == req->status)
  {
    appNwkBuffer[idx].busy = false;
  }
  else
  {
    if (--appNwkBuffer[idx].retries)
      NWK_DataReq(req);
    else
      appNwkBuffer[idx].busy = false;
  }
}

/*************************************************************************//**
*****************************************************************************/
void APP_NwkInit(void)
{
  NWK_SetAddr(appIb.addr);
  NWK_SetPanId(appIb.panId);
  PHY_SetChannel(appIb.channel);
  PHY_SetRxState(true);

  #ifdef NWK_ENABLE_SECURITY
  NWK_SetSecurityKey((uint8_t *)"Security12345678");	//passphrase for AES encryption
  #endif

  NWK_OpenEndpoint(APP_ENDPOINT, appNwkDataInd);

  for (uint8_t i = 0; i < APP_NWK_BUFFERS_SIZE; i++)
  {
    appNwkBuffer[i].busy = false;
    appNwkBuffer[i].req.dstAddr = 0;
    appNwkBuffer[i].req.dstEndpoint = APP_ENDPOINT;
    appNwkBuffer[i].req.srcEndpoint = APP_ENDPOINT;
    appNwkBuffer[i].req.options = NWK_OPT_ACK_REQUEST | NWK_OPT_ENABLE_SECURITY; //enable security mode AES
    appNwkBuffer[i].req.data = appNwkBuffer[i].data;
    appNwkBuffer[i].req.confirm = appNwkDataConf;
  }

  //APP_CommandReport();
  pneOutgoingData();
}

/*************************************************************************//**
*****************************************************************************/
AppNwkBuffer_t *APP_NwkGetBuffer(void)
{
  for (uint8_t i = 0; i < APP_NWK_BUFFERS_SIZE; i++)
  {
    if (false == appNwkBuffer[i].busy)
    {
      appNwkBuffer[i].busy = true;
      appNwkBuffer[i].size = 0;
      appNwkBuffer[i].retries = APP_NWK_RETRIES;
      return &appNwkBuffer[i];
    }
  }

  return NULL;
}

/*************************************************************************//**
*****************************************************************************/
void APP_NwkSendBuffer(AppNwkBuffer_t *buf)
{
  if (buf->size)
  {
    buf->req.size = buf->size;
    NWK_DataReq(&buf->req);
  }
  else
  {
    buf->busy = false;
  }
}

void APP_NwkInit_Broadcast(void)
{
	NWK_SetAddr(appIb.addr);
	NWK_SetPanId(appIb.panId);
	PHY_SetChannel(appIb.channel);
	PHY_SetRxState(true);

	#ifdef NWK_ENABLE_SECURITY
	NWK_SetSecurityKey((uint8_t *)APP_SECURITY_KEY);	//passphrase for AES encryption
	#endif

	NWK_OpenEndpoint(APP_ENDPOINT, appNwkDataInd);

	for (uint8_t i = 0; i < APP_NWK_BUFFERS_SIZE; i++)
	{
		appNwkBuffer[i].busy = false;
		appNwkBuffer[i].req.dstAddr = 0;
		appNwkBuffer[i].req.dstEndpoint = APP_ENDPOINT;
		appNwkBuffer[i].req.srcEndpoint = APP_ENDPOINT;
		appNwkBuffer[i].req.options = NWK_OPT_ACK_REQUEST | NWK_OPT_ENABLE_SECURITY | (1 << 2); //enable security mode AES
		appNwkBuffer[i].req.data = appNwkBuffer[i].data;
		appNwkBuffer[i].req.confirm = appNwkDataConf;
	}
}