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
#include "../../build/config/sdkconfig.h"

#define LCD_PCF8574_RS          CONFIG_LCD_RS_PIN
#define LCD_PCF8574_E           CONFIG_LCD_E_PIN
#define LCD_PCF8574_D4          CONFIG_LCD_D4_PIN
#define LCD_PCF8574_D5          CONFIG_LCD_D5_PIN
#define LCD_PCF8574_D6          CONFIG_LCD_D6_PIN
#define LCD_PCF8574_D7          CONFIG_LCD_D7_PIN
#define LCD_PCF8574_BACKLIGHT   CONFIG_LCD_BACKLIGHT_PIN


#define LCD_ROWS    CONFIG_ROWS 
#define LCD_COLS    CONFIG_COLLUNMS
class ClassLCDI2C{
private:
    
public:
    esp_err_t begin(i2c_dev_t *dev);
    esp_err_t clear();
    esp_err_t TurnOnBackLight();
    esp_err_t TurnOffBackLight();
    esp_err_t print(const char *S, uint8_t x, uint8_t y);
    esp_err_t testPCF();
};





#endif // LCD_I2C_H