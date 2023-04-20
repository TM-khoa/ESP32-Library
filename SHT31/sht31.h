#ifndef __SHT31_H__
#define __SHT31_H__

#include "esp_log.h"
#include "driver/i2c.h"

#define SHT31_ADDR 0x44
#define SDA_PIN    21
#define SCL_PIN    22 

#define WRITE_BIT   I2C_MASTER_WRITE            
#define READ_BIT    I2C_MASTER_READ
#define ACK_CHECK_EN    1
#define ACK_CHECK_DIS   0
#define ACK_VAL     0
#define NACK_VAL    1

#define SINGLESHOT_CLK_STRETCH_EN        0x2C
typedef struct{
    uint8_t HIGH;
    uint8_t MED;
    uint8_t LOW;
}Enable_Repeat;


#define SINGLESHOT_CLK_STRETCH_DIS       0x24
typedef struct{
    uint8_t HIGH;
    uint8_t MED;
    uint8_t LOW;
}Disable_Repeat;



esp_err_t sht31_init(void);

uint8_t sht31_crc(uint8_t *data);

esp_err_t sht31_read_temp_humi(float *temp, float *humi);


#endif