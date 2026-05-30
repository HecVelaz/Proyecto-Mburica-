#include "led_blink.h"
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *TAG = "BLINK_TEST";

void parpadear_led(void) {
    ESP_LOGI(TAG, "¡Iniciando prueba de Blink en ESP-IDF!");

    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    int estado_led = 0;

    while (1) {
        gpio_set_level(BLINK_GPIO, estado_led);
        
        ESP_LOGI(TAG, "El LED del GPIO21 está: %s", estado_led ? "ENCENDIDO" : "APAGADO");

        estado_led = !estado_led;
        vTaskDelay(pdMS_TO_TICKS(1000)); // Espera 1 segundo
    }
}