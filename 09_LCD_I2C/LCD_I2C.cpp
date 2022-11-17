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


// ClassPCF8574 _PCF8574;
// hd44780_t lcd;
esp_err_t pLCD_cb(const hd44780_t *lcd, uint8_t data)
{    
    return ESP_OK;
}

esp_err_t ClassLCDI2C::PCFWrite(uint8_t data)
{
    return this->PCF8574.write_port(data);
}

esp_err_t ClassLCDI2C::print(const char *S, uint8_t x, uint8_t y)
{
    esp_err_t err = hd44780_gotoxy(&this->lcd,x,y);
    err = hd44780_puts(&this->lcd,S);
    
    return err;
}

esp_err_t ClassLCDI2C::TurnOnBackLight(){return hd44780_switch_backlight(&this->lcd,1);}
esp_err_t ClassLCDI2C::TurnOffBackLight() {return hd44780_switch_backlight(&this->lcd,0);}

esp_err_t ClassLCDI2C::begin(PCF8574_conf_t *cfgPCF, LCD_conf_t *cfgLCD)
{
    esp_err_t err = ESP_OK;
             err =     cfgLCD 
                    &&  cfgPCF 
                    &&  cfgLCD->IsHD44780 ? 
                    ESP_OK : ESP_ERR_INVALID_ARG;
    if(err != ESP_OK) {
            ESP_LOGE(TAG,"Not confirm LCD type or lcd and pcf configs are NULL");
            return err;
    }
    // i2c_config_t conf_i2cio =
    // {
    //     .mode = I2C_MODE_MASTER,
    //     .sda_io_num = cfgPCF->sda_pin,
    //     .scl_io_num = cfgPCF->scl_pin,
    //     .sda_pullup_en = GPIO_PULLUP_ENABLE,
    //     .scl_pullup_en = GPIO_PULLUP_ENABLE,
    // };
    //     conf_i2cio.master.clk_speed = 100000,
    // i2c_param_config(0,&conf_i2cio);
    // i2c_driver_install(0,conf_i2cio.mode,0,0,0);
    // if(err != ESP_OK){
    //     ESP_LOGE(TAG,"config master error %d",err);
    //     return err;
    // // }
    // err = this -> PCF8574.begin(    cfgPCF->sda_pin, 
    //                                 cfgPCF->scl_pin, 
    //                                 cfgPCF->port, 
    //                                 cfgPCF->Address);

    err = this -> PCF8574.begin(    GPIO_NUM_18, 
                                    GPIO_NUM_19, 
                                    0, 
                                    0x20);
    this -> PCF8574.write_port(0x55);
    this -> lcd.pins.d4 = cfgLCD->d4;
    this -> lcd.pins.d5 = cfgLCD->d5;
    this -> lcd.pins.d6 = cfgLCD->d6;
    this -> lcd.pins.d7 = cfgLCD->d7;
    this -> lcd.pins.e = cfgLCD->e;
    this -> lcd.pins.rs = cfgLCD->rs;
    // use callback to send data to LCD by I2C GPIO expander
    this -> lcd.font = cfgLCD->font;
    this -> lcd.lines = cfgLCD->lines;
    this -> lcd.pins.bl = cfgLCD->backlight;
    // this -> lcd.write_cb = (hd44780_write_cb_t)pLCD_cb(&this->lcd, *pcf_data); 
    hd44780_init(&this->lcd);
    return err;
}

esp_err_t ClassLCDI2C::clear(){return hd44780_clear(&this->lcd);}
