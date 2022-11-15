#ifndef __ManHinhLCD_H__
#define __ManHinhLCD_H__

#include "esp_log.h"
#include "driver/gpio.h"

#include <stdbool.h>
#include <stdlib.h>
#include <i2cdev.h>
#include <esp_err.h>

#include <iostream>
using namespace std;

#include "PCF8574.h"
#include "hd44780.h"

#define LCD_I2C_ADDRESS         0x20

#define LCD_PCF8574_RS          0
#define LCD_PCF8574_RW          1
#define LCD_PCF8574_E           2
#define LCD_PCF8574_D4          3
#define LCD_PCF8574_D5          4
#define LCD_PCF8574_D6          5
#define LCD_PCF8574_D7          6
#define LCD_PCF8574_BACKLIGHT   7

// class ClassLCDI2C{
// private:
//     ClassPCF8574 PCF8574;
//     hd44780_t lcd;
// public:
//     esp_err_t begin(gpio_num_t sda_gpio, gpio_num_t scl_gpio, i2c_port_t port, uint8_t Address);
// };

esp_err_t KhoiTaoLCD(gpio_num_t sda_gpio, gpio_num_t scl_gpio, i2c_port_t port, uint8_t Address);
esp_err_t HuyKhoiTaoLCD();


#endif // __ManHinhLCD_H__