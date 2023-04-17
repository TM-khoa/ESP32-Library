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

void HC595_SetTarget(HC595 *hc595)
{
	_hc595 = hc595;
}

/**
 * @brief Sends data to cascaded 74HC595 shift registers.
 * 
 * This function shifts data into multiple cascaded 74HC595 registers.
 * 
 * @param dt Pointer to an array of uint8_t data to be shifted into the registers.
 * @param n Number of cascaded 74HC595 shift registers.
 * @return HC595_Status_t status code indicating the result of the operation.
 */
HC595_Status_t HC595_Send_Data(uint8_t *dt,uint8_t n)
{
	
	if(!_hc595 && !n) return HC595_INVALID_ARG;
	for(uint8_t i=0; i<(n*8); i++)
	{
		if(*(dt+(i/8)) & 0x80) HC595_WRITE(HC595_DS,1);
		else HC595_WRITE(HC595_DS,0);
		HC595_WRITE(HC595_CLK,0);
		HC595_WRITE(HC595_CLK,0);
		HC595_WRITE(HC595_CLK,1);
		HC595_WRITE(HC595_CLK,1);
		*(dt+(i/8)) <<= 1;
	}
	HC595_WRITE(HC595_LATCH,0);
	HC595_WRITE(HC595_LATCH,1);
	return HC595_OK;
}
// (Qn output)
// n là ouput Qn cần test ( từ Q0-> Qn max)
// hc_max là số hc595 hiện có
HC595_Status_t HC595_Test_OutputPin(uint8_t n,uint8_t hc_max)
{
	// Ý tưởng : Khi muốn gửi đến 1 bit bất kỳ bắt buộc phải gửi từ Qn max trước
	// => Tính toán và gửi các dummy byte sau đó sẽ gửi bit cần gửi
	// Ví dụ cho code bên dưới : n = 13, hc_max = 5 ( có 5 HC595 => Qn max = 39, nhưng chỉ muốn test Qn với n = 13)
	// Tìm vị trí HC595 hiện tại cần test hc_x = 1 ( tính từ 0)
	// Tạo một temp_arr chứa các dummy byte và byte chứa bit cần test => nó sẽ chứa 4 phần tử (gửi 3 dummy byte và 1 byte chứa bit cần test)
	// Tìm vị trí của bit cần test trong byte chứa bit cần test n = 13 tương ứng với bit thứ 3 tính từ LSB pos_in_hc = (1+1)*8-1-13 = 2 ( không bằng 3 bởi vì sử dụng để thực hiện phép dịch trái 0x01)
	// Sau đó tạo các dummy byte bằng for như ví dụ trên thì sẽ có 3 vòng for tương đương với 3 dummy byte
	// Sau đó là byte chứa bit cần test, thực hiện dịch để tìm vị trí chính xác và dùng hàm HC595_Send_Data gửi dữ liệu

	// Kết quả: Tạo một xung từ bit cần test

	uint8_t hc_x = n/8;
	uint8_t temp_arr[hc_max - hc_x];
	uint8_t pos_in_hc = ((hc_x)+1)*8-1-n;
	for(uint8_t i = 0; i < (hc_max - hc_x-1);i++)
	{
		if(i != (hc_max - hc_x-1))
			temp_arr[i] = 0; // Gửi các dummy byte
	}
	temp_arr[hc_max - hc_x-1] =(1<<pos_in_hc);
	HC595_Send_Data(temp_arr,hc_max);
	DELAY_MS(1000);
	temp_arr[hc_max - hc_x-1] =(0<<pos_in_hc);
	HC595_Send_Data(temp_arr,hc_max);
	DELAY_MS(1000);
	return ESP_OK;
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

