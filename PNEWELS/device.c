/**
 * \file device.c
 *
 * \brief Device implementation
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
 * $Id: device.c 8035 2013-05-17 22:04:06Z ataradov $
 *
 */

/*- Includes ---------------------------------------------------------------*/
#include "config.h"
#include "hal.h"
#include "phy.h"
#include "sys.h"
#include "nwk.h"
#include "sysTimer.h"
#include "appIb.h"
#include "appNwk.h"
//#include "appGpio.h"
//#include "appCommand.h"
#include "PNEWELS.h"
#include "PNEWELSDebug.h"
#include "PNEWELSFactoryInit.h"
#include <avr/wdt.h>

/*- Definitions ------------------------------------------------------------*/

/*- Types ------------------------------------------------------------------*/

/*- Prototypes -------------------------------------------------------------*/
static void appUpdateTimerHandler(SYS_Timer_t *timer);
void WDT_Init(void);


/*- Variables --------------------------------------------------------------*/
static SYS_Timer_t appUpdateTimer;
/*- Implementations --------------------------------------------------------*/

/*************************************************************************//**
*****************************************************************************/
static void APP_TaskHandler(void)
{
}

/*************************************************************************//**
*****************************************************************************/
static void appStartUpdateTimer(void)
{
  appUpdateTimer.interval = APP_UPDATE_INTERVAL;
  appUpdateTimer.mode = SYS_TIMER_PERIODIC_MODE;
  appUpdateTimer.handler = appUpdateTimerHandler;
  SYS_TimerStart(&appUpdateTimer); //disabled temporary
}

/*************************************************************************//**
*****************************************************************************/
static void appUpdateTimerHandler(SYS_Timer_t *timer)
{
  //APP_CommandReport();
  if(wps_send_flag == 1)
  {
	status_report_flag = 1;
	data_update_flag = 1;
  }
  (void)timer;
}

/*************************************************************************//**
*****************************************************************************/
static void appInit(void)
{
  factory_initialization();
  APP_IbLoadSettings();
  APP_NwkInit();  
  //APP_GpioInit();
  //APP_CommandInit();
  appStartUpdateTimer();
  
  PNEWELSGpioInit();
  PNEWELSAppInit();
  systemFlag_check(device_flag); //check whether this is the first time install. If yes, System directly enter WPS mode
  
}

/*************************************************************************//**
*****************************************************************************/
void WDT_Init(void)
{	
	cli();
	wdt_reset();
	wdt_enable(WDTO_8S);
	//WDTCSR = (1<<WDCE)|(1<<WDE);
	sei();
}

/*************************************************************************//**
*****************************************************************************/
int main(void)
{
  SYS_Init();
  appInit();
  WDT_Init(); //Enable Watchdog timer
  
//   pne_debug_Init(); //disable this to disable the device displaying the signal strength
  ANT_DIV = 0x06; //6 for ceramic, 5 for antenna

  while (1)
  {
    SYS_TaskHandler();
    APP_TaskHandler();
    //APP_GpioTaskHandler();
	PNEWELSTaskHandler();
// 	pne_debug_display();
	wdt_reset();
	
  }
}