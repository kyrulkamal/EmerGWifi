/*
 * pne_rf_processor.c
 *
 * Created: 5/26/2015 8:57:56 AM
 *  Author: tay
 *
 *  Modified by zulhilmi
 */ 


#include "pne_rf_processor.h"
#include "PNEWELS.h"
#include "appNwk.h"
#include "pneConfig.h"
#include "appPNEWELSTime2.h"
#include "crc.h"
#include <stddef.h>
#include <string.h>
#include "pne_appTwi.h"
#include "pne_appDelay.h"
#include <stdlib.h>
#include "PNE_Factory.h"
#include "memoryConfig.h"
#include "PNEWELSFactoryInit.h"
#include "pnesign.h"


//Variables
uint8_t data_pack_1 = 0;
uint8_t data_pack_2 = 0;
uint8_t data_pack_3 = 0;
uint8_t data_to_rf[100] = {0};

/*delete this later. For testing purpose only------------------*/
//uint8_t data_test[100] = {0x14,0x05,0x00,0x1a,0xb3,0x04,0x00,0x00,0x00,0x1e};


//Functions
void packet_data()
{	
	data_pack_1 = ((PNEWELS_Buffer.BUTTON_WPS & 0x01) | ((PNEWELS_Buffer.BUTTON_MANUAL & 0x01) << 1) | ((PNEWELS_Buffer.IsFactory & 0x01) << 2) | ((PNEWELS_Buffer.STATUS_VCHARGE & 0x01) << 3) | ((PNEWELS_Buffer.TEMP_ALARM & 0x01) << 4) | ((PNEWELS_Buffer.IsBatt & 0x01) << 5) | ((PNEWELS_Buffer.soft_manual & 0x01) << 6) | ((PNEWELS_Buffer.STATUS_AC & 0x01) << 7));
	data_pack_2 = ((PNEWELS_Buffer.mode & 0x07) | ((PNEWELS_Buffer.IsOutputEmergencyLED & 0x01) << 3) | ((PNEWELS_Buffer.IsOutputDisableCharging & 0x01) << 4) | ((PNEWELS_Buffer.oled0 & 0x01) << 5) | ((PNEWELS_Buffer.oled1 & 0x01) << 6) | ((PNEWELS_Buffer.oled2 & 0x01) << 7));
	data_pack_3 = ((PNEWELS_Buffer.led_ch1 & 0x01) | ((PNEWELS_Buffer.led_ch2 & 0x01) << 1) | ((PNEWELS_Buffer.led_drv0 & 0x01) << 2) | ((PNEWELS_Buffer.led_drv1 & 0x01) << 3) | ((PNEWELS_Buffer.led_drv2 & 0x01) << 4) | ((PNEWELS_Buffer.soft_charge & 0x01) << 5) | ((PNEWELS_Buffer.soft_discharge & 0x01) << 6));
}


//devices send to rf
void status_send_to_rf()
{
	packet_data();
	data_to_rf[0] = data_pack_1;
	data_to_rf[1] = data_pack_2;
	data_to_rf[2] = data_pack_3;
	data_to_rf[3] = PNEWELS_Buffer.temperature;
	data_to_rf[4] = PNEWELS_Buffer.V_BATT;
	data_to_rf[5] = PNEWELS_Buffer.V_LED;
	data_to_rf[6] = ((get_timestamp() & 0xFF000000) >> 24);
	data_to_rf[7] = ((get_timestamp() & 0x00FF0000) >> 16);
	data_to_rf[8] = ((get_timestamp() & 0x0000FF00) >> 8);
	data_to_rf[9] = (get_timestamp() & 0x000000FF);
	send_to_rf(data_to_rf, 10, "[status]");
}

void wps_send_to_rf()
{
	send_to_rf(data_to_rf, 0, "[wpsmod]");
}


//devices respond to rf
void emergency_on_to_rf()
{
	PNEWELS_Buffer.soft_manual = 1;
	send_to_rf(data_to_rf, 0, "[emergy]");
}

