#include "sdcard.h"

#include <stdio.h>
#include <string.h>

#include "esp_log.h"
#include "esp_vfs_fat.h"

#include "driver/spi_common.h"
#include "driver/sdspi_host.h"

#include "sdmmc_cmd.h"

static const char *TAG = "SDCARD";

static sdmmc_card_t *card = NULL;
static bool sd_mounted = false;

// ======================================================
// DETECTAR TARJETA
// ======================================================

bool detectar_tarjeta(void)
{
    gpio_reset_pin(SD_PIN_DET);
    gpio_set_direction(SD_PIN_DET, GPIO_MODE_INPUT);
    gpio_set_pull_mode(SD_PIN_DET, GPIO_PULLUP_ONLY);

    return gpio_get_level(SD_PIN_DET) == 0;
}

// ======================================================
// CREAR CABECERA DATALOG
// ======================================================

static bool ensure_file_header(void)
{
    FILE *f = fopen(MAIN_LOG_FILE, "r");

    if (f != NULL)
    {
        fclose(f);
        return true;
    }

    f = fopen(MAIN_LOG_FILE, "w");

    if (f == NULL)
    {
        ESP_LOGE(TAG, "No se pudo crear %s", MAIN_LOG_FILE);
        return false;
    }

    fprintf(f, "Timestamp,nivel\n");
    fclose(f);

    ESP_LOGI(TAG, "datalog.csv creado con cabecera");
    return true;
}

// ======================================================
// CREAR CABECERA TEMP
// ======================================================

static bool ensure_temp_header(void)
{
    FILE *f = fopen(TEMP_QUEUE_FILE, "r");

    if (f != NULL)
    {
        fclose(f);
        return true;
    }

    f = fopen(TEMP_QUEUE_FILE, "w");

    if (f == NULL)
    {
        ESP_LOGE(TAG, "No se pudo crear %s", TEMP_QUEUE_FILE);
        return false;
    }

    fprintf(f, "Timestamp,nivel\n");
    fclose(f);

    ESP_LOGI(TAG, "temp.csv creado con cabecera");
    return true;
}

// ======================================================
// INICIALIZAR SD
// ======================================================

bool sdcard_init(void)
{
    if (sd_mounted)
    {
        return true;
    }

    if (!detectar_tarjeta())
    {
        ESP_LOGW(TAG, "No hay tarjeta SD detectada");
        return false;
    }

    esp_err_t ret;

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
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

    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE)
    {
        ESP_LOGE(TAG, "Error inicializando SPI: %s", esp_err_to_name(ret));
        return false;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = SD_PIN_CS;
    slot_config.host_id = host.slot;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    ret = esp_vfs_fat_sdspi_mount(
        MOUNT_POINT,
        &host,
        &slot_config,
        &mount_config,
        &card
    );

    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Error montando SD: %s", esp_err_to_name(ret));
        return false;
    }

    sd_mounted = true;

    ESP_LOGI(TAG, "SD montada correctamente");
    sdmmc_card_print_info(stdout, card);

    if (!ensure_file_header())
    {
        return false;
    }

    if (!ensure_temp_header())
    {
        return false;
    }

    return true;
}

// ======================================================
// GUARDAR EN DATALOG
// ======================================================

bool sdcard_append_level(uint32_t timestamp, float level_m)
{
    if (!sd_mounted)
    {
        ESP_LOGW(TAG, "SD no montada");
        return false;
    }

    FILE *f = fopen(MAIN_LOG_FILE, "a");

    if (f == NULL)
    {
        ESP_LOGE(TAG, "No se pudo abrir datalog.csv");
        return false;
    }

    fprintf(f, "%lu,%.3f\n", (unsigned long)timestamp, level_m);
    fclose(f);

    ESP_LOGI(TAG, "Guardado SD: %lu,%.3f",
             (unsigned long)timestamp,
             level_m);

    return true;
}

// ======================================================
// GUARDAR PENDIENTE EN TEMP
// ======================================================

bool sdcard_append_pending(uint32_t timestamp, float level_m)
{
    if (!sd_mounted)
    {
        ESP_LOGW(TAG, "SD no montada");
        return false;
    }

    if (!ensure_temp_header())
    {
        return false;
    }

    FILE *f = fopen(TEMP_QUEUE_FILE, "a");

    if (f == NULL)
    {
        ESP_LOGE(TAG, "No se pudo abrir temp.csv");
        return false;
    }

    fprintf(f, "%lu,%.3f\n", (unsigned long)timestamp, level_m);
    fclose(f);

    ESP_LOGW(TAG, "Guardado pendiente: %lu,%.3f",
             (unsigned long)timestamp,
             level_m);

    return true;
}

// ======================================================
// VERIFICAR SI HAY PENDIENTES
// ======================================================

bool sdcard_has_pending(void)
{
    if (!sd_mounted)
    {
        return false;
    }

    if (!ensure_temp_header())
    {
        return false;
    }

    FILE *f = fopen(TEMP_QUEUE_FILE, "r");

    if (f == NULL)
    {
        return false;
    }

    char line[128];

    // Saltar cabecera
    fgets(line, sizeof(line), f);

    bool has_data = false;

    while (fgets(line, sizeof(line), f))
    {
        if (strlen(line) > 3)
        {
            has_data = true;
            break;
        }
    }

    fclose(f);
    return has_data;
}

// ======================================================
// LEER PRIMER PENDIENTE FIFO
// ======================================================

bool sdcard_read_first_pending(uint32_t *timestamp, float *level_m)
{
    if (!sd_mounted || timestamp == NULL || level_m == NULL)
    {
        return false;
    }

    if (!ensure_temp_header())
    {
        return false;
    }

    FILE *f = fopen(TEMP_QUEUE_FILE, "r");

    if (f == NULL)
    {
        return false;
    }

    char line[128];

    // Saltar cabecera
    fgets(line, sizeof(line), f);

    bool ok = false;
    unsigned long ts_temp;
    float level_temp;

    while (fgets(line, sizeof(line), f))
    {
        if (sscanf(line, "%lu,%f", &ts_temp, &level_temp) == 2)
        {
            *timestamp = (uint32_t)ts_temp;
            *level_m = level_temp;
            ok = true;
            break;
        }
    }

    fclose(f);
    return ok;
}

// ======================================================
// ELIMINAR PRIMER PENDIENTE FIFO
// ======================================================

bool sdcard_remove_first_pending(void)
{
    if (!sd_mounted)
    {
        return false;
    }

    FILE *src = fopen(TEMP_QUEUE_FILE, "r");

    if (src == NULL)
    {
        return false;
    }

    FILE *dst = fopen("/sdcard/temp_aux.csv", "w");

    if (dst == NULL)
    {
        fclose(src);
        return false;
    }

    char line[128];

    // Copiar cabecera
    if (fgets(line, sizeof(line), src))
    {
        fputs(line, dst);
    }
    else
    {
        fprintf(dst, "Timestamp,nivel\n");
    }

    bool skipped = false;

    while (fgets(line, sizeof(line), src))
    {
        if (!skipped && strlen(line) > 3)
        {
            skipped = true;
            continue;
        }

        fputs(line, dst);
    }

    fclose(src);
    fclose(dst);

    remove(TEMP_QUEUE_FILE);

    if (rename("/sdcard/temp_aux.csv", TEMP_QUEUE_FILE) != 0)
    {
        ESP_LOGE(TAG, "No se pudo renombrar temp_aux.csv");
        return false;
    }

    ESP_LOGI(TAG, "Primer pendiente eliminado");

    return true;
}