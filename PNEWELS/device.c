/**
 * \file device.c
 * Modified by :	Zulhilmi
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
#include <avr/io.h>

/*- Fuses configuration ----------------------------------------------------*/
FUSES =
{
	.low = 0xc2,
	.high = 0x95,
	.extended = 0xf6,
};

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
  
  //pne_debug_Init(); //disable this to disable the device displaying the signal strength
  ANT_DIV = 0x05; //6 for ceramic, 5 for antenna

  while (1)
  {
    SYS_TaskHandler();	//system service. DO NOT MODIFY or REMOVE
    APP_TaskHandler();	//application service. strongly advised NOT TO MODIFY or REMOVE
    //APP_GpioTaskHandler();
	PNEWELSTaskHandler();	//main program reside here
	//pne_debug_display();
	wdt_reset();	//reset watchdog timer. If the timer not reset after 8 seconds the system will reset.
	
  }
}