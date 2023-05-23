/*
 * RTC_Format.c
 *
 *  Created on: May 12, 2023
 *      Author: SpiritBoi
 */

#include "RTC_Format.h"

RTC_t RTC_GetTimeFromString(char *s)
{
	unsigned int hour,minute,second,day,month,year;
	sscanf(s,"%u/%u/%u %u:%u:%u",&day,&month,&year,&hour,&minute,&second);
	RTC_t t = {
	.hour = hour,
	.minute = minute,
	.second = second,
	.day = day,
	.month = month,
	.year = year,
	};
	return t;
}
void RTC_PackTimeToString(RTC_t t, char* s)
{
	sprintf(s,"%u/%u/%u %u:%u:%u",t.day,t.month,t.year,t.hour,t.minute,t.second);
}
