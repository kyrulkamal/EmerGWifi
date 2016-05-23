#include "ext_int_megarf.h"
#include "PNEWELS.h"
#include "pne_appDelay.h"
#include "adc.h"
#include "pneConfig.h"
#include "pne_appTwi.h"
#include "appNwk.h"
#include <stdlib.h>
#include "config.h"
#include "crc.h"
#include "sys.h"
#include "PNE_Factory.h"
#include "pne_rf_processor.h"
#include <string.h>
#include "memoryConfig.h"
#include <stdio.h>
#include <stdint.h>
#include "appBattery.h"
#include "appVerify.h"
#include "appIb.h"

uint8_t UID[max_UID_length] = {0}; //UID cache

//ISR
static void manual_btn_isr(void)
{
	interrupt_flag = 1;
	manual_btn_handler();
/*	operation_seq = on_emergency_light;*/
}

//Soft Timer
static void appLEDIntervalTimerHandler(SYS_Timer_t *timer)
{
	SYS_TimerStart(&appLEDTimer);
	(void)timer;
}

static void appLEDTimerHandler(SYS_Timer_t *timer)
{
	if (LED_flag < 2)
	{
		switch(PNEWELS_Buffer.mode)
		{
			case MODE_WPS:	
				gpio_clr(STATUS_LED_2);
				gpio_toggle(STATUS_LED_1);
			break;
			
			case MODE_STANDBY:	
				gpio_toggle(STATUS_LED_1);
				gpio_clr(STATUS_LED_2);
			break; 
			
			case MODE_FACTORY:
				gpio_toggle(STATUS_LED_1);
				gpio_toggle(STATUS_LED_2);
			break;
			
			case MODE_Emergency:
				gpio_toggle(STATUS_LED_1);
				gpio_clr(STATUS_LED_2);
			break;
			
			default:
				gpio_clr(STATUS_LED_1);
				gpio_clr(STATUS_LED_2);
			break;
		}
		
		LED_flag++;
		SYS_TimerStart(&appLEDTimer);
	}
	else
	{
		gpio_clr(STATUS_LED_1);
		gpio_clr(STATUS_LED_2);
		LED_flag = 0;
		SYS_TimerStart(&appLEDIntervalTimer);
	}
	//scanChannelEnergy();
	wdt_reset(); //watchdog moved here from mainloop due to sometimes routing will lock the device down and halt the main operation prevent the LED from blinking. This can help avoid that
	(void)timer;
}

static void errorTimerStatusHandler(SYS_Timer_t *timer)
{
	error_count_status++;
	if (error_count_status>max_rf_retries)
	{
		error_count_status = 0;
	}
	else
	{
		status_report_flag = 1; 
		//send_usart_char("\r\nstatus rf retrying");
	}
	(void)timer;
}

static void errorTimerWpsHandler(SYS_Timer_t *timer)
{
	wps_send_flag = 1;
	send_usart_char("\r\nwps rf retrying");
	(void)timer;
}

static void appWPSRequestTimerHandler(SYS_Timer_t *timer)
{
	request_to_RF(UID);
	//SYS_TimerStart(&appWPSRequestTimer);
	(void)timer;
}

static void appWPSActivationTimerHandler(SYS_Timer_t *timer)
{
	if(wps_flag == 1)
	{
		SYS_TimerStop(&appWPSActivationTimer);
		operation_seq = wps_state;
		reboot_countdown = 0;
		APP_IbLoadSettings_WPS(); //load temporary setting
		SYS_TimerStart(&appWPSRequestTimer); //start requesting data
		/*wps_flag = 0;*/
	}
	else if(wps_flag == 0)
	{
		SYS_TimerStop(&appWPSActivationTimer);
		operation_seq = off_emergency_light;
		APP_IbLoadSettings();
		APP_NwkInit();
		SYS_TimerStop(&appWPSRequestTimer);
		SYS_TimerStop(&error_timer_wps);
	}
	(void)timer;
}

