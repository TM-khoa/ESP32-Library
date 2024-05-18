#ifndef _LCD_I2C_HPP_
#define _LCD_I2C_HPP_


#include "../LibraryConfig.h"
#ifdef CONFIG_USE_LCDI2C
#include "esp_log.h"
#include "driver/gpio.h"

#include <stdbool.h>
#include <stdlib.h>
#include <i2cdev.h>
#include <esp_err.h>
#include "PCF8574.hpp"
#include "hd44780.h"
#include <string.h>

#define LCD_PCF8574_RS          0
#define LCD_PCF8574_E           2
#define LCD_PCF8574_D4          3
#define LCD_PCF8574_D5          4
#define LCD_PCF8574_D6          5
#define LCD_PCF8574_D7          6
#define LCD_PCF8574_BACKLIGHT   7
#define LCD_ROWS    4
#define LCD_COLS    20

#define I2C_MASTER_SDA_IO GPIO_NUM_21
#define I2C_MASTER_SCL_IO GPIO_NUM_22
#define I2C_PORT I2C_NUM_0
#define PCF8574_DEFAULT_ADDRESS 0x20

class ClassLCDI2C{
private:
    
public:
    esp_err_t begin();
    esp_err_t clear();
    esp_err_t TurnOnBackLight();
    esp_err_t TurnOffBackLight();
    esp_err_t print(const char *S, uint8_t x, uint8_t y);
    esp_err_t testPCF(uint8_t value);
};
#endif
#endif