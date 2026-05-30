#include "dtu.h"

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "DTU";

// ======================================================
// Inicializar UART RS485
// ======================================================
void dtu_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = DTU_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_driver_install(
        DTU_UART_PORT,
        DTU_BUF_SIZE * 2,
        DTU_BUF_SIZE * 2,
        0,
        NULL,
        0
    );

    uart_param_config(
        DTU_UART_PORT,
        &uart_config
    );

    uart_set_pin(
        DTU_UART_PORT,
        DTU_TX_PIN,
        DTU_RX_PIN,
        DTU_RTS_PIN,
        UART_PIN_NO_CHANGE
    );

    uart_set_mode(
        DTU_UART_PORT,
        UART_MODE_RS485_HALF_DUPLEX
    );

    ESP_LOGI(TAG, "UART RS485 inicializada");

    vTaskDelay(pdMS_TO_TICKS(3000));
}

// ======================================================
// Enviar dato HTTP
// ======================================================
void dtu_enviar_dato(const char *dato)
{
    ESP_LOGI(TAG, "Enviando dato HTTP: %s", dato);

    uart_flush_input(DTU_UART_PORT);

    vTaskDelay(pdMS_TO_TICKS(200));

    uart_write_bytes(
        DTU_UART_PORT,
        dato,
        strlen(dato)
    );

    // Importante: probar con fin de línea
    uart_write_bytes(
        DTU_UART_PORT,
        "\r\n",
        2
    );

    uart_wait_tx_done(
        DTU_UART_PORT,
        pdMS_TO_TICKS(3000)
    );

    ESP_LOGI(TAG, "Dato enviado al modem");
}

// ======================================================
// Leer respuesta
// ======================================================
void dtu_leer_respuesta(uint32_t timeout_ms)
{
    uint8_t data[DTU_BUF_SIZE];

    int len = uart_read_bytes(
        DTU_UART_PORT,
        data,
        DTU_BUF_SIZE - 1,
        pdMS_TO_TICKS(timeout_ms)
    );

    if (len > 0)
    {
        data[len] = 0;

        ESP_LOGI(TAG, "RESPUESTA:");
        printf("%s\n", (char *)data);
    }
    else
    {
        ESP_LOGW(TAG, "Sin respuesta");
    }
}