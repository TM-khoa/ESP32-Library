/**
 * @file ultrasonic.h
 *
 * ESP-IDF driver for ultrasonic range meters, e.g. HC-SR04, HY-SRF05 and so on
 *
 * Ported from esp-open-rtos
 * Copyright (C) 2016, 2018 Ruslan V. Uss <unclerus@gmail.com>
 * BSD Licensed as described in the file LICENSE
 */
#ifndef __HC_SR04_H__
#define __HC_SR04_H__

#include "../LibraryConfig.h"
#ifdef CONFIG_USE_HCSR04

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include "driver/mcpwm_cap.h"
#include "esp_check.h"
// #include "../../build/config/sdkconfig.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HC_SR04_ECHO_GPIO CONFIG_HC_SR04_ECHO_GPIO
#define HC_SR04_TRIGGER_GPIO CONFIG_HC_SR04_TRIGGER_GPIO

void gen_trig_output(void);
esp_err_t HC_SR04_init();

#ifdef __cplusplus
}
#endif

#endif /* __ULTRASONIC_H__ */
#endif