/*
 * PNE_Factory.c
 *
 * Created: 5/25/2015 10:33:10 AM
 *  Author: tay
 *
 * Modified by zulhilmi
 */ 



//-------------------- Includes ------------------//
#include "PNE_Factory.h"
#include "pneConfig.h"
#include "sysTypes.h"
#include "PNEWELS.h"
#include "adc.h"
#include "pne_appTwi.h"
#include <string.h>
#include "pne_appDelay.h"
#include "pne_rf_processor.h"
#include "memoryConfig.h"
#include "appBattery.h"

//---------- main routine for factory mode ----------//
void factory_mode(void)
{
	configuration_flag = 0; 
	reset_option_flag = 0;
	config_waiting_input = 0;
	flag = 0;
	count_usart = 0;
	usart_state = Usart_start;
	factory_state = factory_startup_message;
	factory_command = groupID_command;
	config_state = config_startup_message;
	reset_state = reset_startup_message;
	config_command = groupID_config;
	button = button1;
	option_select = option_message;
	battery_option_flag = 0;
	battery_state = battery_startup_message;
	
	for (uint8_t i=0; i<max_command_length; i++)
	{
		commamd[i] = 0;
	}


	while (1)
	{
		if(flag == 1)
		{
			if(configuration_flag == 1)
				config_state = configSelect();
			else if(reset_option_flag == 1)
				reset_state = resetSelect();
			else if(battery_option_flag == 1)
				battery_state = batterySelect();
			else option_select = optionSelect();
		}
		
		switch(option_select)
		{
			case config:
				pneConfiguration();
			break;
			
			case display_info:
				pneDisplayInfo();
			break;
			
			case reset_flag:
				resetFlag();
			break;
			
			case option_message:
			{
				//Welcome Message	
				send_usart_char("\r\n\r\n\r\nPNEWELS ");
				send_usart_char(version);
				send_usart_char(" ");
				send_usart_char(version_date);
				send_usart_char("\r\n");
				send_usart_char("1. Configuration\r\n2. Display info\r\n3. Test mode\r\n4. Reset device flag\r\n");
				send_usart_char("5. Dump Memory\r\n6. Dump Log Files\r\n7. Format Memory\r\n8. Battery Configuration\r\n");
				option_select = idle;
			}
			break;
			
			case test_mode:
				testMode();
			break;
			
			case dump_memory:
				dump_eeprom();
			break;
			
			case dump_log:
				dump_log_file();
			break;

			case wipe_memory:
				erase_memory();
			break;
			
			case battery_set:
				battery_config();
			break;	
			
			case idle:
			break;
		}
//			pneModeFactory();
// 		else
// 			send_usart_char("Invalid option/r/n");
	}
}

//---------- USART routine for factory mode ----------//
void pnewels_get_usart(uint8_t tmp)
{
	switch (usart_state)
	{
		case Usart_start:
		{
			for (uint8_t a=0; a<13; a++)
			{
				commamd[a] = 0x20;
			}
			
			if (tmp == 0x5B)
			{
				usart_state = Usart_receive;
			}
			else if (tmp == 0x5D)
			{
				count_usart = 0;
				usart_state = Usart_start;
			}
			else
			{
				
			}
		}
		break;
		
		case Usart_receive:
		{
			if (tmp == 0x5D)
			{
				count_usart = 0;
				usart_state = Usart_start;
				flag = 1;
			}
			else
			{
				if (count_usart<13)
				{
					commamd[count_usart] = tmp;
					count_usart++;
				}
				else
				{
					count_usart = 0;
					usart_state = Usart_start;
					send_usart_char("\r\n[Exceed Command Length Error]");
				}
			}
		}
		break;
	}
}

void send_usart_char(char *tx_buf)
{
	uint8_t i =0;
	
	while (tx_buf[i] != 0x00)
	{
		usart_putchar(USART_SERIAL_EXAMPLE, tx_buf[i]);
		i++;
	}
}

void send_usart(uint8_t *tx_buf)
{
	uint8_t i =0;
	
	while (tx_buf[i] != 0x00)
	{
		usart_putchar(USART_SERIAL_EXAMPLE, tx_buf[i]);
		i++;
	}
}


