Código subido al Arduino , funcional 
[[Final_system(sin drift).ino]]
- **Store & Forward (Guardar y Reenviar):** Si el módem pierde señal, los datos no se pierden; se acumulan en `buffer.csv` y se envían automáticamente cuando vuelve la conexión.
    
- **Gestión de Memoria:** El uso de una arquitectura no bloqueante y el reinicio programado con `ESP.restart()` son prácticas estándar en equipos industriales para evitar bloqueos del sistema.
    
- **Sincronización de Intervalos:** En lugar de usar `delay()`, utiliza el reloj del RTC para disparar la medición exactamente cada 5 minutos.