/*
 * appVerify.c
 *
 * Created: 12/2/2015 09:06:04
 *  Author: Zulhilmi
 * 
 * This driver is for device. This driver slightly different from Gateway driver.
 */ 

#include "pnesign.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "PNE_Factory.h"
#include "crc.h"
#include "pneConfig.h"

uint8_t pneVerify_received(uint8_t *data, uint8_t size)
{
	uint16_t signature_d = (((uint16_t)data[size-2]) << 8) | ((uint16_t)data[size-1]);
// 	int_to_ascii((uint8_t)(signature_d >> 8));
// 	int_to_ascii((uint8_t)(signature_d));
// 	send_usart_char("\r\n");
	/*int_to_ascii(data[9]);*/
	uint16_t signature = pnesign(((char*)(data)), ((uint16_t)(size - 2)), SIGNATURE_KEY, strlen(SIGNATURE_KEY)); //need some rework
// 	int_to_ascii((uint8_t)(signature >> 8));
// 	int_to_ascii((uint8_t)(signature));
// 	send_usart_char("\r\n");
	if (signature == signature_d )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

uint16_t pneVerify_send(uint8_t *data, uint8_t size)
{
	uint16_t signature = pnesign(((char*) (data)), ((uint16_t)(size - 2)), SIGNATURE_KEY, strlen(SIGNATURE_KEY)); //need some rework

	return signature;
}