//---------- side function for factory mode ----------//
void int_to_ascii(uint8_t number)
{
	uint8_t display;
	
	display = ((number>>4) & 0x0F);
	if (display > 9)
	{
		display = display + 0x37;
	}
	else
	{
		display = display + 0x30;
	}
	usart_putchar(USART_SERIAL_EXAMPLE, display);
	
	display = ((number) & 0x0F);
	if (display > 9)
	{
		display = display + 0x37;
	}
	else
	{
		display = display + 0x30;
	}
	usart_putchar(USART_SERIAL_EXAMPLE, display);
}

void pneConfiguration()
{
	switch(config_state)
	{
		case config_startup_message:
		{
			send_usart_char("\r\n\r\nConfiguration mode. Select an option:\r\n\r\n");
/*			send_usart_char("1. Change Group ID\r\n");*/
			send_usart_char("1. Change Device Address\r\n2. Change Device Channel\r\n3. Change Device PAN ID\r\n");
			send_usart_char("0. Exit configuration mode\r\n");
			configuration_flag = 1;
			config_state = config_idle;
		}
		break;
		
// 		case config_GroupID:
// 		{
// 			send_usart_char("Enter new group ID: \r\n");
// 			config_read_input(GROUP_ID, '1', groupID_display);
// 			config_state = config_startup_message;
// 		}
// 		break;
		
// 		case config_ZoneID:
// 		{
// 			send_usart_char("Enter new zone ID: \r\n");
// 			config_read_input(ZONE_ID, '1', zoneID_display);
// 			config_state = config_startup_message;
// 		}
// 		break;
// 		
// 		case config_NodeID:
// 		{
// 			send_usart_char("Enter new node ID: \r\n");
// 			config_read_input(NODE_ID, '1', nodeID_display);
// 			config_state = config_startup_message;
// 		}
// 		break;
		
		case config_DeviceADDR:
		{
			send_usart_char("Enter new device address: \r\n");
			config_read_input(DEVICE_ADDRESS_L, '2', deviceADDR_display);
			config_state = config_startup_message;
		}
		break;
		
		case config_DeviceCH:
		{
			send_usart_char("Enter new device channel: \r\n");
			config_read_input(DEVICE_CHANNEL_L, '2', deviceCH_display);
			config_state = config_startup_message;
		}
		break;
		
		case config_DevicePAN:
		{
			send_usart_char("Enter new device PAN ID: \r\n");
			config_read_input(DEVICE_PAN_L, '2', devicePAN_display);
			config_state = config_startup_message;
		}
		break;
		
		case config_idle:
		break;
		
		case config_exit:
		{
			configuration_flag = 0;
			send_usart_char("Exiting config mode");
			option_select = option_message;
			config_state = config_startup_message;
		}
		default:
			option_select = option_message;
			config_state = config_startup_message;
		break;
		
	}
}

uint8_t optionSelect()
{
	/*char real_command[3] = "123";*/
	flag = 0;
	
	switch(commamd[0])
	{
		case '1':
			return 1;
		break;
		
		case '2':
			return 2;
		break;
		
		case '3':
			return 3;
		break;
		
		case '4':
			return 4;
		break;
		
		case '5':
			return 5;
		break;
		
		case '6':
			return 6;
		break;
		
		case '7':
			return 7;
		break;
		
		case '8':
			return 8;
		break;
		
		case '9':
			return 9;
		break;
		
		default:
			send_usart_char("Invalid command\r\n");
		break;
			
	}
		return idle;
}

