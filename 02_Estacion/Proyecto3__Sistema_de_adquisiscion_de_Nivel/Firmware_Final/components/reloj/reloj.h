#ifndef RELOJ_H
#define RELOJ_H

#include <stdbool.h>
#include <stdint.h>

#include "driver/gpio.h"
#include "driver/i2c.h"

// ======================================================
// PINES RTC DS3231
// ======================================================

#define RTC_I2C_SDA       GPIO_NUM_8
#define RTC_I2C_SCL       GPIO_NUM_9

#define I2C_MASTER_NUM    I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

// ======================================================
// ESTRUCTURA FECHA/HORA
// ======================================================

typedef struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;

    uint8_t hour;
    uint8_t minute;
    uint8_t second;

} rtc_datetime_t;

// ======================================================
// FUNCIONES
// ======================================================

bool reloj_init(void);

bool reloj_now(rtc_datetime_t *dt);

void reloj_format(
    const rtc_datetime_t *dt,
    char *buffer,
    size_t len
);

uint32_t reloj_datetime_to_epoch(
    const rtc_datetime_t *dt
);
uint32_t reloj_segundos_hasta_proximo_slot_5min(
    const rtc_datetime_t *dt
);
#endif