/*
 * adc.h
 *
 * Created: 1/5/2015 4:09:25 PM
 *  Author: tay
 */ 


#ifndef ADC_H_
#define ADC_H_


#include "adc_megarf.h"
#include <stdbool.h> 
#include "pne_rf_processor.h"
#include "pne_appDelay.h"

/*  ADC prescaler mask */
#define ADC_PRESCALER_MASK (1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0)

/*  ADC voltage reference mask */
#define ADC_VREF_MASK      (1 << REFS1 | 1 << REFS0)

/*  ADC MUX mask */
#define ADC_MUX_MASK    (1 << MUX4 | 1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0)

/*  ADC Auto Trigger mask */
#define ADC_ADTS_SOURCE_MASK     (1 << ADTS2 | 1 << ADTS1 | 1 << ADTS0)

/*  \brief ADC result adjustment */
enum adc_adjustment {
	/*  Right adjust result */
	ADC_ADJUSTMENT_RIGHT = 0,
	/*  Left adjust result */
	ADC_ADJUSTMENT_LEFT = (1 << ADLAR),
};

/**
 * \brief ADC prescaler settings
 *
 * Available ADC prescaler settings
 */
enum adc_prescaler {
	/*  Divide system clock by 2 */
	ADC_PRESCALER_DIV2 = (0x1 << ADPS0),
	/*  Divide system clock by 4 */
	ADC_PRESCALER_DIV4 = (0x2 << ADPS0),
	/*  Divide system clock by 8 */
	ADC_PRESCALER_DIV8 = (0x3 << ADPS0),
	/*  Divide system clock by 16 */
	ADC_PRESCALER_DIV16 = (0x4 << ADPS0),
	/*  Divide system clock by 32 */
	ADC_PRESCALER_DIV32 = (0x5 << ADPS0),
	/*  Divide system clock by 64 */
	ADC_PRESCALER_DIV64 = (0x6 << ADPS0),
	/*  Divide system clock by 128 */
	ADC_PRESCALER_DIV128 = (0x7 << ADPS0),
};

/**
 * \brief ADC Auto Trigger sources
 *
 * Available trigger sources for ADC Auto Trigger
 */
enum adc_auto_trigger_source {
	/*  Free Running mode */
	ADC_AUTOTRIGGER_SOURCE_FREERUNNING = (0x0 << ADTS0),
	/*  Analog Comparator */
	ADC_AUTOTRIGGER_SOURCE_ANALOG_COMPARATOR = (0x1 << ADTS0),
	/*  External Interrupt Request 0 */
	ADC_AUTOTRIGGER_SOURCE_EIR0 = (0x2 << ADTS0),
	/*  Timer/Counter0 Compare Match */
	ADC_AUTOTRIGGER_SOURCE_TC0_COMPARE = (0x3 <<  ADTS0),
	/*  Timer/Counter0 Overflow */
	ADC_AUTOTRIGGER_SOURCE_TC0_OVERFLOW = (0x4 << ADTS0),
	/*  Timer/Counter1 Compare Match B */
	ADC_AUTOTRIGGER_SOURCE_TC1_COMPARE_B = (0x5 << ADTS0),
	/*  Timer/Counter1 Overflow */
	ADC_AUTOTRIGGER_SOURCE_TC1_OVERFLOW = (0x6 << ADTS0),
	/*  Timer/Counter1 Capture Event */
	ADC_AUTOTRIGGER_SOURCE_TC1_CAPTURE = (0x7 << ADTS0),
};

/**
 * \enum adc_mux_setting
 *
 * \brief ADC input MUX settings
 *
 * MUX settings for ADC input \note If using 10X or 200X gain only 2.56V should
 * be used as internal voltage reference.
 *
 * \note The doxygen documentation contains MUX settings for multiple
 * devices. check the device datasheet or family header files to find what
 * options are available on your device.
 */

/**
 * \enum adc_voltage_reference
 *
 * \brief ADC voltage references
 *
 * Available ADC voltage references
 *
 */

/**
 * \brief ADC initialization. Set ADC prescaler and enable the ADC
 *
 * \param prescaler   ADC clock prescaler
 */
static inline void adc_init(enum adc_prescaler prescaler)
{
	ADCSRA = (uint8_t)prescaler | (1 << ADEN);
}

/*  \brief Return MSB (ADCH) of ADC data register */
static inline uint8_t adc_get_data_8bit(void)
{
	return ADCH;
}

/*  \brief Return ADC data register */
static inline uint16_t adc_get_data_10bit(void)
{
	return ADC;
}