void pneDisplayInfo()
{
	uint8_t cache[2]={0};
		
	send_usart_char("Device information:\r\n");
	send_usart_char("\r\nPAN ID (Zone)\t: ");
	cache[0] = readE2prom(DEVICE_PAN_H);
	cache[1] = readE2prom(DEVICE_PAN_L);
	PNEWELS_Buffer.devicePAN = cache[0];
	PNEWELS_Buffer.devicePAN = PNEWELS_Buffer.devicePAN << 8;
	PNEWELS_Buffer.devicePAN |= cache[1];
	int_to_ascii((uint8_t)(PNEWELS_Buffer.devicePAN>>8));
	int_to_ascii((uint8_t)PNEWELS_Buffer.devicePAN);
	
// 	send_usart_char("\r\nZone ID\t\t: ");
// 	PNEWELS_Buffer.zoneID = readE2prom(ZONE_ID);
// 	int_to_ascii(PNEWELS_Buffer.zoneID);

	
// 	send_usart_char("\r\nGroup ID\t\t: ");
// 	PNEWELS_Buffer.groupID = readE2prom(GROUP_ID);
// 	int_to_ascii(PNEWELS_Buffer.groupID);
	
// 	send_usart_char("\r\nNode ID\t\t: ");
// 	PNEWELS_Buffer.nodeID = readE2prom(NODE_ID);
// 	int_to_ascii(PNEWELS_Buffer.nodeID);
	
	send_usart_char("\r\nDevice address\t: ");
	cache[0] = readE2prom(DEVICE_ADDRESS_H);
	cache[1] = readE2prom(DEVICE_ADDRESS_L);
	PNEWELS_Buffer.deviceAddress = cache[0];
	PNEWELS_Buffer.deviceAddress = PNEWELS_Buffer.deviceAddress << 8;
	PNEWELS_Buffer.deviceAddress |= cache[1];
	int_to_ascii((uint8_t)(PNEWELS_Buffer.deviceAddress>>8));
	int_to_ascii((uint8_t)(PNEWELS_Buffer.deviceAddress));
	
	send_usart_char("\r\nDevice channel\t: ");
	cache[0] = readE2prom(DEVICE_CHANNEL_H);
	cache[1] = readE2prom(DEVICE_CHANNEL_L);
	PNEWELS_Buffer.deviceAddress = cache[0];
	PNEWELS_Buffer.deviceAddress = PNEWELS_Buffer.deviceAddress << 8;
	PNEWELS_Buffer.deviceAddress |= cache[1];
	int_to_ascii((uint8_t)(PNEWELS_Buffer.deviceAddress>>8));
	int_to_ascii((uint8_t)(PNEWELS_Buffer.deviceAddress));
	
	//Read EEPROM and display serial number
	uint8_t serial_addr[2] = {0x00, 0x08};
	uint8_t serial_no[16] = {0};
								
	pne_APP_TwiInit(0xBF);
	pne_APP_TwiReceive(serial_addr, 2, 0xBF, serial_no, 16);
	send_usart_char("\r\nSerial Number   : ");
							
	uint8_t compare = 0;
							
	for (compare=0; compare<15; compare++)
	{
		int_to_ascii(serial_no[compare]);
		usart_putchar(USART_SERIAL_EXAMPLE, 0x2E);
	}
							
	int_to_ascii(serial_no[15]);
							
	//Read ADC
	uint8_t battery_voltage = pneBatteryAverage();
	uint8_t battery_capacity = adc_read_8bit(ADC_EMER_LIGHT_INPUT, ADC_VREF);
							
	send_usart_char("\r\nBattery Voltage : ");
	int_to_ascii(battery_voltage);
	send_usart_char("\r\nBattery Capacity: ");
	int_to_ascii(battery_capacity);
	
	option_select = option_message;
}

uint8_t configSelect()
{
	//uint8_t real_command[6] = "123456";
	flag = 0;
	
	switch(commamd[0])
	{
// 		case '1':
// 			return config_GroupID;
// 		break;
		
// 		case '2':
// 			return config_ZoneID;
// 		break;
// 		
// 		case '3':
// 			return config_NodeID;
// 		break;
		
		case '1':
			return config_DeviceADDR;
		break;
		
		case '2':
			return config_DeviceCH;
		break;
		
		case '3':
			return config_DevicePAN;
		break;
		
		case '0':
			return config_exit;
		break;
		
		default:
		{
			send_usart_char("Invalid command\r\n");
			return config_idle;
		}
		break;
		
			
	}
}

