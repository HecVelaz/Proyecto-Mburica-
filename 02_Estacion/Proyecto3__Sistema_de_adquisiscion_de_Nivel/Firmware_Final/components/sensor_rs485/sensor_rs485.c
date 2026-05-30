#include "sensor_rs485.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_rom_sys.h"

// ======================================================
// PINES SENSOR RS485 NIVEL
// ======================================================

#define SENSOR_UART_PORT UART_NUM_1
#define SENSOR_TX_PIN    GPIO_NUM_1
#define SENSOR_RX_PIN    GPIO_NUM_2
#define SENSOR_DE_RE_PIN GPIO_NUM_4

#define SENSOR_BAUD_RATE 9600
#define SENSOR_BUF_SIZE  256

static const char *TAG = "SENSOR_RS485";

// Comando Modbus: ID 1, función 03, registro 0x0000, cantidad 1
static const uint8_t CMD_READ_LEVEL[] = {
    0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A
};

bool sensor_rs485_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = SENSOR_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_driver_install(SENSOR_UART_PORT, SENSOR_BUF_SIZE * 2, 0, 0, NULL, 0);

    uart_param_config(SENSOR_UART_PORT, &uart_config);

    uart_set_pin(
        SENSOR_UART_PORT,
        SENSOR_TX_PIN,
        SENSOR_RX_PIN,
        UART_PIN_NO_CHANGE,
        UART_PIN_NO_CHANGE
    );

    gpio_reset_pin(SENSOR_DE_RE_PIN);

    gpio_set_direction(
        SENSOR_DE_RE_PIN,
        GPIO_MODE_OUTPUT
    );

    gpio_set_level(SENSOR_DE_RE_PIN, 0);

    ESP_LOGI(TAG, "Sensor RS485 inicializado");

    // ==============================
    // Lectura de descarte
    // ==============================

    float dummy;

    sensor_rs485_read_level(&dummy);

    vTaskDelay(pdMS_TO_TICKS(500));

    return true;
}

bool sensor_rs485_read_level(float *level_m)
{
    if (level_m == NULL) return false;

    uint8_t buffer[32];

    uart_flush(SENSOR_UART_PORT);
    uart_flush_input(SENSOR_UART_PORT);
    vTaskDelay(pdMS_TO_TICKS(50));

    gpio_set_level(SENSOR_DE_RE_PIN, 1);
    esp_rom_delay_us(300);

    uart_write_bytes(
        SENSOR_UART_PORT,
        (const char *)CMD_READ_LEVEL,
        sizeof(CMD_READ_LEVEL)
    );

    uart_wait_tx_done(SENSOR_UART_PORT, pdMS_TO_TICKS(15));

    gpio_set_level(SENSOR_DE_RE_PIN, 0);
    esp_rom_delay_us(300);

    int len = uart_read_bytes(
        SENSOR_UART_PORT,
        buffer,
        sizeof(buffer),
        pdMS_TO_TICKS(800)
    );

    if (len < 5) {
        ESP_LOGW(TAG, "Sin respuesta válida del sensor");
        return false;
    }

    if (buffer[0] != 0x01 || buffer[1] != 0x03) {
        ESP_LOGW(TAG, "Respuesta inesperada");
        ESP_LOG_BUFFER_HEX(TAG, buffer, len);
        return false;
    }

    uint16_t raw = ((uint16_t)buffer[3] << 8) | buffer[4];

    *level_m = raw / 1000.0f;

    ESP_LOGI(TAG, "Nivel leído: %.3f m", *level_m);

    return true;
}