//Initialization
void PNEWELSGpioInit(void)
{
	MANUAL_BTN.port_id = port_b;
	MANUAL_BTN.pin_id = 3;
	MANUAL_BTN.int_id = PC_INT3_PIN;
	ext_int_set_interrupt_callback(MANUAL_BTN, manual_btn_isr);
	ext_int_pcint_init(MANUAL_BTN);
	
	WPS_BTN.port_id = port_b;
	WPS_BTN.pin_id = 2;
	WPS_BTN.int_id = PC_INT2_PIN;
//	ext_int_set_interrupt_callback(WPS_BTN, wps_btn_isr);
	ext_int_pcint_init(WPS_BTN);
	
	TEMP_ALRT.port_id = port_b;
	TEMP_ALRT.pin_id = 1;
	TEMP_ALRT.int_id = PC_INT1_PIN;
	//ext_int_set_interrupt_callback(TEMP_ALRT, temp_alert_isr);
	ext_int_pcint_init(TEMP_ALRT);
	
	VCHARGE_STAT.port_id = port_b;
	VCHARGE_STAT.pin_id = 0;
	VCHARGE_STAT.int_id = PC_INT0_PIN;
	//ext_int_set_interrupt_callback(AC_STAT, ac_stat_isr);
	ext_int_pcint_init(VCHARGE_STAT);
	
	FACTORY_BTN.port_id = port_d;
	FACTORY_BTN.pin_id = 4;
	gpio_in(FACTORY_BTN);
	gpio_pullup(FACTORY_BTN);
	
	STATUS_LED_1.port_id = port_e;
	STATUS_LED_1.pin_id = 5;
	gpio_out(STATUS_LED_1);
	
	STATUS_LED_2.port_id = port_e;
	STATUS_LED_2.pin_id = 7;
	gpio_out(STATUS_LED_2);
	
	CHARGING_DISABLE.port_id = port_e;
	CHARGING_DISABLE.pin_id = 4;
	gpio_out(CHARGING_DISABLE);
	
	EMER_LIGHT_EN.port_id = port_e;
	EMER_LIGHT_EN.pin_id = 3;
	gpio_out(EMER_LIGHT_EN);
	
    /////////////////////////////////
	OUT_LED_CH0.pin_id = port_b;
	OUT_LED_CH0.pin_id = 5;
	gpio_out(OUT_LED_CH0);
	
	OUT_LED_CH1.pin_id = port_b;
	OUT_LED_CH1.pin_id = 6;
	gpio_out(OUT_LED_CH1);
	
	OUT_LED_CH2.pin_id = port_b;
	OUT_LED_CH2.pin_id = 7;
	gpio_out(OUT_LED_CH2);
	
	////////////////////////////////
	AC_STAT.port_id = port_e;
	AC_STAT.pin_id = 0;
	gpio_in(AC_STAT);
	gpio_pullup(AC_STAT);
	
	INPUT_LED_CH1.port_id = port_e;
	INPUT_LED_CH1.pin_id = 1;
	gpio_in(INPUT_LED_CH1);
	gpio_pullup(INPUT_LED_CH1);
	
	INPUT_LED_CH2.port_id = port_e;
	INPUT_LED_CH2.pin_id = 2;
	gpio_in(INPUT_LED_CH2);
	gpio_pullup(INPUT_LED_CH2);
	
	INPUT_LED_DRV0.port_id = port_d;
	INPUT_LED_DRV0.pin_id = 5;
	gpio_in(INPUT_LED_DRV0);
	gpio_pullup(INPUT_LED_DRV0);
	
	INPUT_LED_DRV1.port_id = port_d;
	INPUT_LED_DRV1.pin_id = 6;
	gpio_in(INPUT_LED_DRV1);
	gpio_pullup(INPUT_LED_DRV1);
	
	INPUT_LED_DRV2.port_id = port_d;
	INPUT_LED_DRV2.pin_id = 7;
	gpio_in(INPUT_LED_DRV2);
	gpio_pullup(INPUT_LED_DRV2);
}

void PNEWELSAppInit(void)
{
	initialize_flag = 1;
	installation_flag = 1;
	data_update_flag = 1;
	status_report_flag = 1;
	interrupt_flag = 1;
	debouncing_delay = 0;
	LED_flag = 0;
	wps_flag = 0;
	do_once = 0;
	operation_seq = off_emergency_light;
	current_state_emergency_light = 0;
	memory_address_incr = 0;
	error_count_status = 0;
	error_count_wps = 0;
	wps_send_flag = 1;
	battery_low_once = 0;
	WPS_STATUS = WPS_init;
	rf_flag = 0;
	countdown = 0;
	//do_once_stop_wps_timer = 0;
	
	adc_init(ADC_PRESCALER_DIV128);
	timer2_init();
	memoryCounter_init();
	
	static usart_rs232_options_t USART_SERIAL_OPTIONS =
	{
		.baudrate = USART_SERIAL_EXAMPLE_BAUDRATE,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
	};
	usart_init_rs232(USART_SERIAL_EXAMPLE, &USART_SERIAL_OPTIONS);
	
	/*Software timer declaration*/
	appLEDTimer.interval = STANDBY_TIMER;
	appLEDTimer.mode = SYS_TIMER_INTERVAL_MODE;
	appLEDTimer.handler = appLEDTimerHandler;
	SYS_TimerStart(&appLEDTimer);
	
	appLEDIntervalTimer.interval = STANDBY_INTERVAL_TIMER;
	appLEDIntervalTimer.mode = SYS_TIMER_INTERVAL_MODE;
	appLEDIntervalTimer.handler = appLEDIntervalTimerHandler;
	
	error_timer_status.interval = rand()%400+100;
	error_timer_status.mode = SYS_TIMER_INTERVAL_MODE;
	error_timer_status.handler = errorTimerStatusHandler;
	
	error_timer_wps.interval = rand()%400+100;
	error_timer_wps.mode = SYS_TIMER_INTERVAL_MODE;
	error_timer_wps.handler = errorTimerWpsHandler;
	/*SYS_TimerStart(&error_timer_wps);*/
	
	appWPSRequestTimer.interval = WPS_REQUEST_TIMER;
	appWPSRequestTimer.mode = SYS_TIMER_PERIODIC_MODE;
	appWPSRequestTimer.handler = appWPSRequestTimerHandler;
	/*SYS_TimerStart(&appWPSRequestTimer);*/
	
	appWPSActivationTimer.interval = WPS_REQUEST_TIMER;
	appWPSActivationTimer.mode = SYS_TIMER_INTERVAL_MODE;
	appWPSActivationTimer.handler = appWPSActivationTimerHandler;
	
	pneBatteryAverage_init(); //get average battery reading
	getUID(); //retrieve UID
	
	/*Factory Status*/
	if(gpio_read(FACTORY_BTN) ? 0 : 1)
	{
		PNEWELS_Buffer.IsFactory = STATUS_FACTORY_OK;
		PNEWELS_Buffer.mode_Next = MODE_FACTORY;
		factory_mode();
	}
	else
	{
		PNEWELS_Buffer.IsFactory = STATUS_FACTORY_NOK;
	}
}


