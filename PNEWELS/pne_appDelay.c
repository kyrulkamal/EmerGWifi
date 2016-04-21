/*
 * pne_appDelay.c
 *
 * Created: 2/27/2015 10:34:21 AM
 *  Author: tay
 */ 

#include "pne_appDelay.h"

void pne_delayms(uint16_t milisecond)
{
	uint16_t microsecond = milisecond * 1000;
	
	for (int i=0; i<microsecond; i++)
	{
	}
}