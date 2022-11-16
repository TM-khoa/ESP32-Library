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


ClassPCF8574 _PCF8574;
hd44780_t lcd;
static esp_err_t pcf8574_send_lcd_data(const hd44780_t *lcd, uint8_t data){
    return _PCF8574.write_port(data);
}

esp_err_t ClassLCDI2C::print(const char *S, uint8_t x, uint8_t y){
    esp_err_t err = hd44780_gotoxy(&this->lcd,x,y);
    err = hd44780_puts(&this->lcd,S);
    return err;
}

esp_err_t ClassLCDI2C::TurnOnBackLight(){return hd44780_switch_backlight(&this->lcd,1);}
esp_err_t ClassLCDI2C::TurnOffBackLight() {return hd44780_switch_backlight(&this->lcd,0);}

esp_err_t ClassLCDI2C::begin(){
    esp_err_t err =     &this->lcd_conf 
                    &&  &this->pcf_conf 
                    &&  this->lcd_conf.IsHD44780 ? 
                    ESP_OK : ESP_ERR_INVALID_ARG;
    if(err != ESP_OK) {
            ESP_LOGE(TAG,"Not confirm LCD type or lcd and pcf configs are NULL");
            return err;
        }

    err = this -> PCF8574.begin(    this->pcf_conf.sda_pin, 
                                    this->pcf_conf.scl_pin, 
                                    this->pcf_conf.port, 
                                    this->pcf_conf.Address);

    this -> lcd.pins.d4 = this->lcd_conf.d4;
    this -> lcd.pins.d5 = this->lcd_conf.d5;
    this -> lcd.pins.d6 = this->lcd_conf.d6;
    this -> lcd.pins.d7 = this->lcd_conf.d7;
    this -> lcd.pins.e = this->lcd_conf.e;
    this -> lcd.pins.rs = this->lcd_conf.rs;
    // use callback to send data to LCD by I2C GPIO expander
    this -> lcd.write_cb = pcf8574_send_lcd_data; 
    this -> lcd.font = this->lcd_conf.font;
    this -> lcd.lines = this->lcd_conf.lines;
    this -> lcd.pins.bl = this->lcd_conf.backlight;
    hd44780_init(&this->lcd);
    return err;
}

esp_err_t ClassLCDI2C::clear(){
    return hd44780_clear(&this->lcd);
}
