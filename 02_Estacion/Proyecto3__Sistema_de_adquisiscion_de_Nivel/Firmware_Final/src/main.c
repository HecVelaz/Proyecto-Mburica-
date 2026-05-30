#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "reloj.h"
#include "led_blink.h"
#include "sdcard.h"
#include "sensor_rs485.h"
#include "modem_dtu.h"

static const char *TAG = "MAIN";
static SemaphoreHandle_t dtu_mutex;


// ======================================================
// ENVIO DTU PARA CICLO PRINCIPAL
// Espera mas tiempo porque tiene prioridad.
// ======================================================

static bool dtu_send_level_main(uint32_t timestamp, float nivel)
{
    bool ok = false;

    if (xSemaphoreTake(dtu_mutex, pdMS_TO_TICKS(30000)) == pdTRUE)
    {
        ok = dtu_send_level(timestamp, nivel);
        xSemaphoreGive(dtu_mutex);
    }
    else
    {
        ESP_LOGW(TAG, "DTU ocupado. No se pudo enviar dato actual");
    }

    return ok;
}

// ======================================================
// ENVIO DTU PARA PENDIENTES
// Espera poco. Si el DTU esta ocupado, no molesta al ciclo principal.
// ======================================================

static bool dtu_send_level_bg(uint32_t timestamp, float nivel)
{
    bool ok = false;

    if (xSemaphoreTake(dtu_mutex, pdMS_TO_TICKS(1000)) == pdTRUE)
    {
        ok = dtu_send_level(timestamp, nivel);
        xSemaphoreGive(dtu_mutex);
    }
    else
    {
        ESP_LOGW(TAG, "DTU ocupado. Pendiente queda en cola");
    }

    return ok;
}

// ======================================================
// TASK DE PENDIENTES EN SEGUNDO PLANO
// ======================================================
#define MARGEN_BG_SLOT_S 35
static void task_pendientes(void *pv)
{
    while (1)
    {
        rtc_datetime_t dt;

        if (reloj_now(&dt))
        {
            uint32_t espera_s = reloj_segundos_hasta_proximo_slot_5min(&dt);

            if (espera_s > MARGEN_BG_SLOT_S)
            {
                uint32_t ts;
                float nivel;

                if (sdcard_has_pending() &&
                    sdcard_read_first_pending(&ts, &nivel))
                {
                    ESP_LOGI(TAG,
                             "BG reenviando pendiente: d=%lu,%.3f",
                             (unsigned long)ts,
                             nivel);

                    if (dtu_send_level_bg(ts, nivel))
                    {
                        sdcard_remove_first_pending();
                        ESP_LOGI(TAG, "BG pendiente enviado y eliminado");
                    }
                    else
                    {
                        ESP_LOGW(TAG, "BG fallo reenvio. Cola conservada");
                    }
                }
            }
            else
            {
                ESP_LOGI(TAG,
                         "BG no envia: faltan %lu s para el proximo slot",
                         (unsigned long)espera_s);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// ======================================================
// APP MAIN
// ======================================================

void app_main(void)
{   


    
    ESP_LOGI(TAG, "Iniciando sistema solo nivel...");

    reloj_init();
    sdcard_init();
    sensor_rs485_init();
    dtu_init();
    led_init();
    dtu_mutex = xSemaphoreCreateMutex();

    xTaskCreate(
             task_pendientes,
             "task_pendientes",
             4096,
             NULL,
             4,
            NULL
              );

    while (1)
    {
        rtc_datetime_t dt;

        if (!reloj_now(&dt))
        {
            ESP_LOGE(TAG, "No se pudo leer RTC");
            vTaskDelay(pdMS_TO_TICKS(5000));
            continue;
        }

        uint32_t espera_s = reloj_segundos_hasta_proximo_slot_5min(&dt);

        if (espera_s > 0)
        {
            ESP_LOGI(TAG,
                     "Esperando %lu s hasta proximo slot",
                     (unsigned long)espera_s);

            vTaskDelay(pdMS_TO_TICKS(espera_s * 1000));
            continue;
        }
   
        float nivel_m = 0.0f;
        uint32_t timestamp = reloj_datetime_to_epoch(&dt) + (3 * 3600);

        char fecha[32];
        reloj_format(&dt, fecha, sizeof(fecha));

        ESP_LOGI(TAG, "Procesando slot RTC: %s", fecha);

        bool lectura_ok = false;

        for (int intento = 1; intento <= 3; intento++)
        {
            ESP_LOGI(TAG, "Intento lectura sensor %d/3", intento);

            if (sensor_rs485_read_level(&nivel_m))
            {
                led_blink_sensor_ok();
                lectura_ok = true;
                break;
            }

            ESP_LOGW(TAG, "Fallo lectura sensor, reintentando...");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        if (!lectura_ok)
        {
            ESP_LOGE(TAG, "No se pudo leer nivel luego de 3 intentos");
            nivel_m = -1.0f;
        }

        // Guardar SIEMPRE en datalog.csv
        sdcard_append_level(timestamp, nivel_m);

        // Enviar SIEMPRE el dato actual primero
        if (!dtu_send_level_main(timestamp, nivel_m))
        {
            ESP_LOGW(TAG, "Fallo envio actual. Guardando pendiente");
            sdcard_append_pending(timestamp, nivel_m);
        }

        ESP_LOGI(TAG,
                 "Ciclo terminado: d=%lu,%.3f",
                 (unsigned long)timestamp,
                 nivel_m);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}