//Sequences
void PNEWELSTaskHandler(void)
{
	pneGetStatus();
	pneOperationMode();
	pneOutgoingData();
}

void pneGetStatus()
{
	if (data_update_flag)
	{	
		PNEWELS_Buffer.V_BATT = pneBatteryAverage();//adc_read_8bit(ADC_BATTERY_INPUT, ADC_VREF)*18*10/255;
		PNEWELS_Buffer.temperature = PNEWELSTemperatureRead();
		PNEWELS_Buffer.PWM_LED = 0xF0;
		data_update_flag = 0;
		PNEWELS_Buffer.V_LED = adc_read_8bit(ADC_EMER_LIGHT_INPUT, ADC_VREF)*18*2/255;
		PNEWELS_Buffer.STATUS_AC = gpio_read(AC_STAT) ? STATUS_AC_NOK : STATUS_AC_OK; //might need debouncing
		/* Firmware Schmitt Trigger for Battery Status */
		if(PNEWELS_Buffer.V_BATT </*=*/ batt_low - V_BATT_SCHMITT_TRIGGER)
		{
			PNEWELS_Buffer.IsBatt = STATUS_BATT_NOK;
			PNEWELS_Buffer.IsPowerOK = STATUS_POWER_DOWN;
		}
		else if(PNEWELS_Buffer.V_BATT >= batt_low+V_BATT_SCHMITT_TRIGGER)
		{
			PNEWELS_Buffer.IsBatt = STATUS_BATT_OK;
		}
		else
		{
			PNEWELS_Buffer.IsBatt = STATUS_BATT_NEU;
		}
		if(PNEWELS_Buffer.temperature > 60)
		{
			error_to_rf(temperature_alert);
		}
		if(PNEWELS_Buffer.V_BATT < 0x65)
		{
			error_to_rf(battery_dead_alert);
		}
		
	}
	
	if (initialize_flag == 1 || interrupt_flag == 1)
	{
		if (debouncing_delay>8)
		{
			PNEWELS_Buffer.BUTTON_MANUAL = gpio_read(MANUAL_BTN) ? BUTTON_MANUAL_DEPRESSED : BUTTON_MANUAL_PRESSED;
			manual_btn_handler();
			PNEWELS_Buffer.BUTTON_WPS = gpio_read(WPS_BTN) ? BUTTON_WPS_DEPRESSED : BUTTON_WPS_PRESSED;
			PNEWELS_Buffer.TEMP_ALARM = gpio_read(TEMP_ALRT) ? TEMP_NALERT : TEMP_ALERT;
			PNEWELS_Buffer.STATUS_VCHARGE = gpio_read(VCHARGE_STAT) ? STATUS_VCHARGE_NOK : STATUS_VCHARGE_OK;
			PNEWELS_Buffer.STATUS_AC = gpio_read(AC_STAT) ? STATUS_AC_NOK : STATUS_AC_OK;
			PNEWELS_Buffer.led_ch1 = gpio_read(INPUT_LED_CH1) ? BUTTON_CH1_DEPRESSED : BUTTON_CH1_PRESSED;
			PNEWELS_Buffer.led_ch2 = gpio_read(INPUT_LED_CH2) ? BUTTON_CH2_DEPRESSED : BUTTON_CH2_PRESSED;
			PNEWELS_Buffer.led_drv0 = gpio_read(INPUT_LED_DRV0) ? BUTTON_DRV0_DEPRESSED : BUTTON_DRV0_PRESSED;
			PNEWELS_Buffer.led_drv1 = gpio_read(INPUT_LED_DRV1) ? BUTTON_DRV1_DEPRESSED : BUTTON_DRV1_PRESSED;
			PNEWELS_Buffer.led_drv2 = gpio_read(INPUT_LED_DRV2) ? BUTTON_DRV2_DEPRESSED : BUTTON_DRV2_PRESSED;
			if (((PNEWELS_Buffer.soft_manual == 1) && (PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_OK)) || ((PNEWELS_Buffer.BUTTON_MANUAL == BUTTON_MANUAL_PRESSED) && (PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_OK)))
			{
				error_to_rf(hardware_error);
			}
			initialize_flag = 0;
			debouncing_delay = 0;
			
			if ((PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_OK) && (PNEWELS_Buffer.STATUS_AC == STATUS_AC_OK))//((device_flag & (1<<AC_FLAG)) != 0))
			{
				
				installation_flag = 0;
				//uint8_t set_flag = (device_flag & ~(1<<AC_FLAG));
				//writeE2prom(FLAG,set_flag);
			}
			
			if (interrupt_flag == 1)
			{
				status_report_flag = 1;
				interrupt_flag = 0;
			}
		}
		else
		{
			debouncing_delay++;
		}
	}
}

