/*
 * PNEWELSRFInit.c
 *
 * Created: 10/29/2015 15:53:56
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

uint16_t rf_init(uint16_t address, uint16_t app_type)
{
	uint8_t cache[2] = {0};
	uint8_t memory_address_low = (address & 0xFF);
	uint8_t memory_address_high = ((address >> 8) & 0xFF);
		
	cache[0] = PNEWELSE2promRead(0xAF,'E',memory_address_low+1,memory_address_high);
	cache[1] = PNEWELSE2promRead(0xAF,'E',memory_address_low,memory_address_high);
	app_type = cache[0];
	app_type = app_type << 8;
	app_type |= cache[1];
	return app_type;
}