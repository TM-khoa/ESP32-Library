/*
 * RTC_Format.c
 *
 *  Created on: May 12, 2023
 *      Author: SpiritBoi
 */

#include "RTC_Format.h"

#ifdef __cplusplus
extern "C" {
#endif

RTC_t RTC_GetTimeFromString(char *s) {
	int hour, minute, second, day, month, year;
	RTC_t t;
	uint8_t itemConverted = 0;
	itemConverted = sscanf(s, "%*s %d/%d/%d %d:%d:%d", &day, &month, &year, &hour, &minute, &second);

	if (year > 99)
		t.year = 255;
	else
		t.year = year;

	if (month < 1 || month > 12)
		t.month = 255;
	else
		t.month = month;

	if (day < 1 || day > 31)
		t.day = 255;
	else
		t.day = day;

	if (hour > 23)
		t.hour = 255;
	else
		t.hour = hour;

	if (minute > 59)
		t.minute = 255;
	else
		t.minute = minute;

	if (second > 59)
		t.second = 255;
	else
		t.second = second;

	if (itemConverted != 6) {
		t.year = 255;
		t.month = 255;
		t.day = 255;
		t.hour = 255;
		t.minute = 255;
		t.second = 255;
	}
	return t;
}

uint8_t RTC_ConvertTickElapsedToSecond(uint16_t input_RTC_TickCount) {
	return input_RTC_TickCount % 60;
}
uint8_t RTC_ConvertTickElapsedToMinute(uint16_t input_RTC_TickCount) {
	return ((input_RTC_TickCount / 60) - (input_RTC_TickCount / 3600) * 60);
}
uint8_t RTC_ConvertTickElapsedToHour(uint16_t input_RTC_TickCount) {
	return input_RTC_TickCount / 3600;
}

void RTC_PackTimeToString(RTC_t t, char *s) {
	sprintf(s, "%d/%d/%d %d:%d:%d", t.day, t.month, t.year, t.hour, t.minute, t.second);
}

#ifdef __cplusplus
}
#endif