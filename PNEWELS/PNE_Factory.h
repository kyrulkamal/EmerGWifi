/*
 * PNE_Factory.h
 *
 * Created: 5/25/2015 10:33:32 AM
 *  Author: tay
 *
 *  Modified by zulhilmi
 */ 


#ifndef PNE_FACTORY_H_
#define PNE_FACTORY_H_



//----------- Headers ----------//
#include "sysTypes.h"
#include "pneConfig.h"

//-------Char definition-------//
static const char groupID_display[] = "Group ID";
static const char zoneID_display[] = "Zone ID";
static const char nodeID_display[] = "Node ID";
static const char deviceADDR_display[] = "Device address";
static const char deviceCH_display[] = "Device channel";
static const char devicePAN_display[] = "Device PAN ID";
static const char batteryLOW_display[] = "Battery low threshold";
static const char batteryMAX_display[] = "Battery max threshold";

//----------- Types -----------//
typedef enum Usart_State_t {
	Usart_start,
	Usart_receive
} Usart_State_t;

typedef enum
{
	factory_startup_message,
	factory_pending_message,
	factory_process_message,
	factory_button_instruction
}factory_state_t;

typedef enum
{
	groupID_command,
	zoneID_command,
	nodeID_command,
	battery_command,
	battcap_command,
	device_addr_command,
	device_panID_command,
	display_info_command
}factory_command_t;

typedef enum
{
	button1,
	button2,
	button3,
	button4,
	button5,
	button6,
	button7,
	button8,
	button9,
	button10
}button_state_t;

typedef enum
{
	config_startup_message,
/*	config_GroupID,*/
// 	config_ZoneID,
// 	config_NodeID,
	config_DeviceADDR,
	config_DeviceCH,
	config_DevicePAN,
	config_exit,
	config_idle
}config_state_t;

typedef enum
{
	groupID_config,
	zoneID_config,
	nodeID_config,
	device_addr_config,
	device_panID_config
}config_command_t;

typedef enum
{
	idle,
	config,
	display_info,
	test_mode,
	reset_flag,
	dump_memory,
	dump_log,
	wipe_memory,
	battery_set,
	option_message
}option_select_t;

typedef enum
{
	reset_startup_message,
	reset_all,
	reset_bit0,
	reset_bit1,
	reset_read_status,
	reset_idle,
	reset_exit
}reset_option_t;

typedef enum
{
	battery_startup_message,
	batteryLow,
	batteryMax,
	batteryStatus,
	battery_idle,
	battery_exit
}battery_option_t;

//----------- Global Variables -----------//
uint8_t address_low;
uint8_t address_high;
uint8_t config_waiting_input;
uint8_t configuration_flag;
uint8_t reset_option_flag;
uint8_t reset_cache;
uint8_t battery_cache;
uint8_t battery_option_flag;
volatile uint8_t flag;
uint8_t count_usart;
uint8_t commamd[max_command_length];
Usart_State_t usart_state;
factory_state_t factory_state;
factory_command_t factory_command;
button_state_t button;
config_state_t config_state;
reset_option_t reset_state;
config_command_t config_command;
option_select_t option_select;
battery_option_t battery_state;
uint8_t battery_option_flag;

//----------- Prototypes -----------//
void factory_mode(void);
void pneModeFactory();
void pnewels_get_usart(uint8_t tmp);
void send_usart_char(char *tx_buf);
void send_usart_const_char(const char *tx_buf);
void send_usart(uint8_t *tx_buf);
void int_to_ascii(uint8_t number);
void pneConfiguration(void);
uint8_t optionSelect(void);
void pneDisplayInfo(void);
uint8_t configSelect(void);
void config_read_input(uint16_t address, char data_length, const char *name);
void send_operation_info(const char *name, uint8_t data);
void testMode();
void resetFlag();
uint8_t resetSelect();
void dump_eeprom();
uint8_t readE2prom(uint16_t address);
void dump_log_file();
void erase_memory();
void writeE2prom(uint16_t address, uint8_t data);
void battery_config();
uint8_t batterySelect();


#endif /* PNE_FACTORY_H_ */