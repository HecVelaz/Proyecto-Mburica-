#include "reloj.h"
#include <stdio.h>
#include <time.h>
#include "esp_log.h"
static const char *TAG = "RELOJ";

#define DS3231_ADDR 0x68

// ======================================================
// BCD -> DECIMAL
// ======================================================

static uint8_t bcd_to_dec(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

// ======================================================
// INIT RTC
// ======================================================

bool reloj_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = RTC_I2C_SDA,
        .scl_io_num = RTC_I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    esp_err_t ret;

    ret = i2c_param_config(
        I2C_MASTER_NUM,
        &conf
    );

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error config I2C");
        return false;
    }

    ret = i2c_driver_install(
        I2C_MASTER_NUM,
        conf.mode,
        0,
        0,
        0
    );

    if (ret != ESP_OK &&
        ret != ESP_ERR_INVALID_STATE)
    {
        ESP_LOGE(TAG, "Error install I2C");
        return false;
    }

    ESP_LOGI(TAG, "RTC inicializado");

    return true;
}

// ======================================================
// LEER HORA
// ======================================================

bool reloj_now(rtc_datetime_t *dt)
{
    uint8_t data[7];

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);

    i2c_master_write_byte(
        cmd,
        (DS3231_ADDR << 1) | I2C_MASTER_WRITE,
        true
    );

    i2c_master_write_byte(
        cmd,
        0x00,
        true
    );

    i2c_master_start(cmd);

    i2c_master_write_byte(
        cmd,
        (DS3231_ADDR << 1) | I2C_MASTER_READ,
        true
    );

    i2c_master_read(
        cmd,
        data,
        6,
        I2C_MASTER_ACK
    );

    i2c_master_read_byte(
        cmd,
        &data[6],
        I2C_MASTER_NACK
    );

    i2c_master_stop(cmd);

    esp_err_t ret = i2c_master_cmd_begin(
        I2C_MASTER_NUM,
        cmd,
        pdMS_TO_TICKS(100)
    );

    i2c_cmd_link_delete(cmd);

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "No se pudo leer RTC");
        return false;
    }

    dt->second = bcd_to_dec(data[0] & 0x7F);
    dt->minute = bcd_to_dec(data[1]);
    dt->hour   = bcd_to_dec(data[2]);
    dt->day    = bcd_to_dec(data[4]);
    dt->month  = bcd_to_dec(data[5]);
    dt->year   = 2000 + bcd_to_dec(data[6]);

    return true;
}

// ======================================================
// FORMATEAR FECHA
// ======================================================

void reloj_format(
    const rtc_datetime_t *dt,
    char *buffer,
    size_t len
)
{
    snprintf(
        buffer,
        len,
        "%04d-%02d-%02d %02d:%02d:%02d",
        dt->year,
        dt->month,
        dt->day,
        dt->hour,
        dt->minute,
        dt->second
    );
}

// ======================================================
// DATETIME -> UNIX EPOCH
// ======================================================

uint32_t reloj_datetime_to_epoch(
    const rtc_datetime_t *dt
)
{
    struct tm t = {
        .tm_year = dt->year - 1900,
        .tm_mon  = dt->month - 1,
        .tm_mday = dt->day,
        .tm_hour = dt->hour,
        .tm_min  = dt->minute,
        .tm_sec  = dt->second
    };

    return (uint32_t)mktime(&t);
}

uint32_t reloj_segundos_hasta_proximo_slot_5min(const rtc_datetime_t *dt)
{
    uint32_t segundos_actuales;

    segundos_actuales = (dt->minute * 60) + dt->second;

    uint32_t intervalo = 2 * 60;

    uint32_t resto = segundos_actuales % intervalo;

    if (resto == 0)
    {
        return 0;
    }

    return intervalo - resto;
}