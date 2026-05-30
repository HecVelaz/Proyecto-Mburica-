#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "driver/i2c.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "reloj.h"

static const char *TAG = "RTC_DS3231";

#define DS3231_I2C_ADDRESS      0x68

// ================================
// AJUSTE MANUAL DE DESFASE
// ================================
#define COMPENSACION_SEGUNDOS   56

// ================================
// DECIMAL -> BCD
// ================================
static uint8_t decimal_a_bcd(uint8_t val)
{
    return ((val / 10) << 4) | (val % 10);
}

// ================================
// BCD -> DECIMAL
// ================================
static uint8_t bcd_a_decimal(uint8_t val)
{
    return ((val >> 4) * 10) + (val & 0x0F);
}

// ================================
// OBTENER NUMERO DE MES
// ================================
static uint8_t obtener_mes(const char *mes)
{
    if (strcmp(mes, "Jan") == 0) return 1;
    if (strcmp(mes, "Feb") == 0) return 2;
    if (strcmp(mes, "Mar") == 0) return 3;
    if (strcmp(mes, "Apr") == 0) return 4;
    if (strcmp(mes, "May") == 0) return 5;
    if (strcmp(mes, "Jun") == 0) return 6;
    if (strcmp(mes, "Jul") == 0) return 7;
    if (strcmp(mes, "Aug") == 0) return 8;
    if (strcmp(mes, "Sep") == 0) return 9;
    if (strcmp(mes, "Oct") == 0) return 10;
    if (strcmp(mes, "Nov") == 0) return 11;
    if (strcmp(mes, "Dec") == 0) return 12;

    return 1;
}

// ================================
// INICIALIZAR I2C
// ================================
void inicializar_rtc_ds3231(void)
{
    ESP_LOGI(TAG, "Inicializando I2C DS3231...");

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = RTC_I2C_SDA,
        .scl_io_num = RTC_I2C_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));

    esp_err_t err = i2c_driver_install(
        I2C_MASTER_NUM,
        conf.mode,
        0,
        0,
        0
    );

    if (err == ESP_ERR_INVALID_STATE) {
        ESP_LOGW(TAG, "Driver I2C ya instalado");
    } else {
        ESP_ERROR_CHECK(err);
    }

    ESP_LOGI(TAG, "I2C inicializado correctamente");
}

// ================================
// AJUSTAR RTC CON HORA COMPILACION
// ================================
void reloj_ajustar_hora_compilacion(void)
{
    const char *fecha_compilacion = __DATE__;
    const char *hora_compilacion  = __TIME__;

    char mes_texto[4];

    int dia;
    int mes;
    int anio;

    int hora;
    int minuto;
    int segundo;

    sscanf(fecha_compilacion, "%3s %d %d",
           mes_texto,
           &dia,
           &anio);

    sscanf(hora_compilacion, "%d:%d:%d",
           &hora,
           &minuto,
           &segundo);

    mes = obtener_mes(mes_texto);

    // ==========================================
    // COMPENSACION POR TIEMPO DE FLASHEO
    // ==========================================
    segundo += COMPENSACION_SEGUNDOS;

    while (segundo >= 60) {
        segundo -= 60;
        minuto++;
    }

    while (minuto >= 60) {
        minuto -= 60;
        hora++;
    }

    while (hora >= 24) {
        hora -= 24;
    }

    // ==========================================
    // DATOS A ESCRIBIR EN RTC
    // ==========================================
    uint8_t datos[8];

    datos[0] = 0x00;

    datos[1] = decimal_a_bcd(segundo);
    datos[2] = decimal_a_bcd(minuto);
    datos[3] = decimal_a_bcd(hora);

    datos[4] = decimal_a_bcd(1);

    datos[5] = decimal_a_bcd(dia);
    datos[6] = decimal_a_bcd(mes);
    datos[7] = decimal_a_bcd(anio - 2000);

    esp_err_t err = i2c_master_write_to_device(
        I2C_MASTER_NUM,
        DS3231_I2C_ADDRESS,
        datos,
        sizeof(datos),
        pdMS_TO_TICKS(100)
    );

    if (err == ESP_OK) {

        ESP_LOGI(TAG,
                 "RTC AJUSTADO: %04d-%02d-%02d %02d:%02d:%02d",
                 anio,
                 mes,
                 dia,
                 hora,
                 minuto,
                 segundo);

    } else {

        ESP_LOGE(TAG, "Error ajustando RTC");
    }
}

// ================================
// LEER HORA RTC
// ================================
void leer_hora_rtc_ds3231(void)
{
    uint8_t datos_tiempo[7];

    while (1)
    {
        uint8_t registro = 0x00;

        esp_err_t err = i2c_master_write_read_device(
            I2C_MASTER_NUM,
            DS3231_I2C_ADDRESS,
            &registro,
            1,
            datos_tiempo,
            7,
            pdMS_TO_TICKS(100)
        );

        if (err == ESP_OK)
        {
            uint8_t segundos = bcd_a_decimal(datos_tiempo[0] & 0x7F);
            uint8_t minutos  = bcd_a_decimal(datos_tiempo[1] & 0x7F);
            uint8_t horas    = bcd_a_decimal(datos_tiempo[2] & 0x3F);

            uint8_t dia      = bcd_a_decimal(datos_tiempo[4] & 0x3F);
            uint8_t mes      = bcd_a_decimal(datos_tiempo[5] & 0x1F);
            uint8_t anio     = bcd_a_decimal(datos_tiempo[6]);

            ESP_LOGW(TAG,
                     "Hora RTC: 20%02d-%02d-%02d %02d:%02d:%02d",
                     anio,
                     mes,
                     dia,
                     horas,
                     minutos,
                     segundos);
        }
        else
        {
            ESP_LOGE(TAG, "Error leyendo RTC");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}