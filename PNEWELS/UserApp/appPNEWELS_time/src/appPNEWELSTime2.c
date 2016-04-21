/*
 * appPNEWELSTime2.c
 *
 * Created: 3/18/2015 11:51:16 AM
 *  Author: tay
 */ 

#include "appPNEWELSTime2.h"

#include <stdbool.h>

//! Unix epoch year
#define EPOCH_YEAR 1970

//! Number of seconds in a day
#define SECS_PER_DAY 86400UL

//! Number of seconds in an hour
#define SECS_PER_HOUR 3600UL

//! Number of seconds in a minute
#define SECS_PER_MINUTE 60UL

uint32_t timestamp_internal = 0;
uint8_t count = 0;

//! Number of days in a specified month. Index 1 for leap year, else 0.
const uint8_t month[2][12] = {
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

ISR(TIMER0_OVF_vect)
{
	count++;
	
	if (count>=30)
	{
		timestamp_internal++;
		count = 0;
	}
	
}

ISR(TIMER2_OVF_vect)
{
	timestamp_internal++;		
}

void timer0_init(void)
{
	//disable global interrupt
	cpu_irq_disable();
	//disable interrupts in timer0
	TIMSK0 = (0<<OCIE0B) | (0<<OCIE0A) | (0<<TOIE0);
	//set timer0 counter initial value to 0
	TCNT0 = 0x00;
	//set timer0 mode to normal mode (timer overflow)
	TCCR0A = (0<<WGM01) | (0<<WGM00);
	//set prescaler 1024
	TCCR0B = (1 << CS02) | (0 << CS01) | (1 << CS00);
	//clear timer0 overflow flag
	TIFR0 = (0<<OCF0B) | (0<<OCF0A) | (0<<TOV0);
	//enable timer0 interrupt
	TIMSK0 |= (1 << TOIE0);
	//enable global interrupt
	SYS_EnableInterrupts();
}

void timer2_init(void)
{
	//disable global interrupt
	cpu_irq_disable();
	//disable interrupts in timer2
	TIMSK2 = (0<<OCIE2B) | (0<<OCIE2A) | (0<<TOIE2);
	//set using TOSC crystal
    ASSR |= (1 << AS2);
	//set timer2 counter initial value to 0
	TCNT2 = 0x00;
	//set timer2 mode to normal mode (timer overflow)
	TCCR2A = (0<<WGM21) | (0<<WGM20);
	//set prescaler 128
	TCCR2B = (1 << CS22) | (0 << CS21) | (1 << CS20); 
	
	while((ASSR & ((1<<TCR2BUB) | (1<<TCR2AUB))) == 3)
	{
		//add in error handler here
	}
	
	//set timer2 counter initial value to 0
	TCNT2 = 0x00;
	//set timer2 mode to normal mode (timer overflow)
	TCCR2A = (0<<WGM21) | (0<<WGM20);
	//set prescaler 1024
	TCCR2B = (1 << CS22) | (0 << CS21) | (1 << CS20);
	//clear timer2 overflow flag
	TIFR2 = (0<<OCF2B) | (0<<OCF2A) | (0<<TOV2);
	//enable timer2 interrupt
	TIMSK2 |= (1 << TOIE2);
	//enable global interrupt
	SYS_EnableInterrupts();
}

/**
 * \internal
 * \brief Check if a year is a leap year
 *
 * Returns true or false depending if the input year is a leap year or not.
 *
 * \param year the year in format YYYY to check for leap year or not
 *
 * \retval true  if the year is a leap year
 * \retval false if the year is not a leap year
 */
static bool calendar_leapyear(uint16_t year)
{
	if(!((year) % 4) && (((year) % 100) || !((year) % 400))) {
		return true;
	} else {
		return false;
	}
}

/**
 * \internal
 * \brief Find number of days in a year
 *
 * Returns the number of days in a year, depending if the input is leap year
 * or not.
 *
 * \param year the year in format YYYY to check number of days
 *
 * \retval 366 if the year is a leap year
 * \retval 365 if the year is not a leap year
 */
static uint16_t calendar_yearsize(uint16_t year)
{
	if (calendar_leapyear(year)) {
		return 366;
	} else {
		return 365;
	}
}

/**
 * \brief Convert a UNIX timestamp to a date
 *
 * Finds the corresponding date and time for a UNIX timestamp.
 *
 * \param timestamp  UNIX timestamp
 * \param date_out   Date to store result
 *
 */
void calendar_timestamp_to_date(struct calendar_date *date_out)
{
	uint32_t day_number;
	uint32_t day_clock;

	date_out->year = EPOCH_YEAR;
	date_out->month = 0;

	day_clock = timestamp_internal % SECS_PER_DAY;
	day_number = timestamp_internal / SECS_PER_DAY;

	date_out->second = day_clock % SECS_PER_MINUTE;
	date_out->minute = (day_clock % SECS_PER_HOUR) / SECS_PER_MINUTE;
	date_out->hour = day_clock / SECS_PER_HOUR;
	date_out->dayofweek = (day_number + 4) % 7;

	while (day_number >= calendar_yearsize(date_out->year)) {
		day_number -= calendar_yearsize(date_out->year);
		date_out->year++;
	}

	while (day_number >=
			month[calendar_leapyear(date_out->year)][date_out->month]) {
		day_number -= month[calendar_leapyear(date_out->year)][date_out->month];
		date_out->month++;
	}
	date_out->date = day_number;
}

void set_timestamp(uint32_t timestamp)
{
	timestamp_internal = timestamp;
}

uint32_t get_timestamp(void)
{
	return timestamp_internal;
}