#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

// 📌 DEFINICIÓN DE PINES SEGÚN EL ESQUEMÁTICO REAL
#define UART_PORT         UART_NUM_1
#define PIN_TX            GPIO_NUM_1   // Pin físico 39 (DI1)
#define PIN_RX            GPIO_NUM_2   // Pin físico 38 (RO1)
#define PIN_DE_RE         GPIO_NUM_4   // Pin físico 4  (DE/RE 1)

static const char *TAG = "RADAR_SENSOR";

void probar_sensor_ultrasonic(void) {
    ESP_LOGI(TAG, "Inicializando UART_1 (TX: GPIO1, RX: GPIO2, DE/RE: GPIO4)...");

    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    // Configurar e instalar el driver de la UART con los GPIO_NUM_X correctos
    uart_param_config(UART_PORT, &uart_config);
    uart_set_pin(UART_PORT, PIN_TX, PIN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(UART_PORT, 256, 0, 0, NULL, 0);

    // Configurar el pin de control de flujo DE/RE 1
    gpio_reset_pin(PIN_DE_RE);
    gpio_set_direction(PIN_DE_RE, GPIO_MODE_OUTPUT);
    gpio_set_level(PIN_DE_RE, 0); // Modo escucha por defecto

    // Comando Modbus RTU estándar para interrogar al registro 0
    const uint8_t comando_lectura[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};

    while (1) {
        // Limpiar basura acumulada
        uart_flush_input(UART_PORT);

        // 1. Activar Transmisión (DE/RE 1 = HIGH)
        gpio_set_level(PIN_DE_RE, 1);
        esp_rom_delay_us(50); 

        // Enviar comando Modbus
        uart_write_bytes(UART_PORT, (const char *)comando_lectura, sizeof(comando_lectura));
        
        // Esperar la salida física por hardware
        uart_wait_tx_done(UART_PORT, pdMS_TO_TICKS(15));

        // 2. Activar Escucha (DE/RE 1 = LOW)
        gpio_set_level(PIN_DE_RE, 0);
        esp_rom_delay_us(50); 

        // Leer buffer de respuesta
        uint8_t buffer[32];
        int bytes_leidos = uart_read_bytes(UART_PORT, buffer, sizeof(buffer), pdMS_TO_TICKS(300));

        if (bytes_leidos > 0) {
            if (buffer[0] == 0x01 && buffer[1] == 0x03) {
                ESP_LOGW(TAG, "¡Trama Modbus válida capturada!");
                ESP_LOG_BUFFER_HEX(TAG, buffer, bytes_leidos);

                if (bytes_leidos >= 5) {
                    // Procesar los datos crudos del registro 0
                    uint16_t valorCrudo = (buffer[3] << 8) | buffer[4];
                    float valorReal = valorCrudo / 1000.0;
                    
                    ESP_LOGW(TAG, "=======================================");
                    ESP_LOGW(TAG, "-> Distancia Real: %.3f metros", valorReal);
                    ESP_LOGW(TAG, "=======================================");
                }
            } else {
                ESP_LOGE(TAG, "Respuesta inesperada en el bus:");
                ESP_LOG_BUFFER_HEX(TAG, buffer, bytes_leidos);
            }
        } else {
            ESP_LOGE(TAG, "Sin respuesta del radar (Timeout). Verificando línea física.");
        }

        vTaskDelay(pdMS_TO_TICKS(4000));
    }
}