/*
 * appPNEWELSTime2.h
 *
 * Created: 3/18/2015 11:50:39 AM
 *  Author: tay
 */ 

#include "sysTypes.h"

#ifndef APPPNEWELSTIME2_H_
#define APPPNEWELSTIME2_H_


struct calendar_date {
	uint8_t second;        //!< 0-59
	uint8_t minute;        //!< 0-59
	uint8_t hour;          //!< 0-23
	uint8_t date;          //!< 0-30 \note First day of month is 0, not 1.
	uint8_t month;         //!< 0 January - 11 December
	uint16_t year;         //!< 1970-2105
	uint8_t dayofweek;     //!< 0 Sunday  - 6 Saturday
};

void timer0_init(void);
void timer2_init(void);
void calendar_timestamp_to_date(struct calendar_date *date_out);
void set_timestamp(uint32_t timestamp);
uint32_t get_timestamp(void);


#endif /* APPPNEWELSTIME2_H_ */