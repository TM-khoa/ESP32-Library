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
    uint8_t d4;
    uint8_t d5;
    uint8_t d6;
    uint8_t d7;
    uint8_t e;
    uint8_t rs;
    uint8_t backlight;
    hd44780_font_t font;
    uint8_t lines;
    bool IsHD44780;
};

class ClassLCDI2C{
private:
    
public:
    
    esp_err_t begin();
    esp_err_t clear();
    esp_err_t TurnOnBackLight();
    esp_err_t TurnOffBackLight();
    esp_err_t print(const char *S, uint8_t x, uint8_t y);
    // esp_err_t configPin(LCD_conf_t *l,PCF8574_conf_t *pcf);
    // esp_err_t pLCD_cb(const hd44780_t *lcd, uint8_t data);
};




#endif // LCD_I2C_H