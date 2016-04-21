/*
 * PNEWELS.h
 *
 * Created: 2/24/2015 5:50:53 PM
 *  Author: jason
 */ 


#ifndef PNEWELS_H_
#define PNEWELS_H_



// Includes
#include <stdbool.h>
#include "gpio.h"
#include "sysTimer.h"
#include "appPNEWELSTime2.h"


/*Mode*/
#define MODE_Emergency 		    1
#define MODE_WPS 			    2
#define MODE_DISCHARGING		3
#define MODE_CHARGING			4
#define MODE_STANDBY 		    5
#define MODE_FACTORY 		    6

/*System Status Definition*/
//#define V_BATT_TRESHOLD			0x40
#define V_BATT_SCHMITT_TRIGGER	17
//#define V_BATT_CHARGED			0x97

#define STATUS_VCHARGE_OK		0
#define STATUS_VCHARGE_NOK		1

#define STATUS_BATT_OK			0
#define STATUS_BATT_NOK			1
#define STATUS_BATT_NEU         2

#define STATUS_POWER_DOWN		0
#define STATUS_POWER_UP			1

#define BUTTON_MANUAL_DEPRESSED	0
#define BUTTON_MANUAL_PRESSED	1

#define BUTTON_WPS_DEPRESSED	0
#define BUTTON_WPS_PRESSED		1

#define TEMP_ALERT          	0
#define TEMP_NALERT     		1

#define STATUS_FACTORY_OK		0
#define STATUS_FACTORY_NOK		1

#define STATUS_AC_OK			0
#define STATUS_AC_NOK			1

#define BUTTON_CH1_DEPRESSED	0
#define BUTTON_CH1_PRESSED		1

#define BUTTON_CH2_DEPRESSED	0
#define BUTTON_CH2_PRESSED		1

#define BUTTON_DRV0_DEPRESSED	0
#define BUTTON_DRV0_PRESSED		1

#define BUTTON_DRV1_DEPRESSED	0
#define BUTTON_DRV1_PRESSED		1

#define BUTTON_DRV2_DEPRESSED	0
#define BUTTON_DRV2_PRESSED		1

#define SMA_ANTENNA ((1 << ANT_EXT_SW_EN) | (0 << ANT_CTRL1) | (1 << ANT_CTRL0))
#define CHIP_ANTENNA ((1 << ANT_EXT_SW_EN) |(1 << ANT_CTRL1) | (0 << ANT_CTRL0))


/*Types*/
typedef struct
{
	//-------------------------------Input Flag------------------------------------------
	uint8_t	STATUS_VCHARGE;		// AC Status			: 0-AC OK; 1-AC Fail
	uint8_t V_BATT;			// Battery Voltage		: 0-20V
	uint8_t V_LED;			// LED Voltage			: TBD
	
	uint8_t BUTTON_MANUAL;	// Manual Test Button	: 0-depress; 1-press
	uint8_t BUTTON_WPS;		// WPS Button			: 0-depress; 1-press
	uint8_t TEMP_ALARM;		// temperature alert	: 0-no alert; 1-alert
	uint8_t temperature;	// temperature			: 0~255 or TBD
	uint8_t IsFactory;
	
	uint8_t soft_manual;
	uint8_t soft_charge;
	uint8_t soft_discharge;
	
	//-------------------------------Output Flag------------------------------------------
	uint8_t IsOutputEmergencyLED;
	uint8_t IsOutputStatusLED1;
	uint8_t IsOutputStatusLED2;
	uint8_t IsOutputDisableCharging;
	uint8_t PWM_LED;		// LED PWM				: 0-255 or TBD
	uint8_t IsBatt;
	uint8_t IsPowerOK;
	
	//-------------------------------System Flag------------------------------------------
	uint8_t mode;
	uint8_t mode_Next;      // operation mode
	uint8_t BatteryInUse;
	uint8_t BatteryCapacity;
	uint8_t groupID;
	uint8_t zoneID;
	uint8_t nodeID;
	uint16_t deviceChannel;
	uint16_t deviceAddress;
	uint16_t devicePAN;
	
	//-------------------------------Extra add in------------------------------------------
	uint8_t STATUS_AC;
	uint8_t led_ch1;
	uint8_t led_ch2;
	uint8_t led_drv0;
	uint8_t led_drv1;
	uint8_t led_drv2;
	
	uint8_t oled0;
	uint8_t oled1;
	uint8_t oled2;
}PNEWELSBuffer_t;

typedef enum
{
	on_emergency_light,
	off_emergency_light,
	charging_test,
	discharging_test,
	wps_state,
	three_sec_handling_state,
	/*three_sec_handling_state_2*/
}operation_sequence_t;


/*Inputs*/
gpio_t MANUAL_BTN;
gpio_t WPS_BTN;
gpio_t TEMP_ALRT;
gpio_t VCHARGE_STAT;
gpio_t FACTORY_BTN;
gpio_t AC_STAT; //this pin temporary used as AC power detection
gpio_t INPUT_LED_CH1;
gpio_t INPUT_LED_CH2;
gpio_t INPUT_LED_DRV0;
gpio_t INPUT_LED_DRV1;
gpio_t INPUT_LED_DRV2;

/*Outputs*/
gpio_t STATUS_LED_1;
gpio_t STATUS_LED_2;
gpio_t CHARGING_DISABLE;
gpio_t EMER_LIGHT_EN;
gpio_t OUT_LED_CH0;
gpio_t OUT_LED_CH1;
gpio_t OUT_LED_CH2;


/*Variables*/
PNEWELSBuffer_t PNEWELS_Buffer;
operation_sequence_t operation_seq;
volatile uint8_t initialize_flag;
uint8_t installation_flag;
uint8_t data_update_flag;
uint8_t status_report_flag;
uint8_t interrupt_flag;
uint8_t debouncing_delay;

uint8_t LED_flag;
uint8_t wps_flag;
uint8_t do_once;
/*uint8_t //do_once_stop_wps_timer;*/
uint8_t battery_low_once;
uint16_t reboot_countdown;
volatile uint8_t rf_flag;
uint16_t countdown;

SYS_Timer_t appLEDTimer;
SYS_Timer_t appLEDIntervalTimer;
SYS_Timer_t error_timer_status;
SYS_Timer_t error_timer_wps;
SYS_Timer_t appWPSRequestTimer;
SYS_Timer_t appWPSActivationTimer;
uint8_t error_count_status;
uint8_t error_count_wps;
uint8_t wps_send_flag;
uint8_t WPS_STATUS;

/*- Device Flag Definitions ----------------------------------------------------*/
volatile uint8_t device_flag;
volatile uint8_t batt_low;
volatile uint8_t batt_max;

typedef enum
{
	FACTORY_FLAG,
	AC_FLAG
}DEVICE_FLAG;

typedef enum
{
	WPS_init,
	WPS_acknowledged,
	WPS_handshaked,
	WPS_done,
}WPS_FLAG;

/*Prototypes*/
void PNEWELSGpioInit(void);
void PNEWELSAppInit(void);
void PNEWELSTaskHandler(void);
void pneGetStatus();
void pneOperationMode();
void pneOutgoingData();
bool pneIncomingData(uint8_t *data, uint8_t size);
void timer2_isr(void);
void factory_mode(void);
void manual_btn_handler();
void signed_to_ascii(int8_t data);
/*void scanChannelEnergy(void);*/
void getUID();



#endif /* PNEWELS_H_ */