/*******************************************************
 * CÓDIGO DE PRUEBA DE VOLTAJE ESTÁTICO
 * Su único propósito es poner el GPIO5 en estado ALTO
 * para poder medir su voltaje con un multímetro.
 *******************************************************/

#define PIN_A_MEDIR 5 // Vamos a medir el voltaje en el GPIO5

void setup() {
  // Configura el pin como una salida
  pinMode(PIN_A_MEDIR, OUTPUT);
  
  // Pone el pin en estado ALTO y lo deja ahí para siempre
  digitalWrite(PIN_A_MEDIR, HIGH); 
}

void loop() {
  // No hacemos nada en el loop. El pin se quedará en HIGH.
  delay(1000);
}