/*
 * gpio_megarf.h
 *
 * Created: 1/6/2015 2:12:44 PM
 *  Author: tay
 */ 


#ifndef GPIO_MEGARF_H_
#define GPIO_MEGARF_H_

#include "sysTypes.h"

typedef enum
{
	EXT_INT0_PIN,
	EXT_INT1_PIN,
	EXT_INT2_PIN,
	EXT_INT3_PIN,
	EXT_INT4_PIN,
	EXT_INT5_PIN,
	EXT_INT6_PIN,
	EXT_INT7_PIN,
	PC_INT0_PIN,
	PC_INT1_PIN,
	PC_INT2_PIN,
	PC_INT3_PIN,
	PC_INT4_PIN,
	PC_INT5_PIN,
	PC_INT6_PIN,
	PC_INT7_PIN,
	PC_INT8_PIN
}interrupt_t;

typedef enum
{
	port_b,
	port_d,
	port_e,
	port_f,
	port_g
}port_id_t;

typedef struct
{
	port_id_t port_id;
	uint8_t pin_id;
	interrupt_t int_id;
}gpio_t;


#endif /* GPIO_MEGARF_H_ */