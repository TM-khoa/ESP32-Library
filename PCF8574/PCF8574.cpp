#include "PCF8574.hpp"
#include <esp_err.h>
#include <esp_check.h>

#ifdef CONFIG_USE_PCF8574

#if defined(CONFIG_USE_CPP)
static const char *TAG = "PCF8574";

i2c_master_dev_handle_t* ClassPCF8574::get_address_dev_handle(){
    return &devHandle;
}
i2c_device_config_t* ClassPCF8574::get_address_device_config(){
    return &devCfg;
}

esp_err_t ClassPCF8574::begin(  gpio_num_t sda_gpio, 
                                gpio_num_t scl_gpio, 
                                i2c_port_t port, 
                                uint8_t Address,
                                bool pullup)
{
    i2c_master_bus_config_t i2c_master_config{

    .i2c_port = port,
    .sda_io_num = sda_gpio,
    .scl_io_num = scl_gpio,
    .clk_source = I2C_CLK_SRC_DEFAULT,
    .glitch_ignore_cnt = 7,
    };
    if(pullup){
        i2c_master_config.flags.enable_internal_pullup = true;
    }


    i2c_master_bus_handle_t bus_handle;
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_master_config, &bus_handle));

    devCfg.dev_addr_length = I2C_ADDR_BIT_LEN_7;
    devCfg.device_address = 0x20;
    devCfg.scl_speed_hz = 100000;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &devCfg, &devHandle));
    ESP_ERROR_CHECK(i2c_master_probe(bus_handle, 0x20, -1));
    return ESP_OK;
}

esp_err_t ClassPCF8574::end(){
    return i2c_master_bus_rm_device(devHandle);
}

esp_err_t ClassPCF8574::write_port(uint8_t val){
    ESP_ERROR_CHECK(i2c_master_transmit(devHandle,&val,1,-1));
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
#endif /*CONFIG_USE_CPP*/


#endif