/*  \brief Enable ADC */
static inline void adc_enable(void)
{
	ADCSRA |= (1 << ADEN);
}

/*  \brief Disable ADC */
static inline void adc_disable(void)
{
	ADCSRA &= ~(1 << ADEN);
}

/*  \brief Start ADC conversion */
static inline void adc_start_conversion(void)
{
	ADCSRA |= (1 << ADSC);
}

/**
 * \brief Poll ADC conversion complete interrupt flag
 *
 * \return false if conversion not complete
 */
static inline bool adc_conversion_is_complete(void)
{
	return ADCSRA & (1 << ADIF);
}

/*  \brief Enable ADC interrupt */
static inline void adc_enable_interrupt(void)
{
	ADCSRA |= (1 << ADIE);
}

/*  \brief Disable ADC interrupt */
static inline void adc_disable_interrupt(void)
{
	ADCSRA &= ~(1 << ADIE);
}

/**
 * \brief Set ADC Auto Trigger source
 *
 * \param trg ADC Auto Trigger source
 */
static inline void adc_set_autotrigger_source(enum adc_auto_trigger_source trg)
{
	uint8_t temp;

	temp = (ADC_ADTS_REG & ~(ADC_ADTS_SOURCE_MASK));
	temp |= (uint8_t)trg;
	ADC_ADTS_REG = temp;
}

/*  \brief Enable ADC Auto Trigger */
static inline void adc_enable_autotrigger(void)
{
	ADCSRA |= (1 << ADATE);
}

/*  \brief Disable ADC Auto Trigger */
static inline void adc_disable_autotrigger(void)
{
	ADCSRA &= ~(1 << ADATE);
}

/**
 * \brief Set the ADC voltage reference.
 *
 * An example demonstrating the usage:
 * \code
	adc_set_voltage_reference(ADC_VREF_1V1);
\endcode
 *
 * \param vref ADC voltage reference
 *
 * \note The first ADC conversion result after switching reference voltage
 * source may be inaccurate, and the user is advised to discard this result.
 */
static inline void adc_set_voltage_reference(enum adc_voltage_reference vref)
{
	uint8_t temp;

	temp = (ADMUX & ~(ADC_VREF_MASK));
	temp |= (uint8_t)vref;
	ADMUX = temp;
}

/**
 * \brief Set ADC clock prescaler
 *
 * \note The ADC clock must not exceed the maximum speed of 1MHz
 *
 * A small example demonstrating the usage:
 * \code
	adc_set_prescaler(ADC_PRESCALER_DIV64)
\endcode
 *
 * \param prescaler ADC clock prescaler
 */
static inline void adc_set_prescaler(enum adc_prescaler prescaler)
{
	uint8_t temp;

	temp = (ADCSRA & ~(ADC_PRESCALER_MASK));
	temp |= (uint8_t)prescaler;
	ADCSRA = temp;
}

/**
 * \brief Select source from ADC input mux
 *
 * A small example demonstrating the usage:
 * \code
	adc_set_mux(ADC_MUX_ADC0);
\endcode
 *
 * \param input ADC channel
 */
static inline void adc_set_mux(enum adc_mux_setting input)
{
	uint8_t temp;

	temp = (ADMUX & ~(ADC_MUX_MASK));

#if defined(MUX5)
	/* if bit 6 (MUX5) is set: */
	if (input & (1 << 5)) {
		ADCSRB = (1 << MUX5);
	} else {
		ADCSRB &= ~(1 << MUX5);
	}

	/* mask out MUX5 for ADMUX. */
	temp |= ((uint8_t)input & ADC_MUX_MASK);
#else
	temp |= (uint8_t)input;
#endif
	ADMUX = temp;
}

/**
 * \brief Blocking ADC conversion with 8-bit result.
 *
 * An example demonstrating the minimum initialization needed to perform
 * a conversion:
 *
 * \code
	 // Initialize ADC
	 adc_init(ADC_PRESCALER_DIV32);

	 analog_val = adc_read_8bit(ADC_MUX_ADC0, ADC_VREF_1V1);
\endcode
 *
 * \param input   ADC input mux selection.
 *
 * \param vref    ADC voltage reference
 *
 * \return Conversion result as an unsigned 8-bit integer
 *
 * \note The first ADC conversion result after switching reference voltage
 * source may be inaccurate, and the user is advised to discard this result.
 * this function is not interrupt safe.
 */
