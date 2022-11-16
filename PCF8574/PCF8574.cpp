#include "PCF8574.h"
static const char *TAG = "PCF8574";

#define CHECK(x)                \
    do                          \
    {                           \
        esp_err_t __;           \
        if ((__ = x) != ESP_OK) \
            return __;          \
    } while (0)

#define CHECK_ARG(VAL)                  \
    do                                  \
    {                                   \
        if (!(VAL))                     \
            return ESP_ERR_INVALID_ARG; \
    } while (0)

esp_err_t ClassPCF8574::begin(gpio_num_t sda_gpio, gpio_num_t scl_gpio, i2c_port_t port, uint8_t Address){
    this->i2c_dev.port = port;
    this->i2c_dev.addr = Address;
    this->i2c_dev.cfg.sda_io_num = sda_gpio;
    this->i2c_dev.cfg.scl_io_num = scl_gpio;

#if HELPER_TARGET_IS_ESP32
    this->i2c_dev.cfg.master.clk_speed = PCF8574_I2C_FREQ_HZ;
#endif
    return i2c_dev_create_mutex(&this->i2c_dev);
}

esp_err_t ClassPCF8574::end(){
    return i2c_dev_delete_mutex(&this->i2c_dev);
}

esp_err_t ClassPCF8574::write_port(uint8_t val){
    I2C_DEV_TAKE_MUTEX(&this->i2c_dev);
    I2C_DEV_CHECK(&this->i2c_dev, i2c_dev_write(&this->i2c_dev, NULL, 0, &val, 1));
    I2C_DEV_GIVE_MUTEX(&this->i2c_dev);

    return ESP_OK;
}

esp_err_t ClassPCF8574::write(uint8_t val){
    esp_err_t err = write_port(val);
    
    if (err != ESP_OK){
        ESP_LOGE(TAG, "%s", esp_err_to_name(err));
        return err;
    }
    this->value = val;
    return err;
}

esp_err_t ClassPCF8574::write(uint8_t pin, bool val){
    if (pin > 7){
        ESP_LOGE(TAG, "Exceed pin number");
        return ESP_FAIL;
    }
    uint8_t temp = this->value;
    (val) ? (temp |= (0x01 << pin)) : (temp &= ~(1 << pin));
    return write(temp);
}