void pneOperationMode()
{
	switch(operation_seq)
	{
		case off_emergency_light:
// 			send_usart_char("\r\nstandby mode");
			wps_flag = 0;
			wps_send_flag = 1;
			gpio_clr(OUT_LED_CH0);//gpio_set(OUT_LED_CH0);
			PNEWELS_Buffer.IsOutputEmergencyLED = 0;
			PNEWELS_Buffer.mode_Next = off_emergency_light;
			PNEWELS_Buffer.mode = MODE_STANDBY;
			PNEWELS_Buffer.IsPowerOK = STATUS_POWER_UP;
			appLEDTimer.interval = STANDBY_TIMER;
			appLEDIntervalTimer.interval = STANDBY_INTERVAL_TIMER;
			if ((PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_NOK) && (installation_flag ==0) && (gpio_read(CHARGING_DISABLE) == 0)) //unverified yet. Need to be tested on psu
			{
				operation_seq = on_emergency_light;
			}
			if ((PNEWELS_Buffer.soft_manual == 1) || (PNEWELS_Buffer.BUTTON_MANUAL == BUTTON_MANUAL_PRESSED))
			{
				operation_seq = on_emergency_light;
			}
			if (PNEWELS_Buffer.BUTTON_WPS == BUTTON_WPS_PRESSED)
			{
				operation_seq = three_sec_handling_state;
// 				APP_IbLoadSettings_WPS(); //load temporary setting
// 				SYS_TimerStart(&appWPSRequestTimer); //start requesting data
			}
			if (PNEWELS_Buffer.soft_charge == 1)
			{
				operation_seq = charging_test;
			}
			if (PNEWELS_Buffer.soft_discharge == 1)
			{
				operation_seq = discharging_test;//PNEWELS_Buffer.soft_discharge = 0;
				rf_flag = 0;
			}

		break;
		
		case on_emergency_light:
// 			send_usart_char("\r\nemergency mode");
			if(rf_flag == 1) //to put hold temporary the interrupt
				rf_flag++;
			else
				rf_flag = 0;
			wps_flag = 0;
			wps_send_flag = 1;
			PNEWELS_Buffer.mode = MODE_Emergency;
			appLEDTimer.interval = EMERGENCY_TIMER;
			appLEDIntervalTimer.interval = EMERGENCY_INTERVAL_TIMER;
			if ((PNEWELS_Buffer.IsBatt == STATUS_BATT_NOK) || (PNEWELS_Buffer.IsPowerOK == STATUS_POWER_DOWN))
			{
				gpio_clr(OUT_LED_CH0);//gpio_set(OUT_LED_CH0);
				PNEWELS_Buffer.IsOutputEmergencyLED = 0;
				if(battery_low_once == 0)
				{
					store(battery_discharged);
					battery_low_once = 1;
				}
			}
			if ((PNEWELS_Buffer.IsBatt == STATUS_BATT_OK) && (PNEWELS_Buffer.IsPowerOK == STATUS_POWER_UP))
			{
				gpio_set(OUT_LED_CH0);//gpio_clr(OUT_LED_CH0);
				PNEWELS_Buffer.IsOutputEmergencyLED = 1;
			}
			if (do_once == 0)
			{
				store(emergency_start);
				do_once = 1;
			}
			if (PNEWELS_Buffer.soft_discharge == 1)
			{
				operation_seq = discharging_test;
				do_once = 0;
			}
			if ((PNEWELS_Buffer.STATUS_AC == STATUS_AC_OK) && (PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_OK) && (PNEWELS_Buffer.soft_manual == 0) && (PNEWELS_Buffer.BUTTON_MANUAL == BUTTON_MANUAL_DEPRESSED))
			{
				store(emergency_end);
				do_once = 0;
				battery_low_once = 0; //flag for battery low
				operation_seq = off_emergency_light;
			}
			if ((PNEWELS_Buffer.STATUS_AC == STATUS_AC_NOK) && (PNEWELS_Buffer.soft_manual == 1))
			{
				gpio_clr(CHARGING_DISABLE);
				PNEWELS_Buffer.soft_manual = 0;
				PNEWELS_Buffer.BUTTON_MANUAL = BUTTON_MANUAL_DEPRESSED;
			}
			if (PNEWELS_Buffer.soft_charge == 1)
			{
				PNEWELS_Buffer.soft_charge = 0;
			}
			if ((PNEWELS_Buffer.BUTTON_MANUAL == BUTTON_MANUAL_PRESSED) && (PNEWELS_Buffer.BUTTON_WPS == BUTTON_WPS_PRESSED))
			{
				countdown++;
				if(countdown > 65000)
				{
					uint8_t memory_address_low = (FLAG & 0xFF);
					uint8_t memory_address_high = ((FLAG >> 8) & 0xFF);
					reset_cache = readE2prom(FLAG);
					reset_cache = (reset_cache | 0x03);
					PNEWELSE2promWrite(0xAE,'E',memory_address_low,memory_address_high,reset_cache);
					reboot_to_rf();
				}
			}
			else countdown = 0;
		break;
		
		case discharging_test:
// 			send_usart_char("\r\ndischarging mode");
			/*gpio_set(CHARGING_DISABLE); *///new addition
			PNEWELS_Buffer.mode = MODE_DISCHARGING;
			gpio_set(OUT_LED_CH0);//turn on led only. No need to to report emergency
			PNEWELS_Buffer.IsOutputEmergencyLED = 1;
			wps_flag = 0;
			wps_send_flag = 1;
			if (do_once == 0)
			{
				store(discharge_start);
				do_once = 1;
				pend_to_rf('d');
				gpio_set(CHARGING_DISABLE); 
				/*rf_flag = 0; //13/1/16*/
			}
			if (PNEWELS_Buffer.STATUS_AC == STATUS_AC_NOK)
			{
				store(discharge_end);
				test_completed('a', 2);
				do_once = 0;
				rf_flag = 1;
				operation_seq = on_emergency_light;
				PNEWELS_Buffer.soft_discharge = 0;
				gpio_clr(CHARGING_DISABLE); //reconnect Vcharge
				/*gpio_clr(OUT_LED_CH0);//turn of LED*/
				PNEWELS_Buffer.IsOutputEmergencyLED = 0; //clear report
				PNEWELS_Buffer.STATUS_VCHARGE = gpio_read(VCHARGE_STAT) ? STATUS_VCHARGE_NOK : STATUS_VCHARGE_OK;
			}
			else if (/*(PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_OK) && (*/PNEWELS_Buffer.IsBatt == STATUS_BATT_NOK/*)*/)
			{
				store(discharge_end);
				test_completed('n', 2);
				do_once = 0;
				rf_flag = 1;
				operation_seq = off_emergency_light;
				PNEWELS_Buffer.soft_discharge = 0;
				gpio_clr(CHARGING_DISABLE); //reconnect Vcharge
				/*gpio_clr(OUT_LED_CH0);//turn of LED*/
				PNEWELS_Buffer.IsOutputEmergencyLED = 0; //clear report
				PNEWELS_Buffer.STATUS_VCHARGE = gpio_read(VCHARGE_STAT) ? STATUS_VCHARGE_NOK : STATUS_VCHARGE_OK;
			}
			else if (PNEWELS_Buffer.soft_discharge != 1)
			{
				store(discharge_end);
				test_completed('s', 2);
				do_once = 0;
				rf_flag = 1;
				operation_seq = off_emergency_light;
				PNEWELS_Buffer.soft_discharge = 0;
 				gpio_clr(CHARGING_DISABLE); //reconnect Vcharge
				/*gpio_clr(OUT_LED_CH0);//turn of LED*/
				PNEWELS_Buffer.IsOutputEmergencyLED = 0; //clear report
				PNEWELS_Buffer.STATUS_VCHARGE = gpio_read(VCHARGE_STAT) ? STATUS_VCHARGE_NOK : STATUS_VCHARGE_OK;

			}
		break;
		
		case charging_test:
// 			send_usart_char("\r\ncharging mode");
			PNEWELS_Buffer.mode = MODE_CHARGING;
			wps_flag = 0;
			wps_send_flag = 1;
			PNEWELS_Buffer.mode_Next = charging_test;
			if (do_once == 0)
			{
				store(charge_start);
				do_once = 1;
				pend_to_rf('c');
			}
			if ((PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_NOK) && (installation_flag ==0) /*&& (gpio_read(CHARGING_DISABLE) == 0)*/)
			{
				store(charge_end);
				test_completed('a', 2);
				do_once = 0;
				rf_flag = 1;
				operation_seq = off_emergency_light;
				PNEWELS_Buffer.soft_charge = 0;
			}
			else if (PNEWELS_Buffer.V_BATT >= batt_max)
			{
				store(charge_end);
				test_completed('n', 2);
				do_once = 0;
				rf_flag = 1; //19/1
				operation_seq = off_emergency_light;
				PNEWELS_Buffer.soft_charge = 0;
 			}
			else if (PNEWELS_Buffer.soft_charge != 1)
			{
				store(charge_end);
				test_completed('s', 2);
				do_once = 0;
				rf_flag = 1;
				operation_seq = off_emergency_light;
				PNEWELS_Buffer.soft_charge = 0;     
			}
		break;
		
		case three_sec_handling_state:
// 			send_usart_char("\r\n3s handling 1");
			if(wps_flag == 0)
			{
				SYS_TimerStart(&appWPSActivationTimer);
				wps_flag++;
			}
			if (debouncing_delay>8)
			{
				PNEWELS_Buffer.BUTTON_WPS = gpio_read(WPS_BTN) ? BUTTON_WPS_DEPRESSED : BUTTON_WPS_PRESSED;
				debouncing_delay = 0;
			}
			else
			{
				debouncing_delay++;
			}
			if (PNEWELS_Buffer.BUTTON_WPS == BUTTON_WPS_DEPRESSED)
			{
				SYS_TimerStop(&appWPSActivationTimer);
				operation_seq = off_emergency_light;
				wps_flag = 0;
			}
			if ((PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_NOK) && (installation_flag ==0) /*&& (gpio_read(CHARGING_DISABLE) == 0)*/)
			{
				operation_seq = on_emergency_light;
				wps_flag = 0;
			}
		break;
		
		case wps_state:
// 			send_usart_char("\r\nwps mode");
			appLEDTimer.interval = WPS_TIMER;
			appLEDIntervalTimer.interval = WPS_INTERVAL_TIMER;
			/*wps_flag = 0;*/
			wps_send_flag = 0;
			PNEWELS_Buffer.mode = MODE_WPS;
			switch(WPS_STATUS)
			{
				case WPS_init:
					//gpio_set(OUT_LED_CH0);//gpio_clr(OUT_LED_CH0);
					//do_once_stop_wps_timer = 1;
					//at this state device keep sending request
					break;
				
				case WPS_acknowledged:
// 					if(do_once_stop_wps_timer == 1)
// 					{
					SYS_TimerStop(&appWPSRequestTimer);
						//do_once_stop_wps_timer = 0;
						/*send_to_rf(UID,max_UID_length,"[ready!]");*/
/*					}*/
					WPS_STATUS = WPS_handshaked;
					break;
					
				case WPS_handshaked:
					WPS_OK_Alert(UID, max_UID_length);
					WPS_STATUS = WPS_done;
					break;
				
				case WPS_done:
					if(reboot_countdown > 50000)
					{
						if(device_flag & (1<<AC_FLAG))
						{
							device_flag = (device_flag & ~(1<<AC_FLAG)); //write the flag
							writeE2prom(FLAG,device_flag);
						}						
						reboot_to_rf();
					}
					else
					{
						reboot_countdown++;
					}
// 					pne_delayms(1000);
// 					reboot_to_rf();
					break;
					
				default:
					error_to_rf(wps_invalid_data_size);
					break;
			}
			
// 			if(wps_flag == 0)
// 			{
// 				SYS_TimerStart(&appWPSActivationTimer);
// 				wps_flag++;
// 			}
			if (debouncing_delay>8)
			{
				PNEWELS_Buffer.BUTTON_WPS = gpio_read(WPS_BTN) ? BUTTON_WPS_DEPRESSED : BUTTON_WPS_PRESSED;
				PNEWELS_Buffer.STATUS_VCHARGE = gpio_read(VCHARGE_STAT) ? STATUS_VCHARGE_NOK : STATUS_VCHARGE_OK;
				debouncing_delay = 0;
			}
			else
			{
				debouncing_delay++;
			}
			if (PNEWELS_Buffer.BUTTON_WPS == BUTTON_WPS_PRESSED)
			{
				if(wps_flag == 1)
				{
					SYS_TimerStart(&appWPSActivationTimer);
					wps_flag = 0;
				}
			}
			else if(PNEWELS_Buffer.BUTTON_WPS == BUTTON_WPS_DEPRESSED)
			{
				if(wps_flag == 0)
				{
					SYS_TimerStop(&appWPSActivationTimer);
					}
				wps_flag = 1;
			}
			
// 			if (PNEWELS_Buffer.BUTTON_WPS == BUTTON_WPS_DEPRESSED)
// 			{
// 				WPS_STATUS = WPS_init;
// 				operation_seq = off_emergency_light;
// 				APP_IbLoadSettings();
// 				APP_NwkInit();
// 				SYS_TimerStop(&appWPSRequestTimer);
// 				SYS_TimerStop(&error_timer_wps);
// 				
// 			}
			if ((PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_NOK) && (installation_flag ==0))
			{
				WPS_STATUS = WPS_init;
				operation_seq = off_emergency_light;
				APP_IbLoadSettings();
				APP_NwkInit();
				SYS_TimerStop(&error_timer_wps);
				SYS_TimerStop(&appWPSRequestTimer);
			}
//			if (wps_send_flag == 1)
// 			{
// 				wps_send_to_rf();
// 				SYS_TimerStart(&error_timer_wps);
// 				wps_send_flag = 0;
// 			}
		break;
		
// 		case three_sec_handling_state_2:
// // 			send_usart_char("\r\n3s handling 2");
// 			if (PNEWELS_Buffer.BUTTON_WPS == BUTTON_WPS_PRESSED)
// 			{
// 				wps_flag++;
// 			}
// 			if (wps_flag > 500)
// 			{
// 				operation_seq = off_emergency_light;
// 				SYS_TimerStop(&error_timer_wps);
// 				wps_send_flag = 1;
// 			}
// 			if ((PNEWELS_Buffer.STATUS_VCHARGE == STATUS_VCHARGE_NOK) && (installation_flag ==0))
// 			{
// 				operation_seq = on_emergency_light;
// 				SYS_TimerStop(&error_timer_wps);
// 				wps_send_flag = 1;
// 			}
// 			if (PNEWELS_Buffer.BUTTON_WPS == BUTTON_WPS_DEPRESSED)
// 			{
// 				operation_seq = wps_state;
// 			}
// 		break;
	}
}

