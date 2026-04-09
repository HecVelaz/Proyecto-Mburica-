#include <HardwareSerial.h>

// --- PINES (para un conversor automático) ---
#define MODEM_RX_PIN  19  // Conectar al TXD del conversor (Oído del ESP)
#define MODEM_TX_PIN  18  // Conectar al RXD del conversor (Boca del ESP)

const int INTERVALO_SEGUNDOS = 30;
const long INTERVALO_MS = INTERVALO_SEGUNDOS * 1000;

HardwareSerial modemSerial(1); // Usamos UART1

void setup() {
  Serial.begin(115200); // Para depuración en PC
  modemSerial.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  Serial.println("\n--- Sistema de Envío con Confirmación ---");
  delay(5000);
}

void loop() {
  Serial.println("\n----------------------------------------");
  
  // 1. Preparar el dato
  float nivelSimulado = 1.0 + (random(0, 1000) / 100.0); 
  String dataParaEnviar = "nivel=" + String(nivelSimulado, 2);
  
  Serial.print("Intentando enviar: '");
  Serial.print(dataParaEnviar);
  Serial.println("'");

  // 2. Enviar los datos al módem
  modemSerial.print(dataParaEnviar);
  modemSerial.flush(); 
  Serial.println("Dato enviado al conversor. Esperando respuesta del servidor...");

  // 3. ¡NUEVO! Escuchar la respuesta
  bool respuestaRecibida = false;
  unsigned long tiempoInicio = millis();
  String respuestaServidor = "";

  // Esperaremos hasta 10 segundos por una respuesta
  while (millis() - tiempoInicio < 10000 && !respuestaRecibida) {
    if (modemSerial.available()) {
      respuestaServidor = modemSerial.readString(); // Leer todo lo que llegue
      respuestaRecibida = true;
    }
  }

  // 4. Procesar el resultado
  if (respuestaRecibida) {
    Serial.print("Respuesta recibida del servidor: ");
    Serial.println(respuestaServidor);
    // Aquí podrías añadir lógica más específica. Por ejemplo:
    if (respuestaServidor.indexOf("NIVEL_REGISTRADO") != -1) {
      Serial.println("¡ÉXITO! El servidor confirmó el registro.");
    } else {
      Serial.println("ADVERTENCIA: Se recibió una respuesta inesperada.");
    }
  } else {
    Serial.println("¡ERROR! No se recibió respuesta del servidor en 10 segundos.");
  }

  // 5. Esperar para el siguiente ciclo
  Serial.print("Esperando ");
  Serial.print(INTERVALO_SEGUNDOS);
  Serial.println(" segundos...");
  delay(INTERVALO_MS);
}