void config_read_input(uint16_t address, char data_length, const char *name)
{
	uint8_t data_short = 0;
	uint8_t cache[2] = {0};
	config_waiting_input = 1;
	flag = 0;
	uint8_t memory_address_low = (address & 0xFF);
	uint8_t memory_address_high = ((address >> 8) & 0xFF);
	while(config_waiting_input == 1)
	{
		if(flag == 1)
		{
			switch(data_length)
			{
				case '1':
				{
					cache[0] = ascii_to_hex(commamd[0])<<4;
					cache[1] = ascii_to_hex(commamd[1]);
					data_short = cache[0] | cache[1];
					PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, data_short);
					send_operation_info(name, data_short);
					pne_delayms(10);

					config_waiting_input = 0;
					flag = 0;
				}
				break;
				
				case '2':
				{
					cache[0] = ascii_to_hex(commamd[0])<<4;
					cache[1] = ascii_to_hex(commamd[1]);
					data_short = cache[0] | cache[1];
					PNEWELSE2promWrite(0xAE, 'E', memory_address_low + 1, memory_address_high, data_short);
					cache[0] = ascii_to_hex(commamd[2])<<4;
					cache[1] = ascii_to_hex(commamd[3]);
					send_operation_info(name, data_short);
					data_short = cache[0] | cache[1];
					PNEWELSE2promWrite(0xAF, 'E', memory_address_low , memory_address_high, data_short);
					int_to_ascii(data_short);
					config_waiting_input = 0;
					flag = 0;
				}
				break;
				
				default:
					send_usart_char("Critical Error! Please contact manufacturer.");
				break;
			}
		}
		else
		{
			
		}
	}
}

void send_usart_const_char(const char *tx_buf)
{
	uint8_t i =0;
	
	while (tx_buf[i] != 0x00)
	{
		usart_putchar(USART_SERIAL_EXAMPLE, tx_buf[i]);
		i++;
	}
}

void send_operation_info(const char *name, uint8_t data)
{
	send_usart_const_char(name);
	send_usart_char(" changed to ");
	int_to_ascii(data);
	/*send_usart_char("\r\n");*/
}

void resetFlag()
{
	
	uint8_t memory_address_low = (FLAG & 0xFF);
	uint8_t memory_address_high = ((FLAG >> 8) & 0xFF);
	
	switch(reset_state)
	{
		case reset_startup_message:
		{
			send_usart_char("\r\n\r\nFlag reset mode. Select an option:\r\n\r\n");
			send_usart_char("1. Reset all flag\r\n2. Reset Factory flag (bit0)\r\n3. Reset AC Power flag (bit1)\r\n");
			send_usart_char("4. Read system flag status\r\n5. Set flag\r\n0. Exit configuration mode\r\n");
			reset_option_flag = 1;
			reset_state = reset_idle;
		}
		break;
		
		case reset_all:
		{
 			PNEWELSE2promWrite(0xAE,'E',memory_address_low,memory_address_high,0xFF);
			pne_delayms(10);
			reset_state = reset_startup_message;
		}
		break;
		
		case reset_bit0:
		{
			reset_cache = readE2prom(FLAG);
			reset_cache = (reset_cache | 0x01); 
			PNEWELSE2promWrite(0xAE,'E',memory_address_low,memory_address_high,reset_cache);
			reset_state = reset_startup_message;
		}
		break;
		
		case reset_bit1:
		{
			reset_cache = readE2prom(FLAG);
			reset_cache = (reset_cache | 0x02); 
			PNEWELSE2promWrite(0xAE,'E',memory_address_low,memory_address_high,reset_cache);
			reset_state = reset_startup_message;
		}
		break;
		
		case reset_read_status:
		{
			reset_cache = readE2prom(FLAG);
			send_usart_char("\r\nSystem Flag: ");
			int_to_ascii(reset_cache);
			send_usart_char("\r\n");
			reset_state = reset_startup_message;
		}
		break;

		case reset_set_flag:
		{
			reset_cache = 0xFC;
			PNEWELSE2promWrite(0xAE,'E',memory_address_low,memory_address_high,reset_cache);
			reset_state = reset_startup_message;
		}
		break;
		
		case reset_idle:
		break;
		
		case reset_exit:
		{
			reset_option_flag = 0;
			option_select = option_message;
			config_state = config_startup_message;
			reset_state = reset_startup_message;
			break;
		}
		default:
		option_select = option_message;
		config_state = config_startup_message;
		reset_state = reset_startup_message;
		break;
		
	}
}


