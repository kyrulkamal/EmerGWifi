/*
 * appBattery.c
 *
 * Created: 12/1/2015 15:39:23
 *  Author: Zulhilmi
 */ 
#include "ext_int_megarf.h"
#include "adc.h"
#include "pneConfig.h"
#include "appBattery.h"
#include "PNE_Factory.h"



void pneBatteryAverage_init()
{
	battery_counter = 0;
	uint8_t i=0;
		
	for(i=0; i<5; i++)
	{
		battery_average[i] = adc_read_8bit(ADC_BATTERY_INPUT, ADC_VREF);
		//int_to_ascii(battery_average[i]);
	}
}

uint8_t pneBatteryAverage()
{
	if(battery_counter > 5)
	{
		battery_counter = 0;
	}
	
	battery_average[battery_counter] = adc_read_8bit(ADC_BATTERY_INPUT, ADC_VREF);//*18*10/255;
	battery_counter++;
	
	return (battery_average[0]+battery_average[1]+battery_average[2]+battery_average[3]+battery_average[4])/5;
}
