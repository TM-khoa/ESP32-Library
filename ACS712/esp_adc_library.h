#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

#define ADC_READ(_X_) adc1_get_raw(_X_)
#define micros() esp_log_early_timestamp()