uint8_t resetSelect()
{
	//uint8_t real_command[6] = "123456";
	flag = 0;
	
	switch(commamd[0])
	{
		case '1':
		return reset_all;
		break;
		
		case '2':
		return reset_bit0;
		break;
		
		case '3':
		return reset_bit1;
		break;
		
		case '4':
		return reset_read_status;
		break;

		case '5':
		return reset_set_flag;
		break;
		
		case '0':
		return reset_exit;
		break;
		
		default:
		{
			send_usart_char("Invalid command\r\n");
			return reset_idle;
		}
		break;
				
	}
}

void testMode()
{
	uint8_t button_status_factory;
	uint8_t button_status_factory2;
	uint8_t button_status_factory3;
	uint8_t button_status_factory4;
	uint8_t button_status_factory5;
	uint8_t button_status_factory6;
	uint8_t button_status_factory7;
	uint8_t button_status_factory8;
	uint8_t button_status_factory9;
	
	switch(button)
	{
		case button1:
		{
			gpio_clr(STATUS_LED_1);
			gpio_clr(STATUS_LED_2);
			gpio_clr(CHARGING_DISABLE);
			gpio_clr(EMER_LIGHT_EN);
			gpio_clr(OUT_LED_CH0);
			gpio_clr(OUT_LED_CH1);
			gpio_clr(OUT_LED_CH2);
			
			send_usart_char("\r\nIO Switch ");
			send_usart_char("1");
			
			button_status_factory = gpio_read(AC_STAT) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(AC_STAT) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			button_status_factory7 = gpio_read(WPS_BTN) ? 0 : 1;
			button_status_factory8 = gpio_read(VCHARGE_STAT) ? 0 : 1;
			button_status_factory9 = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_set(STATUS_LED_1);
				send_usart_char(" ------PASS");
				button = button2;
			}
		}
		break;
		
		case button2:
		{
			send_usart_char("\r\nIO Switch ");
			send_usart_char("2");
			
			button_status_factory = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(AC_STAT) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			button_status_factory7 = gpio_read(WPS_BTN) ? 0 : 1;
			button_status_factory8 = gpio_read(VCHARGE_STAT) ? 0 : 1;
			button_status_factory9 = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_set(STATUS_LED_2);
				send_usart_char(" ------PASS");
				button = button3;
			}
		}
		break;
		
		case button3:
		{
			send_usart_char("\r\nIO Switch ");
			send_usart_char("3");
			
			button_status_factory = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(AC_STAT) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			button_status_factory7 = gpio_read(WPS_BTN) ? 0 : 1;
			button_status_factory8 = gpio_read(VCHARGE_STAT) ? 0 : 1;
			button_status_factory9 = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_set(CHARGING_DISABLE);
				send_usart_char(" ------PASS");
				button = button4;
			}
		}
		break;
		
		case button4:
		{
			send_usart_char("\r\nIO Switch ");
			send_usart_char("4");
			
			button_status_factory = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(AC_STAT) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			button_status_factory7 = gpio_read(WPS_BTN) ? 0 : 1;
			button_status_factory8 = gpio_read(VCHARGE_STAT) ? 0 : 1;
			button_status_factory9 = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_set(EMER_LIGHT_EN);
				send_usart_char(" ------PASS");
				button = button5;
			}
		}
		break;
		
		case button5:
		{
			send_usart_char("\r\nIO Switch ");
			send_usart_char("5");
			
			button_status_factory = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(AC_STAT) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			button_status_factory7 = gpio_read(WPS_BTN) ? 0 : 1;
			button_status_factory8 = gpio_read(VCHARGE_STAT) ? 0 : 1;
			button_status_factory9 = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_set(OUT_LED_CH0);
				send_usart_char(" ------PASS");
				button = button6;
			}
		}
		break;
		
		case button6:
		{
			send_usart_char("\r\nIO Switch ");
			send_usart_char("6");
			
			button_status_factory = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(AC_STAT) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			button_status_factory7 = gpio_read(WPS_BTN) ? 0 : 1;
			button_status_factory8 = gpio_read(VCHARGE_STAT) ? 0 : 1;
			button_status_factory9 = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_set(OUT_LED_CH1);
				send_usart_char(" ------PASS");
				button = button7;
			}
		}
		break;
		
		case button7:
		{
			send_usart_char("\r\nIO Switch ");
			send_usart_char("7");
			
			button_status_factory = gpio_read(WPS_BTN) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(WPS_BTN) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(AC_STAT) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			button_status_factory7 = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			button_status_factory8 = gpio_read(VCHARGE_STAT) ? 0 : 1;
			button_status_factory9 = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_set(OUT_LED_CH2);
				send_usart_char(" ------PASS");
				button = button8;
			}
		}
		break;
		
		case button8:
		{
			send_usart_char("\r\nIO Switch ");
			send_usart_char("8");
			
			button_status_factory = gpio_read(VCHARGE_STAT) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(VCHARGE_STAT) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(AC_STAT) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			button_status_factory7 = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			button_status_factory8 = gpio_read(WPS_BTN) ? 0 : 1;
			button_status_factory9 = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_clr(OUT_LED_CH0);
				gpio_clr(OUT_LED_CH1);
				gpio_clr(OUT_LED_CH2);
				send_usart_char(" ------PASS");
				button = button9;
			}
		}
		break;
		
		case button9:
		{
			send_usart_char("\r\nIO Switch ");
			send_usart_char("9");
			
			button_status_factory = gpio_read(MANUAL_BTN) ? 0 : 1;
			
			while(button_status_factory != 1)
			{
				button_status_factory = gpio_read(MANUAL_BTN) ? 0 : 1;
			}
			
			button_status_factory2 = gpio_read(AC_STAT) ? 0 : 1;
			button_status_factory3 = gpio_read(INPUT_LED_CH1) ? 0 : 1;
			button_status_factory4 = gpio_read(INPUT_LED_CH2) ? 0 : 1;
			button_status_factory5 = gpio_read(INPUT_LED_DRV0) ? 0 : 1;
			button_status_factory6 = gpio_read(INPUT_LED_DRV1) ? 0 : 1;
			button_status_factory7 = gpio_read(INPUT_LED_DRV2) ? 0 : 1;
			button_status_factory8 = gpio_read(WPS_BTN) ? 0 : 1;
			button_status_factory9 = gpio_read(VCHARGE_STAT) ? 0 : 1;
			
			if ((button_status_factory2 | button_status_factory3 | button_status_factory4 | button_status_factory5 | button_status_factory6 | button_status_factory7 | button_status_factory8 | button_status_factory9) != 0)
			{
				send_usart_char(" ------FAIL");
				send_usart_char("\r\nMultiple Input Turn On ...");
				button = button1;
			}
			else
			{
				gpio_clr(STATUS_LED_1);
				gpio_clr(STATUS_LED_2);
				gpio_clr(CHARGING_DISABLE);
				gpio_clr(EMER_LIGHT_EN);
				gpio_set(OUT_LED_CH0);
				gpio_set(OUT_LED_CH1);
				gpio_set(OUT_LED_CH2);
				send_usart_char(" ------PASS");
				send_usart_char("\r\n------ Test Finished ------\r\n------ Successful ------");
				button = button10;
			}
			
		}
		break;
		
		case button10:
		{
			option_select = option_message;
		}
		break;
	}

}

