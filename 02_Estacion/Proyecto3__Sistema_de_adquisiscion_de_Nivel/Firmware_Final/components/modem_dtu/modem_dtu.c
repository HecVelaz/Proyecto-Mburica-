#include "modem_dtu.h"

#include <stdio.h>
#include <string.h>

#include "driver/uart.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// ======================================================
// CONFIGURACION UART RS485 MODEM DTU
// ======================================================

#define DTU_UART_PORT     UART_NUM_2

#define DTU_TX_PIN        5
#define DTU_RX_PIN        6
#define DTU_RTS_PIN       7

#define DTU_BAUD_RATE     115200
#define DTU_BUF_SIZE      1024

static const char *TAG = "DTU";

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

    uart_driver_install(DTU_UART_PORT, DTU_BUF_SIZE * 2, DTU_BUF_SIZE * 2, 0, NULL, 0);
    uart_param_config(DTU_UART_PORT, &uart_config);

    uart_set_pin(
        DTU_UART_PORT,
        DTU_TX_PIN,
        DTU_RX_PIN,
        DTU_RTS_PIN,
        UART_PIN_NO_CHANGE
    );

    uart_set_mode(DTU_UART_PORT, UART_MODE_RS485_HALF_DUPLEX);

    ESP_LOGI(TAG, "DTU inicializado");

    vTaskDelay(pdMS_TO_TICKS(3000));
}

bool dtu_send_level(uint32_t timestamp, float level_m)
{
    char payload[64];

    snprintf(
        payload,
        sizeof(payload),
        "d=%lu,%.3f",
        (unsigned long)timestamp,
        level_m
    );

    ESP_LOGI(TAG, "Enviando: %s", payload);

    uart_flush_input(DTU_UART_PORT);
    vTaskDelay(pdMS_TO_TICKS(200));

    uart_write_bytes(DTU_UART_PORT, payload, strlen(payload));
    uart_write_bytes(DTU_UART_PORT, "\r\n", 2);

    uart_wait_tx_done(DTU_UART_PORT, pdMS_TO_TICKS(3000));

    uint8_t data[DTU_BUF_SIZE];

    int len = uart_read_bytes(
        DTU_UART_PORT,
        data,
        DTU_BUF_SIZE - 1,
        pdMS_TO_TICKS(DTU_RESPONSE_TIMEOUT_MS)
    );

    if (len > 0) {
    data[len] = '\0';

    ESP_LOGI(TAG, "Respuesta DTU:");
    printf("%s\n", (char *)data);

    if (strstr((char *)data, "OK") != NULL) {
        return true;
    }

    return false;
    }   

    ESP_LOGW(TAG, "Sin respuesta del DTU");
    return false;
}