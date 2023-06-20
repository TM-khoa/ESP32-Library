#ifndef _LCD_I2C_H_
#define _LCD_I2C_H_


#include "../LibraryConfig.h"
#ifdef CONFIG_USE_LCDI2C
#include "esp_log.h"
#include "driver/gpio.h"

#include <stdbool.h>
#include <stdlib.h>
#include <i2cdev.h>
#include <esp_err.h>
#include "PCF8574.h"
#include "hd44780.h"
#include <string.h>

#if defined(CONFIG_USE_KCONFIG)
// #include "../../build/config/sdkconfig.h"

#define LCD_PCF8574_RS          CONFIG_LCD_RS_PIN
#define LCD_PCF8574_E           CONFIG_LCD_E_PIN
#define LCD_PCF8574_D4          CONFIG_LCD_D4_PIN
#define LCD_PCF8574_D5          CONFIG_LCD_D5_PIN
#define LCD_PCF8574_D6          CONFIG_LCD_D6_PIN
#define LCD_PCF8574_D7          CONFIG_LCD_D7_PIN
#define LCD_PCF8574_BACKLIGHT   CONFIG_LCD_BACKLIGHT_PIN
#define LCD_ROWS    CONFIG_ROWS 
#define LCD_COLS    CONFIG_COLLUNMS
#else
#define LCD_PCF8574_RS          0
#define LCD_PCF8574_E           2
#define LCD_PCF8574_D4          3
#define LCD_PCF8574_D5          4
#define LCD_PCF8574_D6          5
#define LCD_PCF8574_D7          6
#define LCD_PCF8574_BACKLIGHT   7
#define LCD_ROWS    4
#define LCD_COLS    20
#endif /*CONFIG_USE_KCONFIG*/

#define I2C_MASTER_SDA_IO GPIO_NUM_21
#define I2C_MASTER_SCL_IO GPIO_NUM_22
#define I2C_PORT I2C_NUM_0
#define PCF8574_DEFAULT_ADDRESS 0x20

#if defined(CONFIG_USE_CPP)
#include <iostream>
using namespace std;
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
#else
typedef struct LCDI2C{
    PCF8574 pcf;
    hd44780_t lcdDriver;
}LCDI2C;

#define lcd_Driver pLCD_I2C->lcdDriver
#define lcd_sdaPin pLCD_I2C->pcf.i2c_dev.cfg.sda_io_num
#define lcd_sclPin pLCD_I2C->pcf.i2c_dev.cfg.scl_io_num
#define lcd_pcfAddr pLCD_I2C->pcf.i2c_dev.addr
#define lcd_i2cPort pLCD_I2C->pcf.i2c_dev.port
#define lcd_D4 pLCD_I2C->lcdDriver.pins.d4
#define lcd_D5 pLCD_I2C->lcdDriver.pins.d5
#define lcd_D6 pLCD_I2C->lcdDriver.pins.d6
#define lcd_D7 pLCD_I2C->lcdDriver.pins.d7
#define lcd_E  pLCD_I2C->lcdDriver.pins.e
#define lcd_RS pLCD_I2C->lcdDriver.pins.rs
#define lcd_bl pLCD_I2C->lcdDriver.pins.bl
#define lcd_row pLCD_I2C->lcdDriver.lines
#define lcd_font pLCD_I2C->lcdDriver.font

esp_err_t LCDI2C_Config(LCDI2C *lcdi2c);
esp_err_t LCDI2C_Print(const char *S, uint8_t x, uint8_t y);
esp_err_t LCDI2C_TurnOnBackLight();
esp_err_t LCDI2C_TurnOffBackLight();
esp_err_t LCDI2C_Clear();
esp_err_t LCDI2C_TestOutput();

#endif /*CONFIG_USE_CPP*/

#endif /*CONFIG_USE_LCDI2C*/
#endif // LCD_I2C_H