void dump_eeprom()
{
	uint8_t block = 0;
	uint8_t memory_address_high = 0;
	uint8_t memory_address_low = 0;
	
	for (uint16_t tmpi=0; tmpi<8192; tmpi++)
	{
		if(block == 32)
		{
			send_usart_char("\r\n");
			block = 0;
		}
		memory_address_low = (tmpi & 0xFF);
		memory_address_high = ((tmpi >> 8) & 0xFF);
		int_to_ascii(PNEWELSE2promRead(0xAF, 'E', memory_address_low, memory_address_high));
		send_usart_char(" ");
		pne_delayms(5);
		block++;
		
	}
	option_select = option_message;
}

uint8_t readE2prom(uint16_t address)
{
	uint8_t cache = 0;
	uint8_t memory_address_low = (address & 0xFF);
	uint8_t memory_address_high = ((address >> 8) & 0xFF);
	cache = PNEWELSE2promRead(0xAF, 'E', memory_address_low, memory_address_high);
	return cache;
}

void dump_log_file()
{
	uint8_t block = 0;
	uint8_t memory_address_high = 0;
	uint8_t memory_address_low = 0;
	
	for (uint16_t tmpi=0; tmpi<4092; tmpi++)
	{
		if(block == 11)
		{
			send_usart_char("\r\n");
			block = 0;
		}
		memory_address_low = (tmpi & 0xFF);
		memory_address_high = ((tmpi >> 8) & 0xFF);
		int_to_ascii(PNEWELSE2promRead(0xAF, 'E', memory_address_low, memory_address_high));
		send_usart_char(" ");
		pne_delayms(5);
		block++;
		
	}
	option_select = option_message;
}

