/**
 * @file LCD_I2C.cpp
 * @author SpiritBoi (minhkhoa9a31314@gmail.com)
 * @brief LCD I2C driver using PCF8574 GPIO expander with I2C protocol to drive LCD
 * @version 0.1
 * @date 2022-11-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "LCD_I2C.h"
static const char *TAG = "LCD_I2C";

#define LCD_RS          0
#define LCD_RW          1
#define LCD_E           2
#define LCD_D4          3
#define LCD_D5          4
#define LCD_D6          5
#define LCD_D7          6
#define LCD_BACKLIGHT   7

ClassPCF8574 _PCF8574;
hd44780_t _LCD;



esp_err_t PCF_send_lcd_data(const hd44780_t *lcd, uint8_t data)
{
    return _PCF8574.write(data);
}

esp_err_t ClassLCDI2C::begin()
{
    i2cdev_init();
    esp_err_t err = ESP_OK;
    err =_PCF8574.begin(    GPIO_NUM_18,
                            GPIO_NUM_19,
                            0,
                            0x20);

    _LCD.pins.d4 = LCD_D4;
    _LCD.pins.d5 = LCD_D5;
    _LCD.pins.d6 = LCD_D6;
    _LCD.pins.d7 = LCD_D7;
    _LCD.pins.rs = LCD_RS;
    _LCD.pins.e = LCD_E;
    _LCD.pins.bl = LCD_BACKLIGHT;
    _LCD.write_cb = PCF_send_lcd_data;
    _LCD.font = HD44780_FONT_5X8;
    _LCD.lines = 4;
    hd44780_init(&_LCD);
    return err;
}



esp_err_t ClassLCDI2C::print(const char *S, uint8_t x, uint8_t y)
{
    esp_err_t err = hd44780_gotoxy(&_LCD,x,y);
    err = hd44780_puts(&_LCD,S);
    return err;
}

esp_err_t ClassLCDI2C::TurnOnBackLight(){return hd44780_switch_backlight(&_LCD,1);}
esp_err_t ClassLCDI2C::TurnOffBackLight() {return hd44780_switch_backlight(&_LCD,0);}



esp_err_t ClassLCDI2C::clear(){return hd44780_clear(&_LCD);}
