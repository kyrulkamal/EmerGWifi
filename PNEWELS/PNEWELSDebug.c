/*
 * PNEWELSDebug.c
 *
 * Created: 10/19/2015 13:27:27
 * Author: Zulhilmi
 * This source file is specifically used for debug the device
 */ 
#include "PNE_Factory.h"
#include "PNEWELS.h"

uint8_t debug = 0;

void pne_debug_Init(void){
	//To read ANT_DIV register

}

void pne_debug_display(void){
	debug = PHY_RSSI;
	int_to_ascii(debug);
}