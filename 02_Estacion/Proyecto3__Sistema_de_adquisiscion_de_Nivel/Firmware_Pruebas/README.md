# Firmware de Pruebas

Esta carpeta contiene los programas desarrollados durante la etapa de validación de hardware y software del proyecto.

## Objetivo

Cada proyecto incluido en esta carpeta fue utilizado para verificar individualmente el correcto funcionamiento de los distintos módulos antes de su integración en el firmware final.

## Pruebas realizadas

### RTC DS3231M

* Configuración de fecha y hora.
* Lectura de fecha y hora.
* Verificación de persistencia temporal.

### Tarjeta microSD

* Inicialización del módulo SD.
* Detección de inserción de tarjeta.
* Creación y lectura de archivos.
* Verificación del sistema de almacenamiento.

### Sensor de nivel RS485

* Comunicación RS485.
* Recepción y validación de tramas.
* Lectura de nivel en metros.

### Módem DTU GPRS

* Configuración mediante comandos AT.
* Verificación de conectividad.
* Envío de datos al servidor.

### Indicadores LED

* Verificación de funcionamiento de LEDs de estado.

## Importante

Estos códigos corresponden a pruebas aisladas realizadas durante el desarrollo y no representan la versión final utilizada en la estación de adquisición de nivel.

La versión definitiva del sistema se encuentra en la carpeta `Firmware_Final`.
## Modo de uso

Los distintos programas de prueba fueron integrados dentro del mismo proyecto y se ejecutan desde el archivo principal `main.c`.

Para ejecutar una prueba específica es necesario habilitar (descomentar) las líneas correspondientes dentro de la función principal y deshabilitar aquellas asociadas a otras pruebas.

Ejemplo:

```c
// Inicialización RTC
rtc_init();

// Inicialización microSD
sdcard_init();

// Prueba del sensor RS485
sensor_rs485_init();
```

Dependiendo de la prueba que se desee realizar, algunas llamadas a funciones deben comentarse o descomentarse manualmente antes de compilar y cargar el firmware en el ESP32-S3.

Esta metodología fue utilizada durante el desarrollo para validar individualmente cada módulo de hardware antes de integrarlo en el firmware final.

```
```
