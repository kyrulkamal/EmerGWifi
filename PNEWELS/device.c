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

/*- Fuses configuration ----------------------------------------------------
	This FUSES configuration is only valid when flashing the MCU with .ELF file. Flashing with those file will automatically change the FUSES altogether.
*/
FUSES =
{
	.low = 0xf7,
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

/// <summary>
/// Configure main system timer and start it
/// </summary>
static void appStartUpdateTimer(void)
{
  appUpdateTimer.interval = APP_UPDATE_INTERVAL;
  appUpdateTimer.mode = SYS_TIMER_PERIODIC_MODE;
  appUpdateTimer.handler = appUpdateTimerHandler;
  SYS_TimerStart(&appUpdateTimer);
}

/// <summary>
/// When the main timer is ticked, this function will be run.
/// </summary>
/// <param name="SYS_Timer_t">System timer parameter</param>
static void appUpdateTimerHandler(SYS_Timer_t *timer)
{
  if(wps_send_flag == 1)
  {
	status_report_flag = 1;
	data_update_flag = 1;
  }
  (void)timer;
}

/// <summary>
/// Initializing the application layer.
/// </summary>
static void appInit(void)
{
  factory_initialization();
  APP_IbLoadSettings();
  APP_NwkInit();  
  appStartUpdateTimer();
  
  PNEWELSGpioInit();
  PNEWELSAppInit();
  systemFlag_check(device_flag); //check whether this is the first time install. If yes, System directly enter WPS mode
  
}

/// <summary>
/// Initialize watchdog timer.
/// </summary>
void WDT_Init(void)
{	
	cli();
	wdt_reset();
	wdt_enable(WDTO_8S);
	//WDTCSR = (1<<WDCE)|(1<<WDE);
	sei();
}

/// <summary>
/// Main routine
/// </summary>
int main(void)
{
  SYS_Init();
  appInit();
  WDT_Init(); //Enable Watchdog timer
  
  //pne_debug_Init(); //disable this to disable the device displaying the signal strength over serial comm. This function can cause instability. DO NOT USE for production. Debugging only
  ANT_DIV = 0x06; //6 for ceramic, 5 for antenna

  while (1)
  {
    SYS_TaskHandler();	//system service. DO NOT MODIFY or REMOVE
	PNEWELSTaskHandler();	//main program reside here
	//pne_debug_display();
	//wdt_reset();	//reset watchdog timer. If the timer not reset after 8 seconds the system will reset.
	
  }
}