/*
 * appBattery.h
 *
 * Created: 12/1/2015 15:38:48
 *  Author: Zulhilmi
 */ 


#ifndef APPBATTERY_H_
#define APPBATTERY_H_

uint8_t battery_counter;
uint8_t battery_average[5];

void pneBatteryAverage_init();
uint8_t pneBatteryAverage();


#endif /* APPBATTERY_H_ */