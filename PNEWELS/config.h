/**
 * \file config.h
 *
 * \brief Template application and stack configuration
 *
 * Copyright (C) 2012-2013, Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 * $Id: config.h 8119 2013-06-05 21:05:45Z ataradov $
 *
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_


#include "sysTypes.h"

uint16_t device_address;
uint16_t device_panID;

/*- Network Definitions ------------------------------------------------------------*/
#define APP_ADDR                  0x0005 /// Address of device. This config is obselete as address is taken from EEPROM
#define APP_CHANNEL               0x0e /// Frequency channel of RF. Between 0x0b to 0x1b. default: 0x0b. REMARKS: Obselete config. Using data from EEPROM instead.
#define APP_PANID                 0x1235 /// Personal ID. Must be same between communicated device. between 0x0000 to 0xFFFF. default: 0x1234. REMARKS: Obselete config. Using data from EEPROM instead.
#define APP_ENDPOINT              0x01 ///RF endpoint. DO NOT CHANGE THIS

#define APP_UPDATE_INTERVAL       10000 /// Update interval for sending data to gateway in ms. Default:5000ms.

#define APP_NWK_BUFFERS_SIZE      20 /// defines number of buffers that can be allocated at the same time for the commands. Frequently application may need more than one buffer to store information about quick events. For example pressing and releasing a button will generate two separate events and second event can happen faster than the first one is transmitted. Note that even if one of the events is lost, clients still will be updated of the new state of the device due to the periodic state reporting.

#define APP_NWK_RETRIES           1 ///Number of retry to deliver message.

#define APP_SECURITY_KEY          "Security12345678" /// AES128 key. All gateways and devices MUST use same key. Incorrect key can cause communication error. The key lenght MUST BE 16.

#define SYS_SECURITY_MODE                   0 /// Security mode. 0 is for AES128. Use this only as this MCU has hardware accelerated AES128 encoder and decoder

#define NWK_BUFFERS_AMOUNT                  20 /// Number of buffer reserved for stack operation. Highly advised to not change this
#define NWK_DUPLICATE_REJECTION_TABLE_SIZE  10  /// Number of entries for detect and reject duplicate frames.
#define NWK_DUPLICATE_REJECTION_TTL         3000 /// Duration in ms duplicate frames is retained. In event where routing multiple device causing the network misbehave, increase this value.
#define NWK_ROUTE_TABLE_SIZE                255 /// Number of entries for routing table. Do no set this too high to avoid memory constraint. Set to expected number of device in an area. In event where routing multiple device causing the network misbehave, decrease this value.
#define NWK_ROUTE_DEFAULT_SCORE             3   /// Number of retry to routing data before it discarded
#define NWK_ACK_WAIT_TIME                   1000 /// Frame acknowledgement time in ms.

#define NWK_ENABLE_ROUTING  /// Define this to enable routing mode across the network.
#define NWK_ENABLE_SECURITY /// Define this to enable encryption when delivering message.

#define HAL_ENABLE_UART /// Define this to enable UART
#define HAL_UART_CHANNEL                    1   /// UART channel
#define HAL_UART_RX_FIFO_SIZE               32  /// UART rx buffer size.
#define HAL_UART_TX_FIFO_SIZE               32  /// UART tx buffer size
#define PHY_ENABLE_RANDOM_NUMBER_GENERATOR  /// Define this to enable random number generator
//#define PHY_ENABLE_ENERGY_DETECTION  //enable Energy Detection Measurement

#define BROADCAST_ADDR			0xffff
#define BROADCAST_CHANNEL		0x0010
#define BROADCAST_PANID			0xffff

#endif // _CONFIG_H_
