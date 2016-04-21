/*
 * gpio.h
 *
 * Created: 1/6/2015 10:47:55 AM
 *  Author: tay
 */ 


#ifndef GPIO_H_
#define GPIO_H_


#include "gpio_megarf.h"
#include "sysTypes.h"

void gpio_set(gpio_t gpio_id);
void gpio_clr(gpio_t gpio_id);
void gpio_toggle(gpio_t gpio_id);
void gpio_in(gpio_t gpio_id);
void gpio_out(gpio_t gpio_id);
void gpio_pullup(gpio_t gpio_id);
uint8_t gpio_read(gpio_t gpio_id);
uint8_t gpio_state(port_id_t port_id, uint8_t pin_id);

#endif /* GPIO_H_ */