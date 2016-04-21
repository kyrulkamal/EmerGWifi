/*
 * pneConfig.h
 *
 * Created: 2/26/2015 4:40:36 PM
 *  Author: tay
 */ 


#ifndef PNECONFIG_H_
#define PNECONFIG_H_



#include "twi_megarf.h"
#include "adc_megarf.h"
#include "usart_megarf.h"


#define max_UID_length	16

/*- RF Parameter --------------------------------------------------------------*/
#define max_rf_command_length 8
#define max_rf_retries 0



/*- Factory Mode --------------------------------------------------------------*/
#define version        "Build 160321.1"
#define version_date   "NULL"
#define max_command_length 13



/*- TWI Definitions ------------------------------------------------------------*/
//!< TWI slave memeory address length
#define SLAVE_MEM_ADDR_LENGTH         TWI_SLAVE_ONE_BYTE_SIZE
//!< TWI data transfer rate
#define TWI_SPEED_HZ                  125000



/*- ADC Definitions ------------------------------------------------------------*/
#define ADC_BATTERY_INPUT           ADC_MUX_ADC0
#define ADC_EMER_LIGHT_INPUT        ADC_MUX_ADC1
#define ADC_VREF                    ADC_VREF_AVCC



/*- TWI Definitions ------------------------------------------------------------*/
#define USART_SERIAL_EXAMPLE            &USARTA1
#define USART_SERIAL_EXAMPLE_BAUDRATE   9600
#define USART_SERIAL_CHAR_LENGTH        USART_CHSIZE_8BIT_gc
#define USART_SERIAL_PARITY             USART_PMODE_DISABLED_gc
#define USART_SERIAL_STOP_BIT           false

/*- LED Timer Definition (in ms)-------------------------------------------------------*/
#define STANDBY_INTERVAL_TIMER			1000
#define STANDBY_TIMER					500

#define EMERGENCY_INTERVAL_TIMER		500
#define EMERGENCY_TIMER					250		

#define WPS_INTERVAL_TIMER				250
#define WPS_TIMER						125

#define WPS_REQUEST_TIMER				5000
#define WPS_ACTIVATION_TIMER			3000
/*-WPS Mode address, channel and PAN ID---------------------------------------------------------*/
#define WPS_ADDRESS						0xffff
#define WPS_CHANNEL						0x000f
#define WPS_PANID						0xffff

/*-PNE propriety signature key---------------------------------------------------------*/
#define SIGNATURE_KEY		"abc123"

// /*- Battery configuration definition----------------------------------------*/
// #define BAT_MAX_LEVEL		0x97

#endif /* PNECONFIG_H_ */