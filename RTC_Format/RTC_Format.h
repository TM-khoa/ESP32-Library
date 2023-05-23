/*
 * RTC_Format.h
 *
 *  Created on: May 12, 2023
 *      Author: SpiritBoi
 */

#ifndef INC_RTC_FORMAT_H_
#define INC_RTC_FORMAT_H_

// #include "main.h"
#include "string.h"
#include "stdio.h"

typedef struct RTC_t
{
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t weekday;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
}RTC_t;

RTC_t RTC_GetTimeFromString(char *s);
void RTC_PackTimeToString(RTC_t t, char* s);

#endif /* INC_RTC_FORMAT_H_ */
