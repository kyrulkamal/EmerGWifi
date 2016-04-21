/*
 * PNEWELSFactoryInit.h
 *
 * Created: 10/30/2015 14:28:16
 *  Author: Zulhilmi
 */ 
//Configuration for factory initialization inside here.//
//These are value to be written to device information block in EEPROM//

#ifndef PNEWELSFACTORYINIT_H_
#define PNEWELSFACTORYINIT_H_

#define ADDRESS_DEFAULT_L 0x01
#define ADDRESS_DEFAULT_H 0x00

#define CHANNEL_DEFAULT_L 0x10
#define CHANNEL_DEFAULT_H 0x00

#define PAN_DEFAULT_L 0x34
#define PAN_DEFAULT_H 0x12

#define  BATTERY_LOW_THRESH 0xaa
#define  BATTERY_MAX_THRESH 0xee
// #define GROUP_DEFAULT 0x00
// #define ZONE_DEFAULT 0x00
// #define NODE_DEFAULT 0x00

//#define FACTORY_DEFAULT 0x01

void factory_initialization();
void systemFlag_check();
void write_factory_default(uint16_t address, uint8_t factory_default);

#endif /* PNEWELSFACTORYINIT_H_ */