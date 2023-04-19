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
#ifdef CONFIG_USE_LCDI2C
#if defined(CONFIG_USE_CPP)
static const char *TAG = "LCD_I2C";
ClassPCF8574 _PCF8574;
hd44780_t _LCD;

esp_err_t ClassLCDI2C::testPCF()
{
    return _PCF8574.write(0x55);
}

static inline esp_err_t  PCF_send_lcd_data(const hd44780_t *lcd, uint8_t data)
{
    return _PCF8574.write(data);
}

esp_err_t ClassLCDI2C::begin(i2c_dev_t *dev)
{
    i2cdev_init();
    
    esp_err_t err = ESP_OK;
    err =_PCF8574.begin((gpio_num_t)dev->cfg.sda_io_num,
                        (gpio_num_t)dev->cfg.scl_io_num,
                        dev->port,
                        dev->addr,true);
    _LCD.pins.d4 = LCD_PCF8574_D4;
    _LCD.pins.d5 = LCD_PCF8574_D5;
    _LCD.pins.d6 = LCD_PCF8574_D6;
    _LCD.pins.d7 = LCD_PCF8574_D7;
    _LCD.pins.rs = LCD_PCF8574_RS;
    _LCD.pins.e =  LCD_PCF8574_E;
    _LCD.pins.bl = LCD_PCF8574_BACKLIGHT;
    _LCD.write_cb = PCF_send_lcd_data;
    _LCD.font = HD44780_FONT_5X8;
    _LCD.lines = LCD_ROWS;
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
#endif /*CONFIG_USE_CPP*/
#endif /*CONFIG_USE_LCDI2C*/