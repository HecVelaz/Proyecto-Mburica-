#ifndef RTC_H
#define RTC_H

#include "driver/gpio.h"
#include "driver/i2c.h" // Necesario para la configuración I2C del RTC

// 📌 CONFIGURACIÓN DE PINES (Igual a tu imagen)
#define RTC_I2C_SDA      GPIO_NUM_8
#define RTC_I2C_SCL      GPIO_NUM_9
#define RTC_SQW_INT      GPIO_NUM_16

// ⚙️ OTRAS CONFIGURACIONES ÚTILES PARA EL RTC
#define I2C_MASTER_NUM   I2C_NUM_0      // Puerto I2C que usaremos
#define I2C_MASTER_FREQ_HZ 100000       // Frecuencia de reloj para I2C (100kHz estándar para DS3231)

// 🚀 DECLARACIÓN DE LA FUNCIÓN PÚBLICA
// Al declararla aquí, cualquier archivo que incluya "rtc.h" podrá usarla.

void inicializar_rtc_ds3231(void);
void reloj_ajustar_hora_compilacion(void);
void leer_hora_rtc_ds3231(void);
#endif // RTC_H