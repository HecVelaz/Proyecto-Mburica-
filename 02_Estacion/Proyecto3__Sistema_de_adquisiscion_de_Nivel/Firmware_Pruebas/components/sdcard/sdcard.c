#include "sdcard.h"

#include <stdio.h>

#include "esp_log.h"
#include "esp_vfs_fat.h"

#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/sdspi_host.h"

#include "sdmmc_cmd.h"

static const char *TAG = "SDCARD";

// ======================================================
// DETECTAR TARJETA SD
// ======================================================

bool detectar_tarjeta(void)
{
    // Configurar GPIO15 como entrada pull-up
    gpio_reset_pin(SD_PIN_DET);

    gpio_set_direction(
        SD_PIN_DET,
        GPIO_MODE_INPUT
    );

    gpio_set_pull_mode(
        SD_PIN_DET,
        GPIO_PULLUP_ONLY
    );

    // Leer estado
    int estado = gpio_get_level(SD_PIN_DET);

    // 0 = tarjeta insertada
    // 1 = sin tarjeta

    return (estado == 0);
}

// ======================================================
// VERIFICAR SD CARD
// Inicializa, monta, escribe y lee archivo
// ======================================================

esp_err_t verificar_sd_card(void)
{
    esp_err_t ret;

    sdmmc_card_t *card;

    ESP_LOGI(TAG, "================================");
    ESP_LOGI(TAG, "INICIANDO PRUEBA SD");
    ESP_LOGI(TAG, "================================");

    // ==================================================
    // CONFIGURAR BUS SPI
    // ==================================================

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    // Frecuencia SPI
    host.max_freq_khz = 10000;

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = SD_PIN_MOSI,
        .miso_io_num = SD_PIN_MISO,
        .sclk_io_num = SD_PIN_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000
    };

    ret = spi_bus_initialize(
        host.slot,
        &bus_cfg,
        SDSPI_DEFAULT_DMA
    );

    // Si el SPI ya estaba iniciado no es error fatal
    if (ret != ESP_OK &&
        ret != ESP_ERR_INVALID_STATE)
    {
        ESP_LOGE(TAG,
                 "Error inicializando SPI: %s",
                 esp_err_to_name(ret));

        return ret;
    }

    ESP_LOGI(TAG, "SPI OK");

    // ==================================================
    // CONFIGURAR SD
    // ==================================================

    sdspi_device_config_t slot_config =
        SDSPI_DEVICE_CONFIG_DEFAULT();

    slot_config.gpio_cs = SD_PIN_CS;
    slot_config.host_id = host.slot;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    // ==================================================
    // MONTAR SD
    // ==================================================

    ESP_LOGI(TAG, "Montando SD...");

    ret = esp_vfs_fat_sdspi_mount(
        MOUNT_POINT,
        &host,
        &slot_config,
        &mount_config,
        &card
    );

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG,
                 "Error montando SD: %s",
                 esp_err_to_name(ret));

        return ret;
    }

    ESP_LOGI(TAG, "SD montada correctamente");

    // Mostrar información de la tarjeta
    sdmmc_card_print_info(stdout, card);

    // ==================================================
    // CREAR ARCHIVO
    // ==================================================

    ESP_LOGI(TAG, "Creando archivo...");

    FILE *f = fopen("/sdcard/test.txt", "w");

    if (f == NULL)
    {
        ESP_LOGE(TAG,
                 "No se pudo crear archivo");

        esp_vfs_fat_sdcard_unmount(
            MOUNT_POINT,
            card
        );

        return ESP_FAIL;
    }

    fprintf(f, "Hola desde ESP32-S3\n");
    fprintf(f, "Prueba SD correcta\n");

    fclose(f);

    ESP_LOGI(TAG, "Archivo escrito");

    // ==================================================
    // LEER ARCHIVO
    // ==================================================

    ESP_LOGI(TAG, "Leyendo archivo...");

    f = fopen("/sdcard/test.txt", "r");

    if (f == NULL)
    {
        ESP_LOGE(TAG,
                 "No se pudo leer archivo");

        esp_vfs_fat_sdcard_unmount(
            MOUNT_POINT,
            card
        );

        return ESP_FAIL;
    }

    char linea[128];

    printf("\n========== CONTENIDO ==========\n");

    while (fgets(linea, sizeof(linea), f))
    {
        printf("%s", linea);
    }

    printf("========== FIN ==========\n\n");

    fclose(f);

    ESP_LOGI(TAG, "Lectura OK");

    // ==================================================
    // DESMONTAR SD
    // ==================================================

    esp_vfs_fat_sdcard_unmount(
        MOUNT_POINT,
        card
    );

    ESP_LOGI(TAG, "SD desmontada");

    ESP_LOGI(TAG, "PRUEBA FINALIZADA OK");

    return ESP_OK;
}