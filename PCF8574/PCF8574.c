
#include "PCF8574.h"
#ifdef CONFIG_USE_PCF8574
#ifndef CONFIG_USE_CPP
#include <esp_err.h>
#include <esp_check.h>
PCF8574 *pPCF;

esp_err_t PCF8574_Init(PCF8574 *PCF8574, bool PULLUP)
{
    pPCF = PCF8574;
    if(PULLUP){
        pPCF->i2c_dev.cfg.scl_pullup_en = true;
        pPCF->i2c_dev.cfg.sda_pullup_en = true;
    }
    return i2c_dev_create_mutex(&pcfI2CDEV);
    // return ESP_OK;
}

void PCF7574_SetTarget(PCF8574 *PCF8574){
    if(PCF8574) pPCF = PCF8574;
}

PCF8574_Status_t PCF8574_WritePort(uint8_t val){
    I2C_DEV_TAKE_MUTEX(&pcfI2CDEV); 
    I2C_DEV_CHECK(&pcfI2CDEV, i2c_dev_write(&pcfI2CDEV, NULL, 0, &val, 1));
    I2C_DEV_GIVE_MUTEX(&pcfI2CDEV);
    return PCF8574_OK;
}

PCF8574_Status_t PCF8574_Write(uint8_t val){
    if (PCF8574_WritePort(val) != PCF8574_OK){
        return PCF8574_ERROR;
    }
    pcfValue = val;
    return PCF8574_OK;
}

PCF8574_Status_t PCF8574_WritePin(uint8_t pin, bool val){
    if (pin > 7) return PCF8574_INVALID_PIN;
    uint8_t temp = pcfValue;
    (val) ? (temp |= (0x01 << pin)) : (temp &= ~(1 << pin));
    return PCF8574_Write(temp);
}
#endif
#endif