void erase_memory()
{
	clre2prom();
	option_select = option_message;
}

void writeE2prom(uint16_t address, uint8_t data)
{
	uint8_t memory_address_low = (address & 0xFF);
	uint8_t memory_address_high = ((address >> 8) & 0xFF);
	PNEWELSE2promWrite(0xAE, 'E', memory_address_low, memory_address_high, data);
	pne_delayms(10);
}

void battery_config()
{	
	switch(battery_state)
	{
		case battery_startup_message:
		{
			send_usart_char("\r\n\r\nBattery Configuration Mode. Select an option:\r\n\r\n");
			send_usart_char("1. Set Battery Low Threshold\r\n2. Set Battery Max Threshold\r\n");
			send_usart_char("3. Display Battery Threshold\r\n0. Exit\r\n");
			battery_option_flag = 1;
			battery_state = battery_idle;
		}
		break;
		
		case batteryLow:
		{
			send_usart_char("Enter battery low threshold value (00-FF): \r\n");
			config_read_input(BATTERY_LOW, '1', batteryLOW_display);
			battery_state = battery_startup_message;
		}
		break;
		
		case batteryMax:
		{
			send_usart_char("Enter battery max threshold value (00-FF): \r\n");
			config_read_input(BATTERY_MAX, '1', batteryMAX_display);
			battery_state = battery_startup_message;
		}
		break;
		
		case batteryStatus:
		{
			battery_cache = readE2prom(BATTERY_LOW);
			send_usart_char("\r\nBattery Low Threshold: ");
			int_to_ascii(battery_cache);
			send_usart_char("\r\n");
			battery_cache = readE2prom(BATTERY_MAX);
			send_usart_char("\r\nBattery MAX Threshold: ");
			int_to_ascii(battery_cache);
			send_usart_char("\r\n");
			battery_state = battery_startup_message;
		}
		break;
	
		case battery_idle:
		break;
		
		case battery_exit:
		{
			battery_option_flag = 0;
			option_select = option_message;
			config_state = config_startup_message;
			battery_state = battery_startup_message;
			break;
		}
		default:
		option_select = option_message;
		config_state = config_startup_message;
		battery_state = battery_startup_message;
		break;
		
	}
}


uint8_t batterySelect()
{
	//uint8_t real_command[6] = "123456";
	flag = 0;
	
	switch(commamd[0])
	{
		case '1':
		return batteryLow;
		break;
		
		case '2':
		return batteryMax;
		break;
		
		case '3':
		return batteryStatus;
		break;
		
		case '0':
		return battery_exit;
		break;
		
		default:
		{
			send_usart_char("Invalid command\r\n");
			return  battery_idle;
		}
		break;
		
	}
}