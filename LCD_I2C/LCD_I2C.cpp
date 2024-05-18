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
#include "LCD_I2C.hpp"
#ifdef CONFIG_USE_LCDI2C
#if defined(CONFIG_USE_CPP)
static const char *TAG = "LCD_I2C";
ClassPCF8574 _PCF8574;
hd44780_t _LCD;

esp_err_t ClassLCDI2C::testPCF(uint8_t value)
{
    return _PCF8574.write(value);
}

static inline esp_err_t  PCF_send_lcd_data(const hd44780_t *lcd, uint8_t data)
{
    return _PCF8574.write(data);
}

esp_err_t ClassLCDI2C::begin()
{
    esp_err_t err = ESP_OK;
    err =_PCF8574.begin((gpio_num_t)I2C_MASTER_SDA_IO,
                        (gpio_num_t)I2C_MASTER_SCL_IO,
                        I2C_NUM_0,
                        0,false);
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