#include "ManHinhLCD.h"
static const char *TAG = "ManHinhLCD";


ClassPCF8574 _PCF8574;
hd44780_t lcd;

bool _LCD_batDenNen;

static esp_err_t write_lcd_data(const hd44780_t *lcd, uint8_t data){
    return _PCF8574.write(data);
}


// esp_err_t ClassLCDI2C::begin(gpio_num_t sda_gpio, gpio_num_t scl_gpio, i2c_port_t port, uint8_t Address){
//     esp_err_t err = ESP_OK;
//     err |= this -> PCF8574.begin(sda_gpio, scl_gpio, port, Address);

//     this -> lcd.write_cb = write_lcd_data; // use callback to send data to LCD by I2C GPIO expander
//     this->lcd.font = HD44780_FONT_5X8;

//     this -> lcd.lines = 4;
    
//     this -> lcd.pins.rs = LCD_PCF8574_RS;
//     this -> lcd.pins.e = LCD_PCF8574_E;
//     this -> lcd.pins.d4 = LCD_PCF8574_D4;
//     this -> lcd.pins.d5 = LCD_PCF8574_D5;
//     this ->lcd.pins.d6 = LCD_PCF8574_D6;
//     this -> lcd.pins.d7 = LCD_PCF8574_D7;
//     this -> lcd.pins.bl = LCD_PCF8574_BACKLIGHT;
// }



esp_err_t KhoiTaoLCD(gpio_num_t sda_gpio, gpio_num_t scl_gpio, i2c_port_t port, uint8_t Address){
    esp_err_t err = ESP_OK;
    
    err |= _PCF8574.begin(sda_gpio, scl_gpio, port, Address);
    
    lcd.write_cb = write_lcd_data; // use callback to send data to LCD by I2C GPIO expander
    lcd.font = HD44780_FONT_5X8;

    lcd.lines = 4;
    
    lcd.pins.rs = LCD_PCF8574_RS;
    lcd.pins.e = LCD_PCF8574_E;
    lcd.pins.d4 = LCD_PCF8574_D4;
    lcd.pins.d5 = LCD_PCF8574_D5;
    lcd.pins.d6 = LCD_PCF8574_D6;
    lcd.pins.d7 = LCD_PCF8574_D7;
    lcd.pins.bl = LCD_PCF8574_BACKLIGHT;

    err |= hd44780_init(&lcd);
    err |= hd44780_switch_backlight(&lcd, true);
    _LCD_batDenNen = true;


    //err |= hd44780_gotoxy(&lcd, 3, 1);
    //err |= hd44780_puts(&lcd, "Hello world!!!");    
    
    return err;
}

esp_err_t HuyKhoiTaoLCD(){
    esp_err_t err = ESP_OK;
    err = _PCF8574.end();
    return err;
}





