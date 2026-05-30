#include <HardwareSerial.h>

#define RX_PIN 4
#define TX_PIN 5

HardwareSerial TestSerial(1);

void setup() {
  Serial.begin(115200);
  TestSerial.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);
  Serial.println("\n--- Prueba de Puente (Loopback) en UART1 ---");
  Serial.println("Conecta el pin 4 directamente con el pin 5.");
  Serial.println("Deberías ver 'Mensaje Recibido' cada 5 segundos.");
}

void loop() {
  String mensaje = "Prueba_UART1_OK";
  Serial.print("\nEnviando: ");
  Serial.println(mensaje);

  TestSerial.print(mensaje);

  delay(100); // Dar un pequeño tiempo para que los datos viajen

  if (TestSerial.available()) {
    String respuesta = TestSerial.readString();
    if (respuesta == mensaje) {
      Serial.println("¡ÉXITO! Mensaje Recibido Correctamente: " + respuesta);
    } else {
      Serial.println("¡ERROR! Se recibió algo inesperado: " + respuesta);
    }
  } else {
    Serial.println("¡FALLO! No se recibió ningún dato. Revisa la conexión del puente.");
  }
  
  delay(5000);
}