# Evidencias de pruebas del sistema

En esta carpeta se presentan evidencias del funcionamiento del sistema de adquisición de nivel desarrollado. Las pruebas incluyen la verificación del LED indicador, detección de tarjeta microSD, lectura del RTC DS3231, funcionamiento integrado del sistema y recuperación de datos pendientes mediante el mecanismo **Store and Forward**.

Los videos se encuentran alojados en Google Drive para evitar aumentar el tamaño del repositorio.

## Videos de pruebas

| N.º | Prueba                       | Descripción                                                                                                                                                                                                                                       | Enlace                                                                                             |
| --- | ---------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------- |
| 1   | Prueba del LED azul          | Se realiza una prueba simple del LED azul utilizado como indicador visual. Este LED permite verificar que el sistema detectó correctamente una lectura válida del sensor ultrasónico de nivel.                                                    | [Ver video](https://drive.google.com/file/d/1yZTbWLAMQBHtrM2hwZs7zqRO-RH48-HB/view?usp=drive_link) |
| 2   | Prueba de tarjeta microSD    | Se verifica la detección de la tarjeta microSD insertada en el sistema. Esta prueba permite comprobar el funcionamiento del conector microSD y la capacidad del firmware para reconocer la presencia de la tarjeta.                               | [Ver video](https://drive.google.com/file/d/19R5aCAA-6pBu098gMMyVXceTk3J7X_a-/view?usp=drive_link) |
| 3   | Prueba del RTC DS3231        | Se muestra la configuración de la hora en el módulo RTC DS3231 y la lectura correcta de la fecha y hora mediante comunicación I2C. Esta prueba permite validar que el sistema puede generar marcas de tiempo para cada medición.                  | [Ver video](https://drive.google.com/file/d/1AH5-atD6chmUB5mobT-IkS_jdaVtZ49u/view?usp=drive_link) |
| 4   | Prueba del sistema integrado | Se observa el funcionamiento conjunto del sensor de nivel RS485, RTC DS3231, tarjeta microSD, módem DTU, LED indicador y firmware principal. Esta prueba demuestra la lectura del nivel, el almacenamiento local y el envío de datos al servidor. | [Ver video](https://drive.google.com/file/d/1fyCeK_l0-00xuTfR_rccq4oqjixhjDV9/view?usp=drive_link) |
| 5   | Prueba Store and Forward     | Se evidencia el funcionamiento del mecanismo **Store and Forward**. Cuando ocurre una falla de comunicación, el dato se almacena temporalmente como pendiente y posteriormente es reenviado al servidor cuando la comunicación se restablece.     | [Ver video](https://drive.google.com/file/d/12kTLneO2Io2DWGPuWARCHy4YUeFoKbM6/view?usp=drive_link) |

## Observación

Estas evidencias complementan las pruebas documentadas en el informe técnico del proyecto. Los videos permiten verificar visualmente el comportamiento de los módulos principales y del sistema completo durante las etapas de prueba e integración.
