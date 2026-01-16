#include <HardwareSerial.h>

#define MODEM_RX_PIN  6  // Conectar al TXD del conversor
#define MODEM_TX_PIN  5  // Conectar al RXD del conversor


const int INTERVALO_SEGUNDOS = 30;
const long INTERVALO_MS = INTERVALO_SEGUNDOS * 1000;

HardwareSerial modemSerial(1); // Usamos UART1

void setup() {
  Serial.begin(115200); // Para depuración en PC
  Serial.println("\n--- Prueba con Conversor RS485 Automático (Versión Final) ---");
  
  // Iniciar el puerto serie para el módem
  modemSerial.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  
  Serial.println("Puerto serie del módem inicializado. Enviando datos...");
  delay(5000);
}

void loop() {
  Serial.println("\n----------------------------------------");
  
  // Simular y formatear dato
  float nivelSimulado = 1.0 + (random(0, 1000) / 100.0); 
  String dataParaEnviar = "nivel=" + String(nivelSimulado, 2);
  
  Serial.print("Dato a enviar: '");
  Serial.print(dataParaEnviar);
  Serial.println("'");

  // Enviar los datos. El conversor se encarga del resto.
  modemSerial.print(dataParaEnviar);
  modemSerial.flush(); 

  Serial.println("¡Datos enviados al conversor!");
  
  delay(INTERVALO_MS);
}