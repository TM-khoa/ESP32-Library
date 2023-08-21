/*
 * RTC_Format.c
 *
 *  Created on: May 12, 2023
 *      Author: SpiritBoi
 */

#include "RTC_Format.h"

RTC_t RTC_GetTimeFromString(char *s)
{
	int hour,minute,second,day,month,year;
	RTC_t t;
	uint8_t itemConverted = 0;
	itemConverted = sscanf(s,"%*s %d/%d/%d %d:%d:%d",&day,&month,&year,&hour,&minute,&second);
	if(year > 99) t.year = -1;
	else t.year = year;
	if(month < 1 || month > 12) t.month = -1;
	else t.month = month;
	if(day < 1 || day > 31) t.day = -1;
	else t.day = day;
	if(hour > 23) t.hour = -1;
	else t.hour = hour;
	if(minute > 59) t.minute = -1;
	else t.minute = minute;
	if(second > 59) t.second = -1;
	else t.second = second;
	if(itemConverted != 6)
	{
		t.year = -1;
		t.month = -1;
		t.day = -1;
		t.hour = -1;
		t.minute = -1;
		t.second = -1;
	}
	return t;
}



void RTC_PackTimeToString(RTC_t t, char* s)
{
	sprintf(s,"Time: %d/%d/%d %d:%d:%d",t.day,t.month,t.year,t.hour,t.minute,t.second);
}

void RTC_SetSecond(uint8_t sec){
	if(sec > 59) return;
	  
}
