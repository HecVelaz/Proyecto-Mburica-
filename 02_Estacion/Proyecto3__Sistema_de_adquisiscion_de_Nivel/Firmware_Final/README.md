# Firmware Final

Esta carpeta contiene la versión final del firmware desarrollado para la estación de adquisición de nivel.

## Descripción

El firmware fue desarrollado utilizando **PlatformIO** y el framework **ESP-IDF** para el microcontrolador **ESP32-S3-WROOM-1**.

Las principales funciones implementadas son:

* Lectura del sensor ultrasónico de nivel mediante comunicación RS485.
* Obtención de fecha y hora mediante el RTC DS3231M.
* Almacenamiento local de mediciones en tarjeta microSD.
* Envío de datos al servidor remoto mediante módem DTU GPRS.
* Implementación del mecanismo **Store and Forward** para recuperación automática de datos pendientes.
* Ejecución de tareas en segundo plano mediante FreeRTOS.
* Protección de recursos compartidos mediante mutex.
* Indicadores visuales mediante LED de estado.

## Estructura principal

* `src/` : Código fuente principal.
* `components/` : Módulos desarrollados para sensor, RTC, SD, DTU y periféricos.
* `include/` : Archivos de cabecera.
* `platformio.ini` : Configuración del proyecto PlatformIO.

## Estado

Versión funcional validada mediante pruebas integradas de lectura, almacenamiento local, transmisión remota y recuperación de datos pendientes.
## Nota

A diferencia de la carpeta `Firmware_Pruebas`, el firmware contenido en esta carpeta corresponde a la versión final integrada del sistema y no requiere habilitar o deshabilitar módulos manualmente para su funcionamiento normal.
