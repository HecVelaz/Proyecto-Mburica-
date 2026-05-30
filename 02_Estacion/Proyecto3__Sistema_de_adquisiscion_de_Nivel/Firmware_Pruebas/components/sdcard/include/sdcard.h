#pragma once

#include "esp_err.h"
#include "driver/gpio.h"
#include <stdbool.h>

// SPI
#define SD_PIN_CS   ((gpio_num_t)10)
#define SD_PIN_MOSI ((gpio_num_t)11)
#define SD_PIN_CLK  ((gpio_num_t)12)
#define SD_PIN_MISO ((gpio_num_t)13)

// Detect
#define SD_PIN_DET  ((gpio_num_t)15)

#define MOUNT_POINT "/sdcard"

// Funciones
bool detectar_tarjeta(void);

esp_err_t verificar_sd_card(void);