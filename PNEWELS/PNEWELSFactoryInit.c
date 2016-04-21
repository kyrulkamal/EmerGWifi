/*
 * PNEWELSFactoryInit.c
 *
 * Created: 10/30/2015 14:28:40
 *  Author: Zulhilmi
 */ 
#include "PNE_Factory.h"
#include "pneConfig.h"
#include "sysTypes.h"
#include "PNEWELS.h"
#include "adc.h"
#include "pne_appTwi.h"
#include <string.h>
#include "pne_appDelay.h"
#include "pne_rf_processor.h"
#include "PNEWELSFactoryInit.h"
#include "memoryConfig.h"
#include "appIb.h"

void factory_initialization()
{
	device_flag = readE2prom(FLAG); //read flag form EEPROM
	batt_low = readE2prom(BATTERY_LOW);
	batt_max = readE2prom(BATTERY_MAX);
	if(device_flag & (1<<FACTORY_FLAG))
	{
		write_factory_default(DEVICE_ADDRESS_L,ADDRESS_DEFAULT_L);
		write_factory_default(DEVICE_ADDRESS_H,ADDRESS_DEFAULT_H); //set device address default value (MSB)
		
		write_factory_default(DEVICE_CHANNEL_L,CHANNEL_DEFAULT_L); //set device channel default value (LSB)
		write_factory_default(DEVICE_CHANNEL_H,CHANNEL_DEFAULT_H); //set device channel default value (MSB)
		
		write_factory_default(DEVICE_PAN_L,PAN_DEFAULT_L); //set device PAN default value (LSB)
		write_factory_default(DEVICE_PAN_H,PAN_DEFAULT_H); //set device PAN default value (MSB)
		
		write_factory_default(BATTERY_MAX,BATTERY_MAX_THRESH);
		write_factory_default(BATTERY_LOW,BATTERY_LOW_THRESH);
		
		clre2prom(); //wipe all log file
//		write_factory_default(GROUP_ID, GROUP_DEFAULT); //set Group ID default Value
//		write_factory_default(ZONE_ID, ZONE_DEFAULT); //set Zone ID default Value
//		write_factory_default(NODE_ID, NODE_DEFAULT); //set Node ID default value
		
		device_flag = (device_flag & ~(1<<FACTORY_FLAG));
		
		write_factory_default(FLAG, device_flag); //set Factory flag
		pne_delayms(10);
	}
	//PNEWELSE2promWrite(0xAE, 'E', 01, 41, 0xFF); //reset flag
}

void systemFlag_check()
{
	if(device_flag & (1<<AC_FLAG))
	{
		operation_seq = wps_state;
		reboot_countdown = 0;
		APP_IbLoadSettings_WPS(); //load temporary setting
		SYS_TimerStart(&appWPSRequestTimer); //start requesting data
	}
	else operation_seq = off_emergency_light;
	
	//device_flag = (device_flag & ~(1<<AC_FLAG)); //write the flag
	//writeE2prom(FLAG,device_flag);
}

void write_factory_default(uint16_t address, uint8_t factory_default)
{
	uint8_t memory_address_low = (address & 0xFF);
	uint8_t memory_address_high = ((address >> 8) & 0xFF);
	
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, factory_default); //set device address default value (LSB)
	pne_delayms(10);
	
}