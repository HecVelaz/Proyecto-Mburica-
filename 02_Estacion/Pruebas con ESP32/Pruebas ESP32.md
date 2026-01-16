## Propósito
Pruebas por partes con el esp32

## Contenido


- [[Inicializar_Reloj.ino]]
	- **Sincronización RTC DS3231** Configura la hora del módulo hardware usando la fecha de compilación para garantizar el _timestamp_ de los datos.
- [[moden_esp_sensor_2.ino]]
	- **Lectura Modbus y Envío GPRS (No Bloqueante)** Implementa la lectura de un sensor de nivel mediante protocolo Modbus RS485 y transmite los datos vía módem cada 5 minutos. Utiliza una estructura basada en `millis()`
-  [[esp32_comunication.ino]]
	- **Prueba de Puente (Loopback) UART1** Verifica la integridad del hardware de comunicación mediante un puente físico entre los pines RX y TX. Se utiliza para descartar fallas en los puertos del ESP32 antes de conectar el módem o el sensor, validando el envío y recepción de datos.
- [[prueba_led_esp32.ino]]
	- **Transmisión con Conversor RS485 Automático** Simula y envía datos de nivel formateados a través de UART1 hacia un conversor RS485 con control de flujo automático. Permite validar la cadena de transmisión de datos hacia el módem cada 30 segundos, simplificando el hardware al delegar la conmutación de datos al conversor.
- [[voltaje_pin.ino]]
	- **Prueba de Voltaje Estático (Hardware Debug)** Configura el GPIO5 como salida en estado ALTO permanente para permitir la medición física de voltaje con un multímetro. Se utiliza para validar niveles lógicos y verificar la correcta alimentación de los pines antes de conectar periféricos sensibles.
- [[prueba_moden_esp_sensor.ino]]
	- **Sistema Final Unificado (Telemetría GPRS/Modbus)** Integra la lectura de nivel mediante protocolo Modbus RS485 y la transmisión de datos vía módem GPRS. Realiza el procesamiento completo del dato (extracción de bytes, conversión a metros y validación binaria) con un ciclo de envío y espera de confirmación del servidor cada 35 segundos.
	
- [[Prueba_SDcard.ino]]
	- **Verificador de Almacenamiento SD (Bus SPI)** Valida la comunicación con el módulo de tarjeta SD utilizando el bus SPI para el almacenamiento local de respaldo. Realiza un ciclo completo de apertura, escritura, lectura y verificación de archivos para asegurar que los datos del sensor se guarden correctamente en caso de fallas en la red GPRS.
- [[prueba_Sensor_esp32.ino]]
	- **Maestro Modbus RS485 (Lectura Directa)** Establece la comunicación maestro-esclavo para extraer datos crudos de registros internos del sensor de nivel. Incluye la lógica de conversión de unidades (milímetros a metros) y soporte opcional para el control de flujo manual (DE/RE) en conversores RS485.
- [[sistema_con_reloj.ino]]
	- **Telemetría Integrada con Timestamp Unix (RTC + Modbus + GPRS)** Integra la lectura del sensor Modbus con el tiempo real del RTC DS3231, convirtiendo la fecha a formato Unix (UTC-3) para el sellado de tiempo. Envía una cadena de datos unificada ("timestamp,valor") vía GPRS cada minuto
- [[Final_system(sin drift).ino]]
	- **Sistema de Alta Disponibilidad con _Store & Forward_** Implementa un datalogger profesional que asegura la integridad de los datos mediante una doble estrategia: guarda mediciones en un log maestro permanente (SD) y gestiona un archivo _buffer_ para reintentar envíos fallidos al servidor. Incluye una rutina de reinicio automático diario (02:02 AM) para prevenir la fragmentación de memoria y asegurar la estabilidad del sistema a largo plazo.