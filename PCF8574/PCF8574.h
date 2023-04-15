#ifndef __PCF8574_H__
#define __PCF8574_H__

#include "../LibraryConfig.h"

#ifdef CONFIG_USE_PCF8574
#include "esp_log.h"
#include "driver/gpio.h"
#include <stdbool.h>
#include <stdlib.h>
#include <i2cdev.h>
#include <esp_err.h>
#include "driver/i2c.h"
#include <iostream>
using namespace std;

#define PCF8574_I2C_FREQ_HZ         100000 // 100KHz


class ClassPCF8574{
private:
    i2c_dev_t i2c_dev;
    uint8_t value;
public:
    esp_err_t begin(gpio_num_t sda_gpio, gpio_num_t scl_gpio, i2c_port_t port, uint8_t Address,bool PULLUP);
    esp_err_t end();

    esp_err_t write_port(uint8_t val);

    esp_err_t write(uint8_t val);
    esp_err_t write(uint8_t pin, bool val);
};


#endif // __PCF8574_H__
#endif