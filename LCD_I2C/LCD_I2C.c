
#include "LCD_I2C.h"
#include "esp_check.h"
#ifdef CONFIG_USE_LCDI2C

LCDI2C *pLCD_I2C;

esp_err_t LCDI2C_TestOutput(){
    return PCF8574_Write(0x55);
}

static inline esp_err_t PCF_send_lcd_data(const hd44780_t *lcd, uint8_t data)
{
    return PCF8574_Write(data);
}

esp_err_t LCDI2C_Config(LCDI2C *lcdi2c)
{
    esp_err_t err = ESP_OK;
    // Check if object is NULL, return error if NULL
    if(!lcdi2c) return ESP_ERR_INVALID_ARG;
    pLCD_I2C = lcdi2c;
    i2cdev_init();
    lcd_sdaPin = I2C_MASTER_SDA_IO;
    lcd_sclPin = I2C_MASTER_SCL_IO;
    lcd_pcfAddr = PCF8574_DEFAULT_ADDRESS;
    lcd_i2cPort = I2C_NUM_0;
    pLCD_I2C->pcf.i2c_dev.cfg.mode = I2C_MODE_MASTER;
    pLCD_I2C->pcf.i2c_dev.cfg.master.clk_speed = 100000;
    err = PCF8574_Init(&pLCD_I2C->pcf,false);
    ESP_RETURN_ON_ERROR(err,"LCDI2C","PCF8574 Init error: %s",esp_err_to_name(err));
    LCDI2C_TestOutput();

    lcd_D4 = LCD_PCF8574_D4;
    lcd_D5 = LCD_PCF8574_D5;
    lcd_D6 = LCD_PCF8574_D6;
    lcd_D7 = LCD_PCF8574_D7;
    lcd_E = LCD_PCF8574_E;
    lcd_RS = LCD_PCF8574_RS;
    lcd_bl = LCD_PCF8574_BACKLIGHT;
    lcd_row = LCD_ROWS;
    lcd_font = HD44780_FONT_5X8;
    pLCD_I2C->lcdDriver.write_cb = PCF_send_lcd_data;
    err = hd44780_init(&lcd_Driver);
    ESP_RETURN_ON_ERROR(err,"LCDI2C","LCD driver Init error: %s",esp_err_to_name(err));
    return err;
}

esp_err_t LCDI2C_Print(const char *S, uint8_t x, uint8_t y)
{
    esp_err_t err = hd44780_gotoxy(&lcd_Driver,x,y);
    err = hd44780_puts(&lcd_Driver,S);
    return err;
}

esp_err_t LCDI2C_TurnOnBackLight(){return hd44780_switch_backlight(&lcd_Driver,1);}
esp_err_t LCDI2C_TurnOffBackLight() {return hd44780_switch_backlight(&lcd_Driver,0);}
esp_err_t LCDI2C_Clear(){return hd44780_clear(&lcd_Driver);}
#endif



