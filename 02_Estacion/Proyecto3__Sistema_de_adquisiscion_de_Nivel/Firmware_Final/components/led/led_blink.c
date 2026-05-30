#include "led_blink.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

static const char *TAG = "LED";

// ======================================================
// INICIALIZAR LED
// ======================================================

void led_init(void)
{
    gpio_reset_pin(LED_BLINK_GPIO);

    gpio_set_direction(
        LED_BLINK_GPIO,
        GPIO_MODE_OUTPUT
    );

    gpio_set_level(
        LED_BLINK_GPIO,
        0
    );

    ESP_LOGI(TAG, "LED inicializado en GPIO21");
}

// ======================================================
// BLINK CUANDO LECTURA SENSOR ES OK
// ======================================================

void led_blink_sensor_ok(void)
{
    gpio_set_level(
        LED_BLINK_GPIO,
        1
    );

    vTaskDelay(
        pdMS_TO_TICKS(LED_BLINK_TIME_MS)
    );

    gpio_set_level(
        LED_BLINK_GPIO,
        0
    );
}