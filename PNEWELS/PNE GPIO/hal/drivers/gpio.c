/*
 * gpio.c
 *
 * Created: 2/26/2015 3:47:26 PM
 *  Author: jason
 */ 

#include "gpio.h"

/// <summary>
/// Set high the output of GPIO
/// </summary>
/// <param name="gpio_t">GPIO parameter</param>
void gpio_set(gpio_t gpio_id)
{
	port_id_t port_id = gpio_id.port_id;
	uint8_t pin_id = gpio_id.pin_id;
	
	switch (port_id)
	{
		case port_b:
		PORTB |= (1<<pin_id);
		break;
		
		case port_d:
		PORTD |= (1<<pin_id);
		break;
		
		case port_e:
		PORTE |= (1<<pin_id);
		break;
		
		case port_f:
		PORTF |= (1<<pin_id);
		break;
		
		case port_g:
		PORTG |= (1<<pin_id);
		break;
		
		default:
		break;
	}
}

/// <summary>
/// Set low the output of GPIO
/// </summary>
/// <param name="gpio_t">GPIO parameter</param>
void gpio_clr(gpio_t gpio_id)
{
	port_id_t port_id = gpio_id.port_id;
	uint8_t pin_id = gpio_id.pin_id;
	uint8_t temp = 0;
	uint8_t temp1 = 0;
	
	switch (port_id)
	{
		case port_b:
		PORTB &= ~(1<<pin_id);
		break;
		
		case port_d:
		PORTD &= ~(1<<pin_id);
		break;
		
		case port_e:
		temp = (1<<pin_id);
		temp1 = ~(temp);
		PORTE &= temp1;
		break;
		
		case port_f:
		PORTF &= ~(1<<pin_id);
		break;
		
		case port_g:
		PORTG &= ~(1<<pin_id);
		break;
		
		default:
		break;
	}
}

/// <summary>
/// Toggle the output of GPIO
/// </summary>
/// <param name="gpio_t">GPIO parameter</param>
void gpio_toggle(gpio_t gpio_id)
{
	port_id_t port_id = gpio_id.port_id;
	uint8_t pin_id = gpio_id.pin_id;
	
	switch (port_id)
	{
		case port_b:
		PORTB ^= (1 << pin_id);
		break;
		
		case port_d:
		PORTD ^= (1 << pin_id);
		break;
		
		case port_e:
		PORTE ^= (1 << pin_id);
		break;
		
		case port_f:
		PORTF ^= (1 << pin_id);
		break;
		
		case port_g:
		PORTG ^= (1 << pin_id);
		break;
		
		default:
		break;
	}
}

/// <summary>
/// Set GPIO as input
/// </summary>
/// <param name="gpio_t">GPIO parameter</param>
void gpio_in(gpio_t gpio_id)
{
	port_id_t port_id = gpio_id.port_id;
	uint8_t pin_id = gpio_id.pin_id;
	
	switch (port_id)
	{
		case port_b:
		DDRB &= ~(1 << pin_id);
		PORTB &= ~(1 << pin_id);
		break;
		
		case port_d:
		DDRD &= ~(1 << pin_id);
		PORTD &= ~(1 << pin_id);
		break;
		
		case port_e:
		DDRE &= ~(1 << pin_id);
		PORTE &= ~(1 << pin_id);
		break;
		
		case port_f:
		DDRF &= ~(1 << pin_id);
		PORTF &= ~(1 << pin_id);
		break;
		
		case port_g:
		DDRG &= ~(1 << pin_id);
		PORTG &= ~(1 << pin_id);
		break;
		
		default:
		break;
	}
}

/// <summary>
/// Set GPIO as output
/// </summary>
/// <param name="gpio_t">GPIO parameter</param>
void gpio_out(gpio_t gpio_id)
{
	port_id_t port_id = gpio_id.port_id;
	uint8_t pin_id = gpio_id.pin_id;
	
	switch (port_id)
	{
		case port_b:
		DDRB |= (1<<pin_id);
		break;
		
		case port_d:
		DDRD |= (1<<pin_id);
		break;
		
		case port_e:
		DDRE |= (1<<pin_id);
		break;
		
		case port_f:
		DDRF |= (1<<pin_id);
		break;
		
		case port_g:
		DDRG |= (1<<pin_id);
		break;
		
		default:
		break;
	}
}

/// <summary>
/// Enable pull-up resistor at the input pin
/// </summary>
/// <param name="gpio_t">GPIO parameter</param>
void gpio_pullup(gpio_t gpio_id)
{
	port_id_t port_id = gpio_id.port_id;
	uint8_t pin_id = gpio_id.pin_id;
	
	switch (port_id)
	{
		case port_b:
		PORTB |= (1 << pin_id);
		break;
		
		case port_d:
		PORTD |= (1 << pin_id);
		break;
		
		case port_e:
		PORTE |= (1 << pin_id);
		break;
		
		case port_f:
		PORTF |= (1 << pin_id);
		break;
		
		case port_g:
		PORTG |= (1 << pin_id);
		break;
		
		default:
		break;
	}
}

/// <summary>
/// Read the value of pin
/// </summary>
/// <param name="gpio_t">GPIO parameter</param>
/// <returns>The value of pin</returns>
uint8_t gpio_read(gpio_t gpio_id)
{
	port_id_t port_id = gpio_id.port_id;
	uint8_t pin_id = gpio_id.pin_id;
	
	switch (port_id)
	{
		case port_b:
		return (PINB & (1 << pin_id)) != 0;
		break;
		
		case port_d:
		return (PIND & (1 << pin_id)) != 0;
		break;
		
		case port_e:
		return (PINE & (1 << pin_id)) != 0;
		break;
		
		case port_f:
		return (PINF & (1 << pin_id)) != 0;
		break;
		
		case port_g:
		return (PING & (1 << pin_id)) != 0;
		break;
		
		default:
		break;
	}
	
	return 0;
}

/// <summary>
/// Read the state of the GPIO, (input or output)
/// </summary>
/// <param name="gpio_t">GPIO parameter</param>
/// <returns>The state of pin</returns>
uint8_t gpio_state(port_id_t port_id, uint8_t pin_id)
{	
	switch (port_id)
	{
		case port_b:
		return (DDRB & (1 << pin_id)) != 0;
		break;
		
		case port_d:
		return (DDRD & (1 << pin_id)) != 0;
		break;
		
		case port_e:
		return (DDRE & (1 << pin_id)) != 0;
		break;
		
		case port_f:
		return (DDRF & (1 << pin_id)) != 0;
		break;
		
		case port_g:
		return (DDRG & (1 << pin_id)) != 0;
		break;
		
		default:
		break;
	}
	
	return 0;
}

