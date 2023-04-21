/*
 * 74HC595.c
 *
 *  Created on: Mar 23, 2023
 *      Author: hoanganh
 */

#include "74HC595.h"
#include "esp_log.h"
#ifdef CONFIG_USE_74HC595
HC595 *_hc595 = NULL;
HC595_Status_t HC595_AssignPin(HC595* hc595, uint16_t pin, pinName pinName)
{
	if(hc595) _hc595 = hc595;
	switch(pinName){
	case HC595_CLK:
		hc595->clk.pin = pin;
		break;
	case HC595_DS:
		hc595->ds.pin = pin;
		break;
	case HC595_LATCH:
		hc595->latch.pin = pin;
		break;
	case HC595_OE:
		hc595->oe.pin = pin;
		break;
	}
	return ESP_OK;
}

static inline uint32_t ByteMerging(const uint8_t *byte_ptr, bool LSB_FIRST) {
    uint32_t result = 0;
    for (uint8_t i = 0; i < 4; ++i) {
        uint8_t index = LSB_FIRST ? i : (3 - i);
        result |= ((uint32_t)byte_ptr[index]) << (i * 8);
    }
    return result;
}

static inline void ByteSlitting(uint32_t value, uint8_t *byte_array, bool LSB_FIRST)
{
    for (uint8_t i = 0; i < 4; i++)
    {
        uint8_t index = LSB_FIRST ? i : (3 - i);
        byte_array[index] = (uint8_t)((value >> (8 * i)) & 0xFF);
    }
}

/**
 * @brief Sends data to cascaded 74HC595 shift registers.
 * 
 * This function shifts data into multiple cascaded 74HC595 registers.
 * 
 * @param dt Pointer to an array of uint8_t data to be shifted into the registers.
 * @param n Number of cascaded 74HC595 shift registers.
 * @param ClearData clear all interal buffer if dt is NULL or clear dt
 * @return HC595_Status_t status code indicating the result of the operation.
 */
HC595_Status_t HC595_Send_Data(uint8_t *dt,uint8_t n,bool ClearData,uint8_t LSB_FIRST)
{
	if(!_hc595 && !n) return HC595_INVALID_ARG;
	if(n > HC595_MAX_CASCADE) return HC595_BEYOND_MAX_CASCADE;
	// Use internal buffer to shift out if dt is NULL
	uint8_t a; 
	if(!dt && n < 5) {
	    uint8_t Temp[4];
	    dt = Temp;
	    ByteSlitting(_hc595->data,Temp,LSB_FIRST);
	}
	a = *(dt+(n-1));
	for(int8_t i=(n*8)-1; i > -1; i--){
	    if(a&0x80) HC595_WRITE(HC595_DS,1);
	    else HC595_WRITE(HC595_DS,0);
		HC595_WRITE(HC595_CLK,0);
		HC595_WRITE(HC595_CLK,0);
		HC595_WRITE(HC595_CLK,1);
		HC595_WRITE(HC595_CLK,1);
		a <<= 1;
	    if(i%8 == 0) {
	        if(ClearData) *(dt+(i/8)) = a;
	        a = *(dt+(i/8-1));
	    }
	}
	return HC595_OK;
}

void HC595_ReturnValueToString(char *s,HC595_Status_t retVal){
    switch(retVal){
        case HC595_OK: strcpy(s,"HC595_OK"); break;
        case HC595_BEYOND_MAX_CASCADE: strcpy(s,"HC595_BEYOND_MAX_CASCADE"); break;
        case HC595_INVALID_ARG: strcpy(s,"HC595_INVALID_ARG"); break;
        case HC595_ERROR: strcpy(s,"HC595_ERROR"); break;
    }
}

void HC595_TestOutput()
{
	static uint16_t dataOriginal= 0x0200;
	static uint8_t shft=0;
	uint8_t data[2];
	shft++;
	dataOriginal>>=1;
	dataOriginal|=0x0200;
	if(shft==10) {
		shft = 0;
		dataOriginal = 0x0200;
	}
	data[0]=dataOriginal >> 8;
	data[1]=dataOriginal;
	HC595_Send_Data(data,2);
	DELAY_MS(100);
}

void HC595_SetBitOutput(uint8_t pos)
{
	_hc595->data|=(1UL<<pos);
}

void HC595_ClearBitOutput(uint8_t pos)
{
	_hc595->data&=~(1UL<<pos);
}

void HC595_SetByteOutput(uint32_t value,uint8_t pos)
{
	_hc595->data|=value<<(pos*8);
}

void HC595_ClearByteOutput(uint32_t value,uint8_t pos)
{
	_hc595->data&=~(value<<(pos*8));
}

void HC595_SetTarget(HC595 *hc595)
{
	_hc595 = hc595;
}

void HC595_TestPin(pinName pinName)
{
	HC595_WRITE(pinName,1);
	DELAY_MS(2000);
	HC595_WRITE(pinName,0);
	DELAY_MS(2000);
}

void HC595_Enable()
{
	HC595_WRITE(HC595_OE,0);
}

void HC595_Disable()
{
	HC595_WRITE(HC595_OE,1);
}
#endif