void emergency_off_to_rf()
{
	PNEWELS_Buffer.soft_manual = 0;
	send_to_rf(data_to_rf, 0, "[emergy]");
}

void eeprom_to_rf(uint16_t address)
{
	uint8_t memory_address_low = 0;
	uint8_t memory_address_high = 0;
	for (uint8_t i=0; i<11; i++)
	{
		memory_address_high = ((address & 0xFF00) >> 8);
		memory_address_low = (address & 0x00FF);
		data_to_rf[i] = PNEWELSE2promRead(0xAF,'E',memory_address_low,memory_address_high);
		address++;
		pne_delayms(10);
	}
	send_to_rf(data_to_rf, 11, "[rd rom]");
}

void eeprom_byte_to_rf(uint16_t address, uint8_t size)
{
	uint8_t memory_address_low = 0;
	uint8_t memory_address_high = 0;
	for (uint8_t i=0; i<size; i++)
	{
		memory_address_high = ((address & 0xFF00) >> 8);
		memory_address_low = (address & 0x00FF);
		data_to_rf[i] = PNEWELSE2promRead(0xAF,'E',memory_address_low,memory_address_high);
		address++;
		pne_delayms(10);
	}
	send_to_rf(data_to_rf, size, "[rddata]");
}

void error_to_rf(uint8_t alert_type)
{
	switch(alert_type)
	{
		case temperature_alert:
			data_to_rf[0] = temperature_alert;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
			
		case battery_dead_alert:
			data_to_rf[0] = battery_dead_alert;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
			
		case conflicted_command:
			data_to_rf[0] = conflicted_command;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
		
		case battery_wrong_alert:
			data_to_rf[0] = battery_wrong_alert;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
			
		case read_eeprom_error:
			data_to_rf[0] = read_eeprom_error;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
			
		case hardware_error:
			data_to_rf[0] = hardware_error;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
			
		case time_invalid_data_size:
			data_to_rf[0] = time_invalid_data_size;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
		
		case time_crc_error:
			data_to_rf[0] = time_crc_error;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
		
		case wps_invalid_data_size:
			data_to_rf[0] = wps_invalid_data_size;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
			
		case invalid_command_error:
			data_to_rf[0] = invalid_command_error;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
			
		case wps_invalid_UID:
			data_to_rf[0] = wps_invalid_UID;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
			
		case wps_crc_error:
			data_to_rf[0] = wps_crc_error;
			send_to_rf(data_to_rf,1,"[error!]");
			break;
		
		default:
			break;
	}
}

void pend_to_rf(char pend_type)
{
	switch(pend_type)
	{
		case 'c':
			send_to_rf(data_to_rf, 0, "[chrgng]");
		break;
		case 'd':
			send_to_rf(data_to_rf, 0, "[dschrg]");
		break;
		default:
		break;
	}
	/*send_to_rf(data_to_rf, 0, "[pend..]");*/
}

void charge_to_rf()
{
	uint16_t address = memory_address_incr - 11;
	eeprom_to_rf(address);
	address = memory_address_incr - 22;
	eeprom_to_rf(address);
}

void time_to_rf(uint32_t time)
{
	set_timestamp(time);
	send_to_rf(data_to_rf, 0, "[timech]");
}

void clr_rom_to_rf()
{
	clre2prom();
	send_to_rf(data_to_rf, 0, "[clrrom]");
}

void reboot_to_rf()
{
	//send_to_rf(data_to_rf, 0, "[reboot]");
	while(1)
	{
		//after a while this will reset the system when watchdog enabled
	}
}

void info_to_rf()
{
/*	data_to_rf[0] = readE2prom(GROUP_ID); //group*/
// 	data_to_rf[1] = readE2prom(ZONE_ID); //zone
// 	data_to_rf[2] = readE2prom(NODE_ID); //node
	data_to_rf[0] = readE2prom(DEVICE_ADDRESS_H); //address MSB
	data_to_rf[1] = readE2prom(DEVICE_ADDRESS_L); //address LSB
	data_to_rf[2] = readE2prom(DEVICE_CHANNEL_H); //channel MSB
	data_to_rf[3] = readE2prom(DEVICE_CHANNEL_L); //channel LSB
	data_to_rf[4] = readE2prom(DEVICE_PAN_H); //pan id MSB
	data_to_rf[5] = readE2prom(DEVICE_PAN_L); //pan id LSB
	send_to_rf(data_to_rf, 6, "[infoid]");
}