void pneOutgoingData()
{
	if (status_report_flag)
	{	
			if(rf_flag != 0)
			{
				rf_flag = 0;
			}
			else
			{
				status_send_to_rf();
			}
/*			status_send_to_rf();*/
			SYS_TimerStart(&error_timer_status);
			status_report_flag = 0;
			/*request_to_RF(UID);*/ //for testing purpose
	}
}


//Incoming Data
bool pneIncomingData(uint8_t *data, uint8_t size)
{
	//Signature verification part. If not same as gateway, return false.
	signatureVerify = pneVerify_received(data, size);
	if (signatureVerify == FALSE)
	{
		return false;
	}
	
	if (memcmp(data, "[emeron]", max_rf_command_length) == 0)
	{
		if ((operation_seq == on_emergency_light) && (PNEWELS_Buffer.soft_manual == 1))
		{
			error_to_rf(conflicted_command);
		}
		else
		{
			emergency_on_to_rf();
			manual_btn_handler();
		}
/*		status_report_flag = 1;*/
	}
	else if (memcmp(data, "[emerof]", max_rf_command_length) == 0)
	{
		if ((operation_seq == off_emergency_light) && (PNEWELS_Buffer.soft_manual == 0))
		{
			error_to_rf(conflicted_command);
		}
		else
		{
			emergency_off_to_rf();
			manual_btn_handler();
		}
		/*		status_report_flag = 1;*/
	}
	else if (memcmp(data, "[rd rom]", max_rf_command_length) == 0)
	{
		uint16_t address;
		if (size<16)
		{
			error_to_rf(read_eeprom_error);
		}
		else
		{
			if (crc_verify(data, size-2)) //remove signature bit
			{
				uint16_t datatmp2[4] = {0,0,0,0};
				datatmp2[0] = ((uint16_t)(data[9]-0x30)*(uint16_t)(1000));
				datatmp2[1] = ((uint16_t)(data[10]-0x30)*(uint16_t)(100));
				datatmp2[2] = ((uint16_t)(data[11]-0x30)*(uint16_t)(10));
				datatmp2[3] = ((uint16_t)(data[12]-0x30)*(uint16_t)(1));
				address = datatmp2[0]+datatmp2[1]+datatmp2[2]+datatmp2[3];
				if ((address>4092) || (address<0))
				{
					error_to_rf(read_eeprom_error);
				}
				else
				{
					eeprom_to_rf(address);
				}
			}
		}
	}
	else if (memcmp(data, "[chrgon]", max_rf_command_length) == 0)
	{
		
		if(operation_seq == on_emergency_light)
			error_to_rf(conflicted_command);
		else
		{
  			PNEWELS_Buffer.soft_charge = 1;
/*			if (PNEWELS_Buffer.soft_charge == 1);*/
// 	  			pend_to_rf('c');
		}
	}
	else if (memcmp(data, "[chrgof]", max_rf_command_length) == 0)
	{
		
		if(operation_seq == on_emergency_light)
			error_to_rf(conflicted_command);
		else
		{
			PNEWELS_Buffer.soft_charge = 0;
			/*			if (PNEWELS_Buffer.soft_charge == 1);*/
			// 	  			pend_to_rf('c');
		}
	}
	else if (memcmp(data, "[discon]", max_rf_command_length) == 0)
	{
		if(operation_seq == on_emergency_light)
			error_to_rf(conflicted_command);
		else
		{
  			PNEWELS_Buffer.soft_discharge = 1;
/*			if (PNEWELS_Buffer.soft_discharge == 1);*/
/*  				pend_to_rf('d');*/
  			/*gpio_set(CHARGING_DISABLE);*/
		}
		/*PNEWELS_Buffer.IsOutputEmergencyLED = 1;*/
		/*pend_to_rf('d');*/
	}
	else if (memcmp(data, "[discof]", max_rf_command_length) == 0)
	{
		if(operation_seq == on_emergency_light)
			error_to_rf(conflicted_command);
		else
		{
			PNEWELS_Buffer.soft_discharge = 0;
			/*			if (PNEWELS_Buffer.soft_discharge == 1);*/
			/*  				pend_to_rf('d');*/
			/*gpio_set(CHARGING_DISABLE);*/
		}
		/*PNEWELS_Buffer.IsOutputEmergencyLED = 1;*/
		/*pend_to_rf('d');*/
	}
	else if (memcmp(data, "[timech]", max_rf_command_length) == 0)
	{
		uint32_t timestamprf = 0;
		
		if (size<20)	//correct size including signature is 22
		{
			error_to_rf(time_invalid_data_size);
		}
		else
		{
			if ((crc_verify(data,size-2)) == true)
			{
				uint32_t datatmp[10] = {0,0,0,0,0,0,0,0,0,0};
				datatmp[0] = ((uint32_t)(data[9]-0x30)*(uint32_t)(1000000000));
				datatmp[1] = ((uint32_t)(data[10]-0x30)*(uint32_t)(100000000));
				datatmp[2] = ((uint32_t)(data[11]-0x30)*(uint32_t)(10000000));
				datatmp[3] = ((uint32_t)(data[12]-0x30)*(uint32_t)(1000000));
				datatmp[4] = ((uint32_t)(data[13]-0x30)*(uint32_t)(100000));
				datatmp[5] = ((uint32_t)(data[14]-0x30)*(uint32_t)(10000));
				datatmp[6] = ((uint32_t)(data[15]-0x30)*(uint32_t)(1000));
				datatmp[7] = ((uint32_t)(data[16]-0x30)*(uint32_t)(100));
				datatmp[8] = ((uint32_t)(data[17]-0x30)*(uint32_t)(10));
				datatmp[9] = ((uint32_t)(data[18]-0x30)*(uint32_t)(1));
				timestamprf = datatmp[0]+datatmp[1]+datatmp[2]+datatmp[3]+datatmp[4]+datatmp[5]+datatmp[6]+datatmp[7]+datatmp[8]+datatmp[9];
				time_to_rf(timestamprf);
			}
			else
			{
				error_to_rf(time_crc_error);
			}
		}
	}
	else if (memcmp(data, "[statok]", max_rf_command_length) == 0)
	{
		SYS_TimerStop(&error_timer_status);
	}
	else if (memcmp(data, "[clrrom]", max_rf_command_length) == 0)
	{
		clr_rom_to_rf();
	}
	else if (memcmp(data, "[reboot]", max_rf_command_length) == 0)
	{
		reboot_to_rf();
	}
	else if (memcmp(data, "[infoid]", max_rf_command_length) == 0)
	{
		info_to_rf();
	}
	else if (memcmp(data, "[chngid]", max_rf_command_length) == 0)
	{
		if (size<20)	//correct size is 18
		{
			error_to_rf(read_eeprom_error);
		}
		else
		{
			if ((crc_verify(data,size-2)) == true)
			{
				uint8_t datatmp[8] = {0,0,0,0,0,0,0,0};
				datatmp[0] = data[9];
				datatmp[1] = data[10];
				datatmp[2] = data[11];
				datatmp[3] = data[12];
				datatmp[4] = data[13];
				datatmp[5] = data[14];
				datatmp[6] = data[15];
				datatmp[7] = data[16];
				change_id_to_rf(datatmp);
			}
			else
			{
				error_to_rf(read_eeprom_error);
			}	
		
		}
	}
	else if (memcmp(data, "[batlvl]", max_rf_command_length) == 0)
	{
		if (size<14)
		{
			error_to_rf(invalid_command_error);
		}
		else
		{
			if((crc_verify(data,size-2)) == true)
			{
				uint8_t datatmp[2] = {0,0};
				datatmp[0] = data[9];
				datatmp[1] = data[10];
				battery_threshold_to_rf(datatmp);
			}
			else
			{
				error_to_rf(read_eeprom_error);
			}
		}
	}
	else if (memcmp(data, "[wpsmod]", max_rf_command_length) == 0) //entering WPS mode via rf is one way road. Once entered the only way to exit it is need to complete the pairing or manually hold the wps button
	{
		SYS_TimerStop(&appWPSActivationTimer);
		operation_seq = wps_state;
		reboot_countdown = 0;
		APP_IbLoadSettings_WPS(); //load temporary setting
		SYS_TimerStart(&appWPSRequestTimer); //start requesting data
	}
	else if (memcmp(data, "[lst2lg]", max_rf_command_length) == 0)
	{
		last_1_log(' ');
		last_2_log(' ');
	}
	/*---------This is experimental function----------------*/
	else if (memcmp(data, "[dumplg]", max_rf_command_length) == 0)
	{
		dump_log_to_rf();
	}
	//---------End of Experimental function-----------------*/
	/*---------This is experimental function----------------*/
	else if (memcmp(data, "[stress]", max_rf_command_length) == 0)
	{
		stress_test();
	}
	//---------End of Experimental function-----------------*/
	else if (memcmp(data, "[wpsok!]", max_rf_command_length) == 0) //if receiving acknowledge from gateway
	{
		if (size < 28)
		{
			error_to_rf(wps_invalid_data_size);
		}
		else
		{
			if ((crc_verify(data,size-2)) == true)
			{
				for(uint8_t i=0; i<max_UID_length; i++)
				{
					if(data[i+9] != UID[i])
					{
						error_to_rf(wps_invalid_UID);
						return false;
					}
				}
				uint8_t datatmp[9] = {0,0,0,0,0,0,0,0,0};
				datatmp[0] = data[27];
				datatmp[1] = data[28];
				datatmp[2] = data[29];
				datatmp[3] = data[30];
				datatmp[4] = data[31];
				datatmp[5] = data[32];
				WPS_pairing(datatmp);
				WPS_STATUS = WPS_acknowledged;
				pne_delayms(1000);
				
				//add flag indicator that the gateway has acknowledge
			}
			else
			{
				error_to_rf(wps_crc_error);
			}
		}
	}
	else if (memcmp(data, "[wpsend]", max_rf_command_length) == 0) //if receiving acknowledge from gateway
	{
		if (size < 8)
		{
			error_to_rf(wps_invalid_data_size);
		}
		else
		{
			for(uint8_t i=0; i<max_UID_length; i++)
			{
				if(data[i+9] != UID[i])
				{
					error_to_rf(wps_invalid_UID);
					return false;
				}
			}
			reboot_to_rf();
			//add flag indicator that the gateway has acknowledge
		}
	}
	else
	{
		error_to_rf(invalid_command_error);
	}

	(void)size;
	return true;
}


