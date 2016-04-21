/*
 * pne_appTwi.c
 *
 * Created: 2/10/2015 1:53:16 PM
 *  Author: tay
 *  Editted : zulhilmi
 */ 


#include "twi_megarf.h"
#include "pneConfig.h"
#include "pne_appTwi.h"
#include "pne_appDelay.h"
#include "PNEWELS.h"
#include "adc.h"
#include "pne_rf_processor.h"
#include <avr/wdt.h>
#include "PNE_Factory.h"
#include "memoryConfig.h"


//Functions
void pne_APP_TwiInit(uint8_t slave_bus_address)
{	
	//TWI Declaration
	twi_master_options_t opt =
	{
		.speed = TWI_SPEED_HZ,
		.chip  = slave_bus_address,
	};
	opt .baud_reg = TWI_CLOCK_RATE(F_CPU, opt.speed);
	twi_master_init(TWI_EXAMPLE,&opt);
}

AppPNEWELS_TWIStatus_t pne_APP_TwiSend(uint8_t *slave_mem_addr, uint8_t slave_mem_addr_length, uint8_t slave_bus_address, uint8_t *test_pattern, uint8_t test_pattern_length)
{
	uint8_t count = 0;
	AppPNEWELS_TWIStatus_t status = APP_PNEWELS_TWI_WRITE_SUCCESS;
	
	twi_package_t temperature_packet =
	{
		.addr[0]      = slave_mem_addr[0],               /* TWI slave memory address data */
		.addr_length  = slave_mem_addr_length,           /* TWI slave memory address data size */
		.chip         = slave_bus_address,               /* TWI slave bus address */
		.buffer       = (void *)test_pattern,            /* transfer data source buffer */
		.length       = test_pattern_length              /* transfer data size (bytes) */
	};
	
	if (slave_mem_addr_length == 1)
	{
		temperature_packet.addr[0] = slave_mem_addr[0];
	}
	else if (slave_mem_addr_length == 2)
	{
		temperature_packet.addr[0] = slave_mem_addr[1];
		temperature_packet.addr[1] = slave_mem_addr[0];
	}
	else if (slave_mem_addr_length == 3)
	{
		temperature_packet.addr[0] = slave_mem_addr[0];
		temperature_packet.addr[1] = slave_mem_addr[1];
		temperature_packet.addr[2] = slave_mem_addr[2];
	}
	
	while(twi_master_write(TWI_EXAMPLE,&temperature_packet) != TWI_SUCCESS)
	{
		pne_delayms(1);
		count++;
		if (count == 10)
		{
			count = 0;
			status = APP_PNEWELS_TWI_WRITE_FAIL;
			fatal_error_to_rf(twi_fatal_error);
			break;
		}
	}
	
	return status;
}

AppPNEWELS_TWIStatus_t pne_APP_TwiReceive(uint8_t *slave_mem_addr, uint8_t slave_mem_addr_length, uint8_t slave_bus_address, uint8_t *data_received, uint8_t data_received_length)
{
	uint8_t count = 0;
	AppPNEWELS_TWIStatus_t status = APP_PNEWELS_TWI_READ_SUCCESS;
	
	twi_package_t packet_received =
	{
		.addr[0]      = slave_mem_addr[0],               /* TWI slave memory address data */
		.addr_length  = slave_mem_addr_length,           /* TWI slave memory address data size */
		.chip         = slave_bus_address,               /* TWI slave bus address */
		.buffer       = data_received,                   /* transfer data source buffer */
		.length       = data_received_length             /* transfer data size (bytes) */
	};
	
	if (slave_mem_addr_length == 1)
	{
		packet_received.addr[0] = slave_mem_addr[0];
	}
	else if (slave_mem_addr_length == 2)
	{
		packet_received.addr[0] = slave_mem_addr[1];
		packet_received.addr[1] = slave_mem_addr[0];
	}
	else if (slave_mem_addr_length == 3)
	{
		packet_received.addr[0] = slave_mem_addr[0];
		packet_received.addr[1] = slave_mem_addr[1];
		packet_received.addr[2] = slave_mem_addr[2];
	}
	else
	{
		
	}
	
	while(twi_master_read(TWI_EXAMPLE,&packet_received) != TWI_SUCCESS)
	{
		pne_delayms(1);
		count++;
		if (count == 10)
		{
			count = 0;
			status = APP_PNEWELS_TWI_READ_FAIL;
			fatal_error_to_rf(twi_fatal_error);
			break;
		}
	}
	
	return status;
}