void change_id_to_rf(uint8_t *data)
{
/*	write_factory_default(GROUP_ID,data[0]);*/
// 	write_factory_default(ZONE_ID,data[1]);
// 	write_factory_default(NODE_ID,data[2]);
	write_factory_default(DEVICE_ADDRESS_H,data[0]);
	write_factory_default(DEVICE_ADDRESS_L,data[1]);
	write_factory_default(DEVICE_CHANNEL_H,data[2]);
	write_factory_default(DEVICE_CHANNEL_L,data[3]);
	write_factory_default(DEVICE_PAN_H,data[4]);
	write_factory_default(DEVICE_PAN_L,data[5]);
	write_factory_default(BATTERY_LOW,data[6]);
	write_factory_default(BATTERY_MAX,data[7]);
	send_to_rf(data_to_rf, 0, "[chngid]");
}

void write_to_rf(uint8_t *data, uint16_t address, uint8_t size)
{
	for(uint8_t i = 0; i < size; i++){
		write_factory_default(address++, data[i]);
	}
	send_to_rf(data_to_rf, 0, "[wr ok!]");
}

void battery_threshold_to_rf(uint8_t *data)
{
	write_factory_default(BATTERY_LOW,data[0]);
	write_factory_default(BATTERY_MAX,data[1]);
	send_to_rf(data_to_rf, 0, "[batlvl]");
}

void last_1_log(char type)
{
  uint8_t memory_address_low = 0;
  uint8_t memory_address_high = 0;
  for (uint8_t i=0; i<11; i++)
  {
    memory_address_high = (((memory_address_incr-22+i) & 0xFF00) >> 8);
    memory_address_low = ((memory_address_incr-22+i) & 0x00FF);
    data_to_rf[i] = PNEWELSE2promRead(0xAF,'E',memory_address_low,memory_address_high);
    /*pne_delayms(10);*/
  }
  switch(type)
  {
	  case 'n':
		send_to_rf(data_to_rf, 6, "[1stnml]"); //temporary reduce the sent data to 6 only in order to reduce bandwidth usage. Default value is 11
		break;
	  case 'a':
		send_to_rf(data_to_rf, 6, "[1staci]");
		break;
	  case 's':
		send_to_rf(data_to_rf, 6, "[1stsft]");
		break;
	  default:
		send_to_rf(data_to_rf, 11, "[1stlog]");
		break;
  }
}


void last_2_log(char type)
{
	uint8_t memory_address_low = 0;
	uint8_t memory_address_high = 0;
	for (uint8_t i=0; i<11; i++)
	{
		memory_address_high = (((memory_address_incr-11+i) & 0xFF00) >> 8);
		memory_address_low = ((memory_address_incr-11+i) & 0x00FF);
		data_to_rf[i] = PNEWELSE2promRead(0xAF,'E',memory_address_low,memory_address_high);
		/*pne_delayms(10);*/
	}
	switch(type)
	{
		case 'n':
			send_to_rf(data_to_rf, 6, "[2ndnml]");
			break;
		case 'a':
			send_to_rf(data_to_rf, 6, "[2ndaci]");
			break;
		case 's':
			send_to_rf(data_to_rf, 6, "[2ndsft]");
			break;
		default:
			send_to_rf(data_to_rf, 11, "[2ndlog]");
			break;
	}
}

