/*
 * ext_int_megarf.h
 *
 * Created: 2/25/2015 3:27:01 PM
 *  Author: tay
 */ 


#ifndef EXT_INT_MEGARF_H_
#define EXT_INT_MEGARF_H_


#include "gpio_megarf.h"

enum ioport_sense {
	IOPORT_SENSE_LEVEL,     /*!< IOPORT sense low level  */
	IOPORT_SENSE_BOTHEDGES, /*!< IOPORT sense both rising and falling edges */
	IOPORT_SENSE_FALLING,   /*!< IOPORT sense falling edges */
	IOPORT_SENSE_RISING,    /*!< IOPORT sense rising edges */
};

typedef void (*ext_int_callback_t) (void);

#define CLEAR_INT_FLAG(pin) EIFR |= (1 << pin)
#define CLEAR_PCINT_FLAG0() PCIFR |= (1 << PCIF0)
#define CLEAR_PCINT_FLAG1() PCIFR |= (1 << PCIF1)
#define INT_PIN_MASK(pin) (1 << pin)

void ext_int_init(gpio_t gpio_id, enum ioport_sense trigmode);
void ext_int_pcint_init(gpio_t gpio_id);
void ext_int_set_interrupt_callback(gpio_t gpio_id, ext_int_callback_t callback);


#endif /* EXT_INT_MEGARF_H_ */