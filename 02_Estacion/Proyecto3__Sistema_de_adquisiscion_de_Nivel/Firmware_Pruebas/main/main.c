

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "esp_log.h"

// --- IMPORTACIÓN DE TUS COMPONENTES ---
#include "led_blink.h" 
#include "sdcard.h"    
#include "reloj.h"
#include "dtu.h"


/*
//extern void probar_sensor_ultrasonic(void);
void app_main(void) {
    // Espera obligatoria de 2 segundos para conectar la terminal USB de la PC
   
 //    printf("Inicio\n");
    // =================================================================
    // 🛠️ ZONA DE PRUEBAS: Descomenta (quita el //) SOLO UNA a la vez
    // =================================================================
    
     //parpadear_led();     // <-- Prueba 1: LED Blink
    
     //  Probar el Almacenamiento SPI
    //if (detectar_tarjeta()) {

      //  ESP_LOGI("MAIN", "SD INSERTADA");

   // } else {

     //   ESP_LOGI("MAIN", "SD NO INSERTADA");
   // }
     //probar_modem_dtu();
     
    //inicializar_rtc_ds3231();
  //  reloj_ajustar_hora_compilacion();
    //leer_hora_rtc_ds3231();
    // =================================================================
    //probar_sensor_ultrasonic();

}  */


void app_main(void)
{
    dtu_init();

    vTaskDelay(pdMS_TO_TICKS(8000));

    dtu_enviar_dato("d=1713312000,11.56");

    dtu_leer_respuesta(15000);
}




// ==========================================
//        Detectar si hay tarjeta SD, ESCRITURA Y LECTURA
// ==========================================
/*
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"

#include "sdcard.h"

void app_main(void)
{
    while (1)
    {
        // ==========================================
        // Detectar si hay tarjeta SD
        // ==========================================

        if (detectar_tarjeta())
        {
            ESP_LOGI("MAIN",
                     "Hay SD, puedo montar y guardar datos");

            // Aquí llamas tu función de prueba
            verificar_sd_card();
        }
        else
        {
            ESP_LOGW("MAIN",
                     "No hay SD, no intento montar");
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
} */


/*
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdio.h>
#include "esp_log.h"

// --- IMPORTACIÓN DE TUS COMPONENTES ---

#include "led_blink.h" 
#include "sdcard.h"    
#include "reloj.h"
#include "dtu.h"
//extern void probar_sensor_ultrasonic(void);
void app_main(void) {
    // Espera obligatoria de 2 segundos para conectar la terminal USB de la PC
   
 //    printf("Inicio\n");
    // =================================================================
    // 🛠️ ZONA DE PRUEBAS: Descomenta (quita el //) SOLO UNA a la vez
    // =================================================================
    
     parpadear_led();     
}*/