void store(store_seq_t seq)
{
	uint8_t memory_address_low = 0;
	uint8_t memory_address_high = 0;
	uint32_t current_time = 0;
	uint8_t time_1_byte = 0;
	uint8_t time_2_byte = 0;
	uint8_t time_3_byte = 0;
	uint8_t time_4_byte = 0;
	uint8_t status_byte1 = 0;
	uint8_t status_byte2 = 0;
	uint8_t status_byte3 = 0;

	if (seq == charge_start)
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x43);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x53);
		memory_address_incr++;
		pne_delayms(10);
	}
	else if (seq == charge_end)
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x43);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x45);
		memory_address_incr++;
		pne_delayms(10);
	}
	else if (seq == discharge_start)
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x44);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x53);
		memory_address_incr++;
		pne_delayms(10);
	}
	else if (seq == discharge_end)
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x44);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x45);
		memory_address_incr++;
		pne_delayms(10);
	}
	else if (seq == emergency_start)
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x45);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x53);
		memory_address_incr++;
		pne_delayms(10);
	}
	else if (seq == emergency_end)
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x45);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x45);
		memory_address_incr++;
		pne_delayms(10);
	}
	else if (seq == battery_charged)
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x42);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x43);
		memory_address_incr++;
		pne_delayms(10);
	}
	else if (seq == battery_discharged)
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x42);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x44);
		memory_address_incr++;
		pne_delayms(10);
	}
	else
	{
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x4E);
		memory_address_incr++;
		pne_delayms(10);
		if (memory_address_incr >= 4092)
		{
			memory_address_incr = 0;
		}
		memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
		memory_address_low = (memory_address_incr & 0x00FF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0x4E);
		memory_address_incr++;
		pne_delayms(10);
	}
	
	current_state_emergency_light = PNEWELS_Buffer.IsOutputEmergencyLED;
	current_time = get_timestamp();
	time_4_byte = ((current_time & 0xFF000000) >> 24);
	time_3_byte = ((current_time & 0x00FF0000) >> 16);
	time_2_byte = ((current_time & 0x0000FF00) >> 8);
	time_1_byte = (current_time & 0x000000FF);
	status_byte1 = ((PNEWELS_Buffer.BUTTON_WPS & 0x01) | ((PNEWELS_Buffer.BUTTON_MANUAL & 0x01) << 1) | ((PNEWELS_Buffer.IsFactory & 0x01) << 2) | ((PNEWELS_Buffer.STATUS_VCHARGE & 0x01) << 3) | ((PNEWELS_Buffer.TEMP_ALARM & 0x01) << 4) | ((PNEWELS_Buffer.IsBatt & 0x01) << 5) | ((PNEWELS_Buffer.soft_manual & 0x01) << 6) | ((PNEWELS_Buffer.STATUS_AC & 0x01) << 7));
	status_byte2 = ((PNEWELS_Buffer.mode & 0x07) | ((PNEWELS_Buffer.IsOutputEmergencyLED & 0x01) << 3) | ((PNEWELS_Buffer.IsOutputDisableCharging & 0x01) << 4) | ((PNEWELS_Buffer.oled0 & 0x01) << 5) | ((PNEWELS_Buffer.oled1 & 0x01) << 6) | ((PNEWELS_Buffer.oled2 & 0x01) << 7));
	status_byte3 = ((PNEWELS_Buffer.led_ch1 & 0x01) | ((PNEWELS_Buffer.led_ch2 & 0x01) << 1) | ((PNEWELS_Buffer.led_drv0 & 0x01) << 2) | ((PNEWELS_Buffer.led_drv1 & 0x01) << 3) | ((PNEWELS_Buffer.led_drv2 & 0x01) << 4) | ((PNEWELS_Buffer.soft_charge & 0x01) << 5) | ((PNEWELS_Buffer.soft_discharge & 0x01) << 6));
		
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, time_4_byte);
	memory_address_incr++;
	pne_delayms(10);
		
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, time_3_byte);
	memory_address_incr++;
	pne_delayms(10);
		
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, time_2_byte);
	memory_address_incr++;
	pne_delayms(10);
		
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, time_1_byte);
	memory_address_incr++;
	pne_delayms(10);
		
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, PNEWELS_Buffer.V_BATT);
	memory_address_incr++;
	pne_delayms(10);
		
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, PNEWELS_Buffer.temperature);
	memory_address_incr++;
	pne_delayms(10);
		
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, status_byte1);
	memory_address_incr++;
	pne_delayms(10);
	
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, status_byte2);
	memory_address_incr++;
	pne_delayms(10);
	
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memory_address_high = ((memory_address_incr & 0xFF00) >> 8);
	memory_address_low = (memory_address_incr & 0x00FF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, status_byte3);
	memory_address_incr++;
	pne_delayms(10);
	
	if (memory_address_incr >= 4092)
	{
		memory_address_incr = 0;
	}
	memoryCounter_save(memory_address_incr);
	pne_delayms(10);
	

}