static inline uint8_t adc_read_8bit(enum adc_mux_setting input,	\
		enum adc_voltage_reference vref)
{
	
//uint8_t count = 0;

#if defined(MUX5)
	/* if bit 6 (MUX5) is set: */
	if (input & (1 << 5)) {
		ADCSRB = (1 << MUX5);
	} else {
		ADCSRB &= ~(1 << MUX5);
	}

	ADMUX
		= ((uint8_t)input &
			ADC_MUX_MASK) | (uint8_t)vref | ADC_ADJUSTMENT_LEFT;
#else
	ADMUX = (uint8_t)input | (uint8_t)vref | ADC_ADJUSTMENT_LEFT;
#endif
	ADCSRA |= (1 << ADSC);

	while ((ADCSRA & (1 << ADSC))) {
		/* wait for conversion complete */
		//pne_delayms(1);
		//count++;
		//if (count == 10)
		//{
			//count = 0;
			//fatal_error_to_rf(adc_fatal_error);
			//break;
		//}
	}

	return ADCH;
}

/**
 * \brief Blocking ADC conversion with 10-bit result.
 *
 * An example demonstrating the minimum initialization needed to perform
 * a conversion:
 *
 * \code
	 // Initialize and enable ADC
	 adc_init(ADC_PRESCALER_DIV64);

	 analog_val = adc_read_10bit(ADC_MUX_ADC0, ADC_VREF_1V1);
\endcode
 *
 * \param input   ADC input mux selection and voltage reference
 *
 * \param vref    ADC voltage reference
 *
 * \return Conversion result as an unsigned 16-bit integer
 *
 * \note The first ADC conversion result after switching reference voltage
 * source may be inaccurate, and the user is advised to discard this result.
 * this function is not interrupt safe
 */
static inline uint16_t adc_read_10bit(enum adc_mux_setting input, \
		enum adc_voltage_reference vref)
{
#if defined(MUX5)
	/* if bit 6 (MUX5) is set: */
	if (input & (1 << 5)) {
		ADCSRB = (1 << MUX5);
	} else {
		ADCSRB &= ~(1 << MUX5);
	}

	ADMUX = ((uint8_t)input & ADC_MUX_MASK) | (uint8_t)vref;
#else
	ADMUX = (uint8_t)input | (uint8_t)vref;
#endif
	ADCSRA |= (1 << ADSC);

	while ((ADCSRA & (1 << ADSC))) {
		/* wait for conversion complete */
	}
	return ADC;
}

/**
 * \name Optimized functions for register accesses
 * \details These functions are for cases where you do not want the overhead
 * of masking out bitfields before updating the register value. These will
 * only do a write to the register instead of a read-modify-write.
 */
/* @{ */

/**
 * \brief Set voltage reference, mux and adjustment bits in admux register
 *
 * \note This does not handle the MUX5 bit if this is available,
 * only the MUX bits in ADMUX.
 *
 * Example usage:
 *
 * \code
	adc_write_admux(ADC_MUX_ADC0 | ADC_VREF_1V1 | ADC_ADJUSTMENT_LEFT);
\endcode
 *
 * \param regval   ADC input mux selection and voltage reference
 */
static inline void adc_set_admux(uint8_t regval)
{
	ADMUX = regval;
}

#if defined(DIDR) || defined(DIDR0)

/**
 * \brief Disable digital input buffers on ADC pin 0-7
 *
 * disable the digitial input buffer on ADC pin 0-7. This will
 * reduce the power consumption if you are not using the ADC pins for
 * digital IO.
 *
 * \code
	// disable ADC pins 0-7.
	adc_disable_digital_inputs(0xFF)
\endcode
 *
 * \param pinmask   ADC pin bitmask
 */
static inline void adc_disable_digital_inputs(uint8_t pinmask)
{
#if defined(DIDR0)
	DIDR0 = pinmask;
#elif defined(DIDR)
	DIDR = pinmask;
#endif
}

#if MEGA_XX0_1 || MEGA_RF

/**
 * \brief Disable digital input buffers on ADC pin 8-15
 *
 * Disable the digitial input buffer on ADC pin 8-15. This will
 * reduce the power consumption if you are not using the ADC pins for
 * digital IO.
 *
 * \code
	// disable ADC pins 8-15.
	adc_disable_digital_inputs_high(0xFF)
\endcode
 *
 * \param pinmask   ADC pin bitmask
 */
static inline void adc_disable_digital_inputs_high(uint8_t pinmask)
{
	DIDR2 = pinmask;
}

#endif /* MEGA_XX0_1 || MEGA_RF */

#endif /* DIDR */

//@}

/** @} */



#endif /* ADC_H_ */