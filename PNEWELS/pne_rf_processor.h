/*
 * pne_rf_processor.h
 *
 * Created: 5/26/2015 8:58:27 AM
 *  Author: tay
 */ 


#ifndef PNE_RF_PROCESSOR_H_
#define PNE_RF_PROCESSOR_H_


//Includes
#include "sysTypes.h"
#include "pne_appTwi.h"
#include <stdbool.h>


//fatal error struct
typedef enum 
{
	adc_fatal_error,
	twi_fatal_error,
	usart_fatal_error,
	timer2_fatal_error,
}fatal_error_t;

typedef enum
{
	temperature_alert,
	battery_dead_alert,
	battery_wrong_alert,
	conflicted_command,
	read_eeprom_error,
	hardware_error,
	time_invalid_data_size,
	time_crc_error,
	wps_invalid_data_size,
	wps_invalid_UID,
	wps_crc_error,
	invalid_command_error,

	
}error_reporting;

//Prototypes
void status_send_to_rf();
void emergency_on_to_rf();
void emergency_off_to_rf();
void eeprom_to_rf(uint16_t address);
void error_to_rf(uint8_t alert_type);
void pend_to_rf(char pend_type);
void charge_to_rf();
void time_to_rf(uint32_t time);
void wps_send_to_rf();
void clr_rom_to_rf();
void send_to_rf(uint8_t *data, uint8_t data_length, char* command);
bool crc_verify(uint8_t *data, uint8_t size);
uint8_t ascii_to_hex(uint8_t data);
void fatal_error_to_rf(fatal_error_t error);
void reboot_to_rf();
void info_to_rf();
void change_id_to_rf(uint8_t *data);
void last_1_log(char type);
void last_2_log(char type);
void test_completed(char type, uint8_t retry);
void dump_log_to_rf();
void stress_test(void);
void request_to_RF(uint8_t *RFU_UID);
void WPS_pairing(uint8_t *data);
void WPS_OK_Alert(uint8_t *data, uint8_t size);

#endif /* PNE_RF_PROCESSOR_H_ */