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

#define PCF8574_I2C_FREQ_HZ         100000 // 100KHz



#if defined(CONFIG_USE_CPP)
#else
typedef enum{
    PCF8574_OK,
    PCF8574_ERROR,
    PCF8574_INVALID_ARG,
    PCF8574_INVALID_PIN,
}PCF8574_Status_t;

#define pcfI2CDEV pPCF->i2c_dev
#define pcfPort pPCF->i2c_dev.port
#define pcfAddr pPCF->i2c_dev.addr
#define pcfSDA pPCF->i2c_dev.cfg.sda_io_num
#define pcfSCL pPCF->i2c_dev.cfg.scl_io_num
#define pcfValue pPCF->value
typedef struct PCF8574{
    i2c_dev_t i2c_dev;
    uint8_t value;
}PCF8574;

esp_err_t PCF8574_Init(PCF8574 *PCF8574, bool PULLUP);
PCF8574_Status_t PCF8574_WritePort(uint8_t val);
PCF8574_Status_t PCF8574_Write(uint8_t val);
PCF8574_Status_t PCF8574_WritePin(uint8_t pin, bool val);

#endif // CONFIG_USE_CPP

#endif // __PCF8574_H__
#endif