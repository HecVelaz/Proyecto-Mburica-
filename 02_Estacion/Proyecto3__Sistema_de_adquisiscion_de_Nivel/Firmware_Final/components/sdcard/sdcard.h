#ifndef SDCARD_H
#define SDCARD_H

#include <stdbool.h>
#include <stdint.h>

#include "esp_err.h"
#include "driver/gpio.h"

// ======================================================
// PINES SD SPI
// ======================================================

#define SD_PIN_CS    ((gpio_num_t)10)
#define SD_PIN_MOSI  ((gpio_num_t)11)
#define SD_PIN_CLK   ((gpio_num_t)12)
#define SD_PIN_MISO  ((gpio_num_t)13)

// Detect
#define SD_PIN_DET   ((gpio_num_t)15)

#define MOUNT_POINT "/sdcard"

// ======================================================
// ARCHIVOS
// ======================================================

#define MAIN_LOG_FILE "/sdcard/datalog.csv"
#define TEMP_QUEUE_FILE "/sdcard/temp.csv"
// ======================================================
// FUNCIONES
// ======================================================

bool detectar_tarjeta(void);
bool sdcard_append_pending(uint32_t timestamp, float level_m);
bool sdcard_has_pending(void);
bool sdcard_init(void);

bool sdcard_append_level(
    uint32_t timestamp,
    float level_m
);
bool sdcard_read_first_pending(
    uint32_t *timestamp,
    float *level_m
);
bool sdcard_remove_first_pending(void);

#endif