void PNEWELSE2promWrite(uint8_t address, char in_ex_type, uint8_t memory_loc_1, uint8_t memory_loc_2, uint8_t memory_data)
{
	uint8_t e2prom_address_length = 0;
	if (in_ex_type == 'E')
	{
		e2prom_address_length = 2;
	}
	else
	{
		e2prom_address_length = 1;
	}
	uint8_t e2prom_write_address = address;
	uint8_t e2prom_address[2] =
	{
		memory_loc_1,
		memory_loc_2,
	};
	uint8_t write_data[1] =
	{
		memory_data,
	};
	
	pne_APP_TwiInit(e2prom_write_address);
	
	pne_APP_TwiSend(e2prom_address, e2prom_address_length, e2prom_write_address, write_data, 1);
	pne_delayms(5);
}

uint8_t PNEWELSE2promRead(uint8_t address, char in_ex_type, uint8_t memory_loc_1, uint8_t memory_loc_2)
{
	uint8_t e2prom_address_length = 0;
	if (in_ex_type == 'E')
	{
		e2prom_address_length = 2;
	}
	else
	{
		e2prom_address_length = 1;
	}
	uint8_t e2prom_read_address = address;
	uint8_t e2prom_address[2] =
	{
		memory_loc_1,
		memory_loc_2,
	};
	uint8_t read_data[2] = {0,0};
	
	pne_APP_TwiInit(e2prom_read_address);
	
	pne_APP_TwiReceive(e2prom_address, e2prom_address_length, e2prom_read_address, read_data, 1);
	
	return read_data[0];
}

uint8_t PNEWELSTemperatureRead(void)
{
	uint8_t temperature_write_address = 0x90;
	uint8_t temperature_read_address = 0x91;
	uint8_t temperature_register[2] =
	{
		0x00,
		0x00,
	};
	uint8_t data_received[2] = {0, 0};
	uint8_t data = 0;
	
	pne_APP_TwiInit(temperature_write_address);
	
	if (pne_APP_TwiSend(temperature_register, 1, temperature_write_address, 0, 0) == APP_PNEWELS_TWI_WRITE_SUCCESS)
	{
		pne_APP_TwiReceive(temperature_register, 1, temperature_read_address, data_received, 2);
	}
	
	data = (data_received[0]<<1) / 2;

	return data;
}

void clre2prom()
{
	uint8_t memory_address_high = 0;
	uint8_t memory_address_low = 0;
	
	for (uint16_t tmpi=0; tmpi<4092; tmpi++)
	{
		memory_address_low = (tmpi & 0xFF);
		memory_address_high = ((tmpi >> 8) & 0xFF);
		PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, 0xFF);
		pne_delayms(10);
		wdt_reset();
	}
	
	memory_address_incr = 0;
}

void memoryCounter_init()
{
	uint8_t cache = 0;
	for (uint16_t counter = 0; counter<4092; counter = counter + 11)
	{
		cache = readE2prom(counter);
		if(cache == 0xFF)
		{
			memory_address_incr = counter;
			break;
		}
	}
}

void memoryCounter_save(uint16_t log_counter)
{
	writeE2prom(log_counter, 0xFF);
}