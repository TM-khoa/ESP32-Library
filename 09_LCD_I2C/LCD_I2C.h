#ifndef _LCD_I2C_H_
#define _LCD_I2C_H_

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

struct PCF8574_conf_t{
    gpio_num_t scl_pin;
    gpio_num_t sda_pin;
    i2c_port_t port;
    uint8_t Address;
};

struct LCD_conf_t{
    gpio_num_t d4;
    gpio_num_t d5;
    gpio_num_t d6;
    gpio_num_t d7;
    gpio_num_t e;
    gpio_num_t rs;
    gpio_num_t backlight;
    hd44780_font_t font;
    uint8_t lines;
    bool IsHD44780;
};

class ClassLCDI2C{
private:
    ClassPCF8574 PCF8574;
    hd44780_t lcd;
public:
    LCD_conf_t lcd_conf;
    PCF8574_conf_t pcf_conf;
    esp_err_t begin();
    esp_err_t clear();
    esp_err_t TurnOnBackLight();
    esp_err_t TurnOffBackLight();
    esp_err_t print(const char *S, uint8_t x, uint8_t y);
};




#endif // LCD_I2C_H