void test_completed(char type, uint8_t retry)
{
	uint8_t memory_address_low = 0;
	uint8_t memory_address_high = 0;
	uint8_t data_counter = 0;
	uint8_t counter = 0;
	for (uint8_t i=0; i<6; i++) //For the start of the test. Only retrieve the time portion. Fork of last_2_log function
	{
		memory_address_high = (((memory_address_incr-22+i) & 0xFF00) >> 8);
		memory_address_low = ((memory_address_incr-22+i) & 0x00FF);
		data_to_rf[data_counter] = PNEWELSE2promRead(0xAF,'E',memory_address_low,memory_address_high);
		data_counter++;
	}
	for (uint8_t i=0; i<6; i++) //For the end of the test. Only retrieve the time portion
	{
		memory_address_high = (((memory_address_incr-11+i) & 0xFF00) >> 8);
		memory_address_low = ((memory_address_incr-11+i) & 0x00FF);
		data_to_rf[data_counter] = PNEWELSE2promRead(0xAF,'E',memory_address_low,memory_address_high);
		data_counter++;
	}
	while(counter < retry)
	{
		switch(type)
		{
			case 'n':
			send_to_rf(data_to_rf, data_counter, "[tstend]");
			break;
			case 'a':
			send_to_rf(data_to_rf, data_counter, "[tstaci]");
			break;
			case 's':
			send_to_rf(data_to_rf, data_counter, "[tststp]");
			break;
			default:
			send_to_rf(data_to_rf, data_counter, "[tstend]");
			break;
		}
		counter++;
	}
	
/*	send_to_rf(data_to_rf, data_counter + 1, "[tstend]");*/
// 	pne_delayms(10);
// 	send_to_rf(data_to_rf, data_counter + 1, "[tstend]");
}

/*---------This is experimental function----------------*/
void dump_log_to_rf()
{
	uint8_t i = 0;
	uint8_t memory_address_high = 0;
	uint8_t memory_address_low = 0;
	
	for (uint16_t tmpi=0; tmpi<4092; tmpi++)
	{
		if(i == 11)
		{
			send_to_rf(data_to_rf, 11, "[dumpmm]");
			i = 0;
			/*pne_delayms(10);*/
		}
		memory_address_low = (tmpi & 0xFF);
		memory_address_high = ((tmpi >> 8) & 0xFF);
		data_to_rf[i] = PNEWELSE2promRead(0xAF,'E',memory_address_low,memory_address_high);
		pne_delayms(10);
		i++;
		
	}
}
//---------End of Experimental function-----------------*/
/*---------This is experimental function----------------*/
void stress_test()
{
	for(uint8_t u = 0; u<20; u++)
	{
		for(uint16_t i = 0; i<31; i++)
		{
			data_to_rf[i] = 0;
		}
		data_to_rf[31] = u;
		send_to_rf(data_to_rf,32,"[stress]");
	}
}
//---------End of Experimental function-----------------*/
void request_to_RF(uint8_t *RFU_UID)
{
	for(uint8_t i = 0; i<max_UID_length; i++)
	{
		data_to_rf[i] = RFU_UID[i];
	}
	send_to_rf(data_to_rf, max_UID_length, "[requid]");
}

void sos_to_RF(uint8_t *RFU_UID)
{
	for(uint8_t i = 0; i<max_UID_length; i++)
	{
		data_to_rf[i] = RFU_UID[i];
	}
	send_to_rf(data_to_rf, max_UID_length, "[sosmsg]");
}

void WPS_pairing(uint8_t *data)
{
	write_factory_default(DEVICE_ADDRESS_H,data[0]);
	write_factory_default(DEVICE_ADDRESS_L,data[1]);
	write_factory_default(DEVICE_CHANNEL_H,data[2]);
	write_factory_default(DEVICE_CHANNEL_L,data[3]);
	write_factory_default(DEVICE_PAN_H,data[4]);
	write_factory_default(DEVICE_PAN_L,data[5]);
	/*send_to_rf(data_to_rf, 0, "[chngid]");*/
}

void WPS_OK_Alert(uint8_t *data, uint8_t size)
{
	send_to_rf(data,size,"[pairok]");
}

