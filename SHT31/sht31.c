#include "esp_log.h"
#include "driver/i2c.h"
#include "sht31.h"

static i2c_port_t i2c_port = I2C_NUM_0;

Enable_Repeat Rep_En = {
    .HIGH = 0x06,
    .MED = 0x0D,
    .LOW = 0x10,
};
Disable_Repeat Rep_Dis = {
    .HIGH = 0x00,
    .MED = 0x0B,
    .LOW = 0x16
};

esp_err_t sht31_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = SDA_PIN,
        .scl_io_num = SCL_PIN,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,
    };
    i2c_param_config(i2c_port, &conf);
    return i2c_driver_install(i2c_port, conf.mode,0,0,0);
}

uint8_t sht31_crc(uint8_t *data){
    uint8_t crc = 0xFF;
    for(int i = 0; i < 2; i++) {
        crc ^= data[i];
        for(int j = 0; j < 8; j++) {
            if(crc & 0x80) {
                crc <<= 1;
                crc ^= 0x131;
            }
            else
                crc <<= 1;
        }
    }
    return crc;
}

esp_err_t sht31_read_temp_humi(float *temp, float *humi){
    unsigned char data_write[] = {SINGLESHOT_CLK_STRETCH_DIS, Rep_Dis.HIGH};
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT31_ADDR << 1) | I2C_MASTER_WRITE, ACK_CHECK_EN);
    i2c_master_write(cmd, data_write, sizeof(data_write), ACK_CHECK_EN);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(i2c_port, cmd, 1000/portTICK_PERIOD_MS);
    if (ret != ESP_OK){
        return ret;
    }
    i2c_cmd_link_delete(cmd);

    vTaskDelay(2/portTICK_PERIOD_MS);

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (SHT31_ADDR << 1) | I2C_MASTER_READ, ACK_CHECK_EN);
    size_t size = 6;
    uint8_t *data_read = malloc(size);
    if (size > 1){
        i2c_master_read(cmd, data_read, size - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data_read + 5, NACK_VAL);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(i2c_port, cmd, 1000/portTICK_PERIOD_MS);
    if (ret != ESP_OK){
        return ret;
    }
    i2c_cmd_link_delete(cmd);

    if (data_read[2] != sht31_crc(data_read) || data_read[5] != sht31_crc(data_read + 3)){
        return ESP_ERR_INVALID_CRC;
    }
    else {
        *temp = -45 + (175 *(float)(data_read[0] * 256 + data_read[1])/ 65535.0);
        *humi = 100 * (float)(data_read[3] * 256 + data_read[4]) / 65535.0;
        return ESP_OK;
    }

    free (data_read);
}