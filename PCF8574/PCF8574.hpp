#ifndef __PCF8574_HPP__
#define __PCF8574_HPP__

#include "../LibraryConfig.h"

#ifdef CONFIG_USE_PCF8574
#include "esp_log.h"
#include "driver/gpio.h"
#include <stdbool.h>
#include <stdlib.h>
#include <i2cdev.h>
#include <esp_err.h>
#include "driver/i2c_master.h"

#define PCF8574_I2C_FREQ_HZ         100000 // 100KHz

class ClassPCF8574{
private:
    i2c_device_config_t devCfg;
    i2c_master_dev_handle_t devHandle;
    uint8_t value;
public:
    i2c_master_dev_handle_t* get_address_dev_handle();
    i2c_device_config_t* get_address_device_config();
    esp_err_t begin(gpio_num_t sda_gpio, gpio_num_t scl_gpio, i2c_port_t port, uint8_t Address,bool pullup);
    esp_err_t end();

    esp_err_t write_port(uint8_t val);

    esp_err_t write(uint8_t val);
    esp_err_t write(uint8_t pin, bool val);
};
#endif
#endif