//send to rf function
void send_to_rf(uint8_t *data, uint8_t data_length, char* command)
{
	uint16_t sign = 0;
	/*const unsigned char* key = "abc123"; //added previously*/
	
	AppNwkBuffer_t *pnebufall;
	if (NULL == (pnebufall = APP_NwkGetBuffer()))
	return;
	pnebufall->size = 0;
	
	//pnebufall->data[pnebufall->size++] = PNEWELS_Buffer.groupID;
	
	memcpy(pnebufall->data, (uint8_t *)(command), max_rf_command_length);
	pnebufall->size = max_rf_command_length;
	//pnebufall->data[pnebufall->size++] = ((APP_ADDR & 0xFF00) >> 8);
	//pnebufall->data[pnebufall->size++] = APP_ADDR & 0x00FF;
	//pnebufall->data[pnebufall->size++] = PNEWELS_Buffer.groupID;
	for (uint8_t i=0; i<data_length; i++)
	{
		pnebufall->data[pnebufall->size++] = 0x5B;
		pnebufall->data[pnebufall->size++] = data[i];
		pnebufall->data[pnebufall->size++] = 0x5D;
	}
	crcInit();
	pnebufall->data[pnebufall->size++] = ((crcFast(pnebufall->data, (data_length+8+data_length*2)) & 0xFF00) >> 8);
	pnebufall->data[pnebufall->size++] = (crcFast(pnebufall->data, (data_length+8+data_length*2)) & 0x00FF);
	
	sign = pnesign(((unsigned char*) (pnebufall->data)), ((uint16_t) pnebufall->size),(unsigned char*) SIGNATURE_KEY, strlen(SIGNATURE_KEY)); //need some rework
	pnebufall->data[pnebufall->size++] = ((uint8_t)(sign >> 8));
	pnebufall->data[pnebufall->size++] = ((uint8_t)(sign & 0xff));
	
	APP_NwkSendBuffer(pnebufall);
}


//crc verification routine
bool crc_verify(uint8_t *data, uint8_t size)
{
	uint8_t crc_upper = 0;
	uint8_t crc_lower = 0;
	crc_upper = data[size-2]; //crc_upper = (ascii_to_hex(data[size-4])*0x10 + ascii_to_hex(data[size-3]));
	crc_lower = data[size-1]; //crc_lower = (ascii_to_hex(data[size-1])*0x10 + ascii_to_hex(data[size-1]));
	uint8_t crc_upper_internal = 0;
	uint8_t crc_lower_internal = 0;
	uint8_t data_length = size-2; //uint8_t data_length = size-4;
	crcInit();
	crc_upper_internal = ((crcFast(data, (data_length)) & 0xFF00) >> 8);
	crc_lower_internal = (crcFast(data, (data_length)) & 0x00FF);
	if ((crc_upper_internal == crc_upper) && (crc_lower_internal == crc_lower))
	{
		
		return true;
	}

	return false;	
}


//ascii to hex conversion routine
uint8_t ascii_to_hex(uint8_t data)
{
	uint8_t return_data = 0;
	if ((data<=0x39) && (data>=0x30))
	{
		return_data = data - 0x30;
	}
	if ((data<=0x5A) && (data>=0x41))
	{
		return_data = data - 0x37;
	}
	if ((data<=0x66) && (data>=0x61))
	{
		return_data = data - 0x57;
	}
	return return_data;
}


//fatal error routine
void fatal_error_to_rf(fatal_error_t error)
{
	if (error == adc_fatal_error)
	{
		data_to_rf[0] = adc_fatal_error+1;
		send_to_rf(data_to_rf,1,"[ferror]");
	}
	else if (error == twi_fatal_error)
	{
		data_to_rf[0] = twi_fatal_error+1;
		send_to_rf(data_to_rf,1,"[ferror]");
	}
	else if (error == usart_fatal_error)
	{
		data_to_rf[0] = usart_fatal_error+1;
		send_to_rf(data_to_rf,1,"[ferror]");
	}
	else
	{
		data_to_rf[0] = timer2_fatal_error+1;
		send_to_rf(data_to_rf,1,"[ferror]");
	}
}

