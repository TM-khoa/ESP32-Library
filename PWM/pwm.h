#ifndef __PWM_H__
#define __PWM_H__
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PWM_GPIO CONFIG_PWM_GPIO
#define PWM_FREQ CONFIG_PWM_FREQ
#define PWM_PERIOD CONFIG_PWM_PERIOD
void pwm_init(void);
#ifdef __cplusplus
}
#endif
#endif