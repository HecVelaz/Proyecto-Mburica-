#pragma once
#include "driver/gpio.h"

// Definimos el GPIO21
#define BLINK_GPIO ((gpio_num_t)21) 

// Declaramos la función con el nombre CORRECTO que usa tu main.c
void parpadear_led(void);