idf_component_register(SRCS "sensor.c"
                       INCLUDE_DIRS "."
                       REQUIRES driver esp_rom esp_driver_uart)