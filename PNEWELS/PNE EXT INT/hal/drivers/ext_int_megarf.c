/*
 * ext_int_megarf.c
 *
 * Created: 2/27/2015 2:23:52 PM
 *  Author: tay
 */


#include "ext_int_megarf.h"
#include "gpio.h"
 
 //! \internal Local storage of EXT_INT0 interrupt callback function
 static ext_int_callback_t ext_int_int0_callback;
 //! \internal Local storage of EXT_INT1 interrupt callback function
 static ext_int_callback_t ext_int_int1_callback;
 //! \internal Local storage of EXT_INT2 interrupt callback function
 static ext_int_callback_t ext_int_int2_callback;
 //! \internal Local storage of EXT_INT3 interrupt callback function
 static ext_int_callback_t ext_int_int3_callback;
 //! \internal Local storage of EXT_INT4 interrupt callback function
 static ext_int_callback_t ext_int_int4_callback;
 //! \internal Local storage of EXT_INT0 interrupt callback function
 static ext_int_callback_t ext_int_int5_callback;
 //! \internal Local storage of EXT_INT6 interrupt callback function
 static ext_int_callback_t ext_int_int6_callback;
 //! \internal Local storage of EXT_INT7 interrupt callback function
 static ext_int_callback_t ext_int_int7_callback;
 //! \internal Local storage of PC_EXT_INT0 to PC_EXT_INT7 interrupt callback function
 static ext_int_callback_t ext_int_pcint0_callback;
 //! \internal Local storage of PC_EXT_INT8 interrupt callback function
 static ext_int_callback_t ext_int_pcint1_callback;
 
/**
 * \internal
 * \brief Interrupt handler for External Interrupt INT0
 *
 * This function will handle interrupt on External Interrupt INT0 and
 * call the callback function.
 */