//Manual button handler
void manual_btn_handler()
{
	//if ((PNEWELS_Buffer.soft_manual == 0) || (PNEWELS_Buffer.BUTTON_MANUAL == BUTTON_MANUAL_DEPRESSED))
	//{
		//gpio_clr(CHARGING_DISABLE);
	//}
	if(PNEWELS_Buffer.soft_discharge == 0)
	{
  		if ((PNEWELS_Buffer.soft_manual == 1) || (PNEWELS_Buffer.BUTTON_MANUAL == BUTTON_MANUAL_PRESSED))
  		{
    		gpio_set(CHARGING_DISABLE);
  		}
  		else
  		{
    		gpio_clr(CHARGING_DISABLE);
  		}
	}
}

void signed_to_ascii(int8_t data)
{
	uint8_t cache = 0;
	
	if(data & (1<<7))
	{
		cache = (~data) + 1;
		send_usart_char("-");
		int_to_ascii(cache);
		send_usart_char("\r\n");
	}
	else
	{
		int_to_ascii((uint8_t)data);
		send_usart_char("\r\n");
	}
}

// void scanChannelEnergy(void)
// {
// 	int8_t ed;
// 	
// 	ed = PHY_EdReq();
// 	signed_to_ascii(ed);
// }

/*************************************************************************//**
*****************************************************************************/

void getUID()
{
	uint8_t serial_addr[2] = {0x00, 0x08};
		
	pne_APP_TwiInit(0xBF);
	pne_APP_TwiReceive(serial_addr, 2, 0xBF, UID, 16);
}