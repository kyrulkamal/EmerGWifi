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

/// <summary>
/// Check the signature of the received data and return the state.
/// </summary>
/// <param name="data">Data received</param>
/// <param name="size">Size of data received</param>
/// <returns>Signature value</returns>
uint8_t pneVerify_received(uint8_t *data, uint8_t size)
{
	uint16_t signature_d = (((uint16_t)data[size-2]) << 8) | ((uint16_t)data[size-1]); //get the embedded signature from the source. This will be used as comparison.
	uint16_t signature = pnesign(((unsigned char*)(data)), ((uint16_t)(size - 2)),(unsigned char*) SIGNATURE_KEY, strlen(SIGNATURE_KEY));
	if (signature == signature_d )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/// <summary>
/// Check the signature of the data to be sent and return the signature value.
/// </summary>
/// <param name="data">Data to be sent</param>
/// <param name="size">Size of data to be sent</param>
/// <returns>Signature value</returns>
uint16_t pneVerify_send(uint8_t *data, uint8_t size)
{
	uint16_t signature = pnesign(((unsigned char*) (data)), ((uint16_t)(size - 2)),(unsigned char*) SIGNATURE_KEY, strlen(SIGNATURE_KEY));

	return signature;
}