///*
 //* pne_appGpio.c
 //*
 //* Created: 2/9/2015 2:38:38 PM
 //*  Author: tay
 //*/ 
///*
 //* appPNEWELS.c
 //*
 //* Created: 12/28/2014 7:39:56 PM
 //*  Author: chtay
 //*/ 
//
///* #include "file" first in the directory containing the current file, then in the directories as specified by -iquote options
   //#include <file> will search in the system */
//
////0 equal to no press; 1 equal to press
//
//#include "pne_appGpio.h"
//#include "sysTypes.h"
////#include "halGpio.h"
//#include "sysTimer.h"
////#include "appPNEWELS_TWI.h"
////#include "appPNEWELS_USART.h"
//#include "gpio.h"
//
///*- Definitions ------------------------------------------------------------*/
////HAL_GPIO_PIN(LED,    B, 4);
////HAL_GPIO_PIN(BUTTON, E, 4);
//
///*- Variables --------------------------------------------------------------*/
//static SYS_Timer_t appLEDTimer;
//static SYS_Timer_t appLEDIntervalTimer;
////static SYS_Timer_t appStatusUpdateTimer;
//
//uint8_t LED_flag = 0;
//uint8_t set_LED_flag = 0;
////uint8_t update_flag = 1;
////uint8_t data_received[2] = {0, 0};
//uint8_t number_press = 0;
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////static void appStatusUpdateTimerHandler(SYS_Timer_t *timer)
////{
	////update_flag = 1;
	////(void)timer;
////}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static void appLEDIntervalTimerHandler(SYS_Timer_t *timer)
//{
	//SYS_TimerStart(&appLEDTimer);
	//(void)timer;
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static void appLEDTimerHandler(SYS_Timer_t *timer)
//{
	//if (LED_flag != (set_LED_flag*2) && LED_flag < (set_LED_flag*2))
	//{
		//gpio_toggle(port_b,5);
		//LED_flag++;
		//SYS_TimerStart(&appLEDTimer);
	//}
	//else
	//{
		//gpio_clr(port_b,5);
		//LED_flag = 0;
		//SYS_TimerStart(&appLEDIntervalTimer);
	//}
	//(void)timer;
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void pne_APP_GpioInit(void)
//{
	////IO Declaration
	////portb5 led
	////portb3 button
	//gpio_out(port_b,5);
	//gpio_set(port_b,5);
	//
	//gpio_out(port_b,7);
	//gpio_set(port_b,7);
	//
	//gpio_out(port_b,4);
	//gpio_set(port_b,4);
//
	//gpio_in(port_b,3);
	//gpio_pullup(port_b,3);
	//
	////Timer Declaration
	//appLEDTimer.interval = 100;
	//appLEDTimer.mode = SYS_TIMER_INTERVAL_MODE;
	//appLEDTimer.handler = appLEDTimerHandler;
	//SYS_TimerStart(&appLEDTimer);
	//
	//appLEDIntervalTimer.interval = 2000;
	//appLEDIntervalTimer.mode = SYS_TIMER_INTERVAL_MODE;
	//appLEDIntervalTimer.handler = appLEDIntervalTimerHandler;
	//
	////appStatusUpdateTimer.interval = 2000;
	////appStatusUpdateTimer.mode = SYS_TIMER_PERIODIC_MODE;
	////appStatusUpdateTimer.handler = appStatusUpdateTimerHandler;
	////SYS_TimerStart(&appStatusUpdateTimer);
//}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//AppPNEWELS_OperationState_t pne_APP_GpioTaskHandler(void)
//{	
	//uint8_t state;
	//AppPNEWELS_OperationState_t mode = APP_PNEWELS_STATE_NORMAL_OPERATION;
	//
	//state = gpio_read(port_b,3) ? 0 : 1;
	//
	//if (state == 1)
	//{
		//while (state)
		//{
			//state = gpio_read(port_b,3) ? 0 : 1;
		//}
		//number_press++;
	//}
	//else
	//{
		//number_press = number_press;
	//}
	//
	//switch (number_press)
	//{
		//case 0:
		//{
			//mode = APP_PNEWELS_STATE_NORMAL_OPERATION;
			//set_LED_flag = 1;
			//break;
		//}
		//case 1:
		//{
			//mode = APP_PNEWELS_STATE_WAITING_PAIRING;
			//set_LED_flag = 2;
			//break;
		//}
		//case 2:
		//{
			//mode = APP_PNEWELS_STATE_FACTORY_MODE;
			//set_LED_flag = 3;
			//break;
		//}
		//case 3:
		//{
			//mode = APP_PNEWELS_STATE_EMERGENCY_MODE;
			//set_LED_flag = 4;
			//break;
		//}
		//case 4:
		//{
			//mode = APP_PNEWELS_STATE_BATTERY_LOW;
			//set_LED_flag = 5;
			//break;
		//}
		//default:
		//{
			//number_press = 0;
			//mode = APP_PNEWELS_STATE_NORMAL_OPERATION;
			//set_LED_flag = 1;
			//break;
		//}
	//}
	//
	//return mode;
//}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////uint8_t appPNEWELS_GetTemperature(void)
////{
	////return data_received[0];
////}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////void appPNEWELS_Pairing(void)
////{
	//////get from network()
	//////send to network()
////}
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////void appPNEWELS_Normal(void)
////{
	////if (update_flag == 1)
	////{
		////uint8_t temperature_write_address = 0x96;
		////uint8_t temperature_read_address = 0x97;
		////uint8_t temperature_register = 0x00;
		////
		////update_flag = 0;
		////appPNEWELS_TWIInit(temperature_write_address);
		////if (appPNEWELS_TWISend(temperature_register, temperature_write_address, 0, 0) == APP_PNEWELS_TWI_WRITE_SUCCESS)
		////{
			////appPNEWELS_TWIReceive(temperature_register, temperature_read_address, data_received, 2);
		////}
		////else {}		
		////
		//////appPNEWELS_USARTSend(data_received, 2);
		////
	////}
	////else {}
////}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////void appPNEWELS_Factory(void)
////{
	//////set serial number()
////}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////void appPNEWELS_Emergency(void)
////{
	//////store routine()
////}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////void appPNEWELS_BatteryLow(void)
////{
	//////off led()
////}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////void appPNEWELS_TaskHandle(void)
////{
	////AppPNEWELS_OperationState_t mode = APP_PNEWELS_STATE_NORMAL_OPERATION;	
	////
	////mode = appPNEWELS_Button();
	////
	////switch (mode)
	////{
		////case APP_PNEWELS_STATE_NORMAL_OPERATION:
		////{
			////appPNEWELS_Normal();
			////break;
		////}
		////case APP_PNEWELS_STATE_WAITING_PAIRING:
		////{
			////appPNEWELS_Pairing();
			////break;
		////}
		////case APP_PNEWELS_STATE_FACTORY_MODE:
		////{
            ////appPNEWELS_Factory();
			////break;
		////}
		////case APP_PNEWELS_STATE_EMERGENCY_MODE:
		////{
            ////appPNEWELS_Emergency();
			////break;
		////}
		////case APP_PNEWELS_STATE_BATTERY_LOW:
		////{
            ////appPNEWELS_BatteryLow();
			////break;
		////}
		////default:
		////{
			////break;
		////}
	////}
////}
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////