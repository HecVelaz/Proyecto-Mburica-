idf_component_register(
    SRCS "sdcard.c"
    INCLUDE_DIRS "include"
    REQUIRES fatfs sdmmc esp_driver_spi esp_driver_gpio
)