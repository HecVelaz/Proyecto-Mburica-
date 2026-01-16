#include <ModbusMaster.h>

// --- CONFIGURACIÓN DE PINES ---
#define RX_PIN      5 // Oído del ESP (Conectado al TXD del conversor)
#define TX_PIN      6 // Boca del ESP (Conectado al RXD del conversor)

// ¡IMPORTANTE! DESCOMENTA LA LÍNEA DE ABAJO SI TU CONVERSOR ES MANUAL
// #define MAX485_DE_RE_PIN 7 

// --- CONFIGURACIÓN DEL SENSOR (¡DE TU HOJA DE DATOS!) ---
#define SENSOR_MODBUS_ADDRESS 1
#define SENSOR_BAUD_RATE      9600
#define REGISTER_ADDRESS      0 // Registro a leer (si es 40001 en el manual, aquí es 0)

ModbusMaster node;
HardwareSerial sensorSerial(1);

// --- FUNCIÓN DE CONFIGURACIÓN ---
void setup() {
  Serial.begin(115200);
  while (!Serial); // Espera a que el puerto serie se conecte.
  Serial.println("\n--- Iniciando MAESTRO MODBUS ---");

  #ifdef MAX485_DE_RE_PIN
    // Configura el pin de control solo si está definido
    pinMode(MAX485_DE_RE_PIN, OUTPUT);
    digitalWrite(MAX485_DE_RE_PIN, LOW); // Modo recepción por defecto
  #endif

  sensorSerial.begin(SENSOR_BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);

  node.begin(SENSOR_MODBUS_ADDRESS, sensorSerial);

  #ifdef MAX485_DE_RE_PIN
    // Configura las funciones de control solo si son necesarias
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
  #endif
}

// --- BUCLE PRINCIPAL ---
void loop() {
  uint8_t result;
  
  Serial.println("\n---------------------------------");
  Serial.print("Enviando petición Modbus al esclavo #");
  Serial.println(SENSOR_MODBUS_ADDRESS);
  
  // Pedimos leer 1 registro del tipo "Holding Register"
  result = node.readHoldingRegisters(REGISTER_ADDRESS, 1);

  if (result == node.ku8MBSuccess) {
    Serial.print("¡Éxito! Respuesta recibida del sensor. ");
    
    // 1. Obtenemos el valor crudo como un entero
    uint16_t valorCrudo = node.getResponseBuffer(0);
    Serial.print("Valor (crudo): ");
    Serial.print(valorCrudo);

    // 2. ¡LA MAGIA! Convertimos el valor crudo al valor real dividiendo por 1000
    //    Usamos 1000.0 para forzar una división con decimales.
    float valorReal = valorCrudo / 1000.0;
    
    // 3. Mostramos el valor real
    Serial.print(" | Valor Real (metros): ");
    Serial.println(valorReal, 3); // El '3' es para mostrar 3 decimales
  } else {
    Serial.print("¡FALLO! El sensor no respondió correctamente.");
    Serial.print(" Código de error Modbus: ");
    Serial.print(result, HEX);
    Serial.println(" - Revisa la polaridad A/B, la dirección del esclavo o si necesitas el pin DE/RE.");
  }

  delay(5000); // Espera 5 segundos para la siguiente petición
}

#ifdef MAX485_DE_RE_PIN
// --- FUNCIONES DE CONTROL PARA EL CONVERSOR RS485 (MANUAL) ---
void preTransmission() {
  digitalWrite(MAX485_DE_RE_PIN, HIGH);
  delay(1);
}

void postTransmission() {
  delay(1);
  digitalWrite(MAX485_DE_RE_PIN, LOW);
}
#endif