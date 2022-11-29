#include "driver/adc.h"
#include "esp_adc_cal.h"

#define ADC_READ(_X_) adc1_get_raw(_X_)