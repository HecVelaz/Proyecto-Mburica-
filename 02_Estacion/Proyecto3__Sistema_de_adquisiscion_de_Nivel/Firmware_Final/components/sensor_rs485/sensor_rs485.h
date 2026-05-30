#ifndef SENSOR_RS485_H
#define SENSOR_RS485_H

#include <stdbool.h>

// Lee solo nivel/distancia en metros
bool sensor_rs485_init(void);
bool sensor_rs485_read_level(float *level_m);

#endif