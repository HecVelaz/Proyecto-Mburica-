#ifndef LED_BLINK_H
#define LED_BLINK_H

#include "driver/gpio.h"

#define LED_BLINK_GPIO     GPIO_NUM_21
#define LED_BLINK_TIME_MS  1000

void led_init(void);
void led_blink_sensor_ok(void);

#endif