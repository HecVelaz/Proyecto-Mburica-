#include <Wire.h>
#include <RTClib.h>

RTC_DS3231 rtc;

void setup() {
  Serial.begin(115200);
  while (!Serial); 
  delay(1000);

  if (!rtc.begin()) {
    Serial.println("¡ERROR! No se pudo encontrar el RTC. Verifica el cableado.");
    while (1);
  }

  // --- ¡LA LÍNEA CLAVE! ---
  // Ajusta la hora del RTC a la fecha y hora de la compilación, sin ninguna conversión.
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  Serial.println("¡Reloj DS3231 puesto en hora!");
}

void loop() {
  // Leemos la hora cada segundo para verificar que se ajustó bien
  DateTime ahora = rtc.now();
  
  Serial.print("Hora actual guardada en el RTC: ");
  Serial.print(ahora.year(), DEC); Serial.print('/');
  Serial.print(ahora.month(), DEC); Serial.print('/');
  Serial.print(ahora.day(), DEC); Serial.print(" ");
  Serial.print(ahora.hour(), DEC); Serial.print(':');
  Serial.print(ahora.minute(), DEC); Serial.print(':');
  Serial.println(ahora.second(), DEC);
  
  delay(1000);
}