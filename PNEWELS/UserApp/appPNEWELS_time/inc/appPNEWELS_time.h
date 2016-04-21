/*
 * appPNEWELS_time.h
 *
 * Created: 1/8/2015 1:57:15 PM
 *  Author: tay
 */ 


#ifndef APPPNEWELS_TIME_H_
#define APPPNEWELS_TIME_H_


#include <stdbool.h> 
#include <stdint.h>

void time_init(void);
void set_time(int year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute);
uint8_t *request_time(void);


#endif /* APPPNEWELS_TIME_H_ */