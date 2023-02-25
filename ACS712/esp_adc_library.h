#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"
#define TAG_ESP_ADC_LIB "esp_adc_lib"

#define ADC_READ(_X_) adc1_get_raw(_X_)
#define micros() esp_log_early_timestamp()

#define ACS1 ADC1_CHANNEL_7
#define ACS2 ADC1_CHANNEL_3
#define ACS3 ADC1_CHANNEL_5
#define ACS4 ADC1_CHANNEL_4

#define AI4_20mA ADC1_CHANNEL_0

static esp_adc_cal_characteristics_t adc1_chars;

static bool adc_calibration_init(void)
{
    esp_err_t ret;
    bool calib_enable = false;

    ret = esp_adc_cal_check_efuse(ESP_ADC_CAL_VAL_EFUSE_VREF);
    if (ret == ESP_ERR_NOT_SUPPORTED) {
        ESP_LOGW(TAG_ESP_ADC_LIB, "Calibration scheme not supported, skip software calibration");
    } else if (ret == ESP_ERR_INVALID_VERSION) {
        ESP_LOGW(TAG_ESP_ADC_LIB, "eFuse not burnt, skip software calibration");
    } else if (ret == ESP_OK) {
        calib_enable = true;
        esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_DEFAULT, 0, &adc1_chars);
        
    } else {
        ESP_LOGE(TAG_ESP_ADC_LIB, "Invalid arg");
    }

    return calib_enable;
}