ISR(INT0_vect)
{
	if (ext_int_int0_callback) {
		ext_int_int0_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for External Interrupt INT1
 *
 * This function will handle interrupt on External Interrupt INT1 and
 * call the callback function.
 */
ISR(INT1_vect)
{
	if (ext_int_int1_callback) {
		ext_int_int1_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for External Interrupt INT2
 *
 * This function will handle interrupt on External Interrupt INT2 and
 * call the callback function.
 */
ISR(INT2_vect)
{
	if (ext_int_int2_callback) {
		ext_int_int2_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for External Interrupt INT3
 *
 * This function will handle interrupt on External Interrupt INT3 and
 * call the callback function.
 */
ISR(INT3_vect)
{
	if (ext_int_int3_callback) {
		ext_int_int3_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for External Interrupt INT4
 *
 * This function will handle interrupt on External Interrupt INT4 and
 * call the callback function.
 */
ISR(INT4_vect)
{
	if (ext_int_int4_callback) {
		ext_int_int4_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for External Interrupt INT5
 *
 * This function will handle interrupt on External Interrupt INT5 and
 * call the callback function.
 */
ISR(INT5_vect)
{
	if (ext_int_int5_callback) {
		ext_int_int5_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for External Interrupt INT6
 *
 * This function will handle interrupt on External Interrupt INT6 and
 * call the callback function.
 */
ISR(INT6_vect)
{
	if (ext_int_int6_callback) {
		ext_int_int6_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for External Interrupt INT7
 *
 * This function will handle interrupt on External Interrupt INT7 and
 * call the callback function.
 */
ISR(INT7_vect)
{
	if (ext_int_int7_callback) {
		ext_int_int7_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for PC External Interrupt INT0 - INT7
 *
 * This function will handle interrupt on PC External Interrupt INT0-INT7 and
 * call the callback function.
 */
ISR(PCINT0_vect)
{
	if (ext_int_pcint0_callback) {
		ext_int_pcint0_callback();
	}
}

/**
 * \internal
 * \brief Interrupt handler for PC External Interrupt INT8
 *
 * This function will handle interrupt on PC External Interrupt INT8 and
 * call the callback function.
 */
ISR(PCINT1_vect)
{
	if (ext_int_pcint1_callback) {
		ext_int_pcint1_callback();
	}
}

void ext_int_pcint_clear_flag(gpio_t gpio_id)
{
	interrupt_t pin = gpio_id.int_id;
	
	if (pin == PC_INT8_PIN) {
		CLEAR_PCINT_FLAG1();
		} else {
		CLEAR_PCINT_FLAG0();
	}
}

void ext_int_pcint_enable(gpio_t gpio_id)
{
	interrupt_t int_id = gpio_id.int_id;
	uint8_t pin = gpio_id.pin_id;
	
	if (int_id == PC_INT8_PIN) {
		PCICR |= (1 << PCIE1);

		PCMSK1 |= INT_PIN_MASK(0);
		} else {
		PCICR |= (1 << PCIE0);

		PCMSK0 |= INT_PIN_MASK(pin);
	}
}

void ext_int_pcint_disable(gpio_t gpio_id)
{
	interrupt_t int_id = gpio_id.int_id;
	uint8_t pin = gpio_id.pin_id;
	
	if (int_id == PC_INT8_PIN) {
		PCMSK1 &= ~INT_PIN_MASK(0);
		} else {
		PCMSK0 &= ~INT_PIN_MASK(pin);
	}
}

void ext_int_clear_flag(uint8_t pin)
{
	CLEAR_INT_FLAG(pin);
}

void ext_int_disable(uint8_t pin)
{
	EIMSK &= ~INT_PIN_MASK(pin);
}

void ext_int_enable(uint8_t pin)
{
	EIMSK |= INT_PIN_MASK(pin);
}

void ioport_set_pin_sense_mode(uint8_t pin, enum ioport_sense trigmode)
{
	if (pin < 0x04)
	{
		EICRA &= ~((0x03) << ((pin & 0x03) * 2));
		EICRA |= trigmode << ((pin & 0x03) * 2);
	}
	else if (pin > 0x07)
	{
		EICRB &= ~((0x03) << ((pin & 0x03) * 2));
		EICRB |= trigmode << ((pin & 0x03) * 2);
	}
}

void ext_int_init(gpio_t gpio_id, enum ioport_sense trigmode)
{
	uint8_t pin = gpio_id.pin_id;
	
	/* Disable the interrupt */
	ext_int_disable(pin);
	
	/* Enable IRQ pin as input */
	/* Enable the pullup for the IRQ pin */
	gpio_in(gpio_id);
	gpio_pullup(gpio_id);

	/* Setup interrupt sence control */
	ioport_set_pin_sense_mode(pin,trigmode);
	
	/* Clear the INTn interrupt flag */
	ext_int_clear_flag(pin);

	/* Enable the interrupt */
	ext_int_enable(pin);
}

void ext_int_pcint_init(gpio_t gpio_id)
{
	/* Disable the interrupt */
	ext_int_pcint_disable(gpio_id);

	/* Enable IRQ pin as input */
	/* Enable the pullup for the IRQ pin */
	gpio_in(gpio_id);
	gpio_pullup(gpio_id);

	/* Clear the PCINTn interrupt flag */
	ext_int_pcint_clear_flag(gpio_id);

	/* Enable the interrupt */
	ext_int_pcint_enable(gpio_id);
}

void ext_int_set_interrupt_callback(gpio_t gpio_id, ext_int_callback_t callback)
{
	interrupt_t ext_int = gpio_id.int_id;
	
	if (ext_int == EXT_INT0_PIN) {
		ext_int_int0_callback = callback;
	} else
	if (ext_int == EXT_INT1_PIN) {
		ext_int_int1_callback = callback;
	} else
	if (ext_int == EXT_INT2_PIN) {
		ext_int_int2_callback = callback;
	} else
	if (ext_int == EXT_INT3_PIN) {
		ext_int_int3_callback = callback;
	} else
	if (ext_int == EXT_INT4_PIN) {
		ext_int_int4_callback = callback;
	} else
	if (ext_int == EXT_INT5_PIN) {
		ext_int_int5_callback = callback;
	} else
	if (ext_int == EXT_INT6_PIN) {
		ext_int_int6_callback = callback;
	} else
	if (ext_int == EXT_INT7_PIN) {
		ext_int_int7_callback = callback;
	} else
	if (ext_int >= PC_INT0_PIN && ext_int <=PC_INT7_PIN) {
		ext_int_pcint0_callback = callback;
	} else
	if (ext_int == PC_INT8_PIN) {
		ext_int_pcint1_callback = callback;
	} else
	{
		
	}
}