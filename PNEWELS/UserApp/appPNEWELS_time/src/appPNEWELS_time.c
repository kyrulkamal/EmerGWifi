/*
 * appPNEWELS_time.c
 *
 * Created: 1/8/2015 11:13:42 AM
 *  Author: tay
 */ 

#include "appPNEWELS_time.h"
#include "sysTimer.h"
#include "usart_megarf.h"

int cur_year = 0;
uint8_t cur_month = 0;
uint8_t cur_day = 0;
uint8_t cur_hour = 0;
uint8_t cur_minute = 0;
uint8_t cur_second = 0;
uint8_t days_in_month[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
uint8_t time_calendar[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
static SYS_Timer_t appTimeTimer;

static void appTimeTimerHandler(SYS_Timer_t *timer)
{
	if (cur_month > 12)
	{
		cur_month = 1;
		cur_year++;
		
		if(((cur_year% 4 == false) && (cur_year%100 != false)) || (cur_year%400 == false))
		{
			days_in_month[2] = 29;
		}
		else
		{
			days_in_month[2] = 28;
		}
	}
	
	if (cur_day > days_in_month[cur_month])
	{
		cur_day = 1;
		cur_month++;
	}
	
	if (cur_hour > 23)
	{
		cur_hour = 0;
		cur_day++;
	}
	
	if (cur_minute > 59)
	{
		cur_minute = 0;
		cur_hour++;
	}
	
	if (cur_second > 59)
	{
		cur_second = 0;
		cur_minute++;
	}
	
	cur_second++;
	
	(void)timer;
}

void time_init(void)
{	
	appTimeTimer.interval = 1000;
	appTimeTimer.mode = SYS_TIMER_PERIODIC_MODE;
	appTimeTimer.handler = appTimeTimerHandler;	
}

void set_time(int year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute)
{
	SYS_TimerStop(&appTimeTimer);	
	
	cur_year = year;
	cur_month = month;
	cur_day = day;	
	cur_hour = hour;
	cur_minute = minute;
	cur_second = 0;
	
	if(((cur_year% 4 == false) && (cur_year%100 != false)) || (cur_year%400 == false))
	{
		days_in_month[2] = 29;
	}
	else
	{
		days_in_month[2] = 28;
	}
	
	SYS_TimerStart(&appTimeTimer);		
}

uint8_t *request_time(void)
{
	time_calendar[0] = (cur_year / 1000);
	time_calendar[1] = (cur_year / 100) - (time_calendar[0]*10);
	time_calendar[2] = (cur_year / 10) - (time_calendar[0]*100) - (time_calendar[1]*10);
	time_calendar[3] = (cur_year / 1) - (time_calendar[0]*1000) - (time_calendar[1]*100) - (time_calendar[2]*10);
	time_calendar[4] = 0x2C;
	time_calendar[5] = (cur_month / 10);
	time_calendar[6] = (cur_month / 1) - (time_calendar[5]*10);
	time_calendar[7] = 0x2C;
	time_calendar[8] = (cur_day / 10);
	time_calendar[9] = (cur_day / 1) - (time_calendar[8]*10);
	time_calendar[10] = 0x4E;
	time_calendar[11] = (cur_hour / 10);
	time_calendar[12] = (cur_hour / 1) - (time_calendar[11]*10);
	time_calendar[13] = 0x0A;
	time_calendar[14] = (cur_minute / 10);
	time_calendar[15] = (cur_minute / 1) - (time_calendar[14]*10);
	time_calendar[16] = 0x0A;
	time_calendar[17] = (cur_second / 10);
	time_calendar[18] = (cur_second / 1) - (time_calendar[17]*10);
	
	for (uint8_t i = 0; i < 19; i++)
	{
		time_calendar[i] = time_calendar[i] + 0x30;
	}
	
	return time_calendar;
}