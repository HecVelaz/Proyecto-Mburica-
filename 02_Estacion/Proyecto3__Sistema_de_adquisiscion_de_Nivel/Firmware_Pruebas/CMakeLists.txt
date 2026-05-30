cmake_minimum_required(VERSION 3.16)

# Le dice a CMake dónde buscar tus componentes personalizados (led_blink, rtc, sdcard, sensor)
list(APPEND EXTRA_COMPONENT_DIRS "${CMAKE_CURRENT_LIST_DIR}/components")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(ESTACION_LEVEL_ESPIDFSSS)