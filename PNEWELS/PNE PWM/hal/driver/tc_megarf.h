/*
 * tc_megarf.h
 *
 * Created: 1/13/2015 5:09:29 PM
 *  Author: tay
 */ 


#ifndef TC_MEGARF_H_
#define TC_MEGARF_H_

#include "sysTypes.h"

void timer_init(uint8_t brightness)
{	
	TIMSK2 |= (0 << OCIE2A);                            //disable output compare match a interrupt
	OCR2A = brightness;                                 //write compare value
	TCCR2A = (1<<WGM20) | (1<<WGM21) | (1<<COM2A1);     //fast pwm mode and non inverted pwm
	TCCR2B = (1 << CS22) | (1 << CS21) | (1 << CS20);   //no divide 1024
	TIMSK2 |= (1 << OCIE2A);                            //enable output compare match a interrupt
	DDRB |= (1 << DDRB4);                               //output to portb4
}



#endif /* TC_MEGARF_H_ */