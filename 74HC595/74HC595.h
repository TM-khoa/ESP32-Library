/*
 * 74HC595.h
 *
 *  Created on: Mar 23, 2023
 *      Author: hoanganh
 */


#ifndef INC_74HC595_H_
#define INC_74HC595_H_

#include "../LibraryConfig.h"
#include "driver/gpio.h"
#include "string.h"
#ifdef CONFIG_USE_74HC595

#define HC595_MAX_CASCADE 4
#define DELAY_MS(X) (vTaskDelay(X/portTICK_PERIOD_MS))
#define HC595_SET_OUTPUT(X)(1<<X)
#define HC595_SET_OUTPUT(X)(1<<X)

#define latchPin _hc595->latch.pin
#define clkPin _hc595->clk.pin
#define dsPin _hc595->ds.pin
#define oePin _hc595->oe.pin
#define HC595_WRITE(PIN,LOGIC) ( 	((PIN) == HC595_LATCH)  ? 	gpio_set_level(latchPin,(LOGIC)) 	:	\
									((PIN) == HC595_CLK) 	?	gpio_set_level(clkPin,(LOGIC)) 		: 	\
									((PIN) == HC595_OE) 	?	gpio_set_level(oePin,(LOGIC)) 		: 	\
									((PIN) == HC595_DS) 	?	gpio_set_level(dsPin,(LOGIC)) 		: 0	\
								)

typedef struct {
	uint16_t pin;
}pinConfig;

typedef struct HC595{
	pinConfig ds;
	pinConfig clk;
	pinConfig latch;
	pinConfig oe;
	uint32_t data;
}HC595;

typedef enum{
	HC595_LATCH,
	HC595_CLK,
	HC595_DS,
	HC595_OE
}pinName;


typedef enum{
	HC595_OK,
	HC595_ERROR,
	HC595_INVALID_ARG,
	HC595_BEYOND_MAX_CASCADE,
}HC595_Status_t;

HC595_Status_t HC595_ShiftOut(uint8_t *dt,uint8_t n,uint8_t Device0GetLSBByte);
HC595_Status_t HC595_AssignPin(HC595* dev,uint16_t pin, pinName pinName);
HC595_Status_t HC595_PrintOutputToConsole(uint8_t *dt,uint8_t n,uint8_t Device0GetLSBByte,void (*pPrint)(char *s));
void HC595_PrintConsole(char*s);
void HC595_SetBitOutput(uint8_t pos);
void HC595_ClearBitOutput(uint8_t pos);
void HC595_SetByteOutput(uint32_t value);
void HC595_ClearByteOutput(uint32_t value);
void HC595_SetTarget(HC595 *dev);
void HC595_TestPin(pinName pin);
void HC595_DisableOutput();
void HC595_EnableOutput();
void HC595_TestOutput();
#endif /* INC_74HC595_H_ */
#endif

