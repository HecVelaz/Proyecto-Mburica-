#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>

// --- TODAS TUS CONFIGURACIONES DE PINES Y PARÁMETROS VAN AQUÍ... ---
#define MODEM_RX_PIN  19
#define MODEM_TX_PIN  18
HardwareSerial modemSerial(1);

#define SENSOR_RX_PIN      5
#define SENSOR_TX_PIN      6
#define SENSOR_DE_RE_PIN   7
SoftwareSerial sensorSerial;

#define SENSOR_MODBUS_ADDRESS 1
#define SENSOR_BAUD_RATE      9600
#define REGISTER_ADDRESS      0 
ModbusMaster node;

// --- ¡NUEVO! PARÁMETROS DE TEMPORIZACIÓN ---
const long INTERVALO_MS = 300000; // 5 minutos en milisegundos.
unsigned long ultimoEnvio = 0;   // Variable para guardar la hora del último envío.

// --- FUNCIÓN DE CONFIGURACIÓN (sigue siendo la misma) ---
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n--- INICIANDO SISTEMA CON INTERVALO NO BLOQUEANTE ---");

  modemSerial.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  sensorSerial.begin(SENSOR_BAUD_RATE, SWSERIAL_8N1, SENSOR_RX_PIN, SENSOR_TX_PIN);
  node.begin(SENSOR_MODBUS_ADDRESS, sensorSerial);

  pinMode(SENSOR_DE_RE_PIN, OUTPUT);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  Serial.println("Configuración completa. El sistema está activo.");
}

// --- BUCLE PRINCIPAL (ESTRUCTURA NO BLOQUEANTE) ---
void loop() {
  // En cada pasada del loop, preguntamos:
  // ¿Ha pasado suficiente tiempo desde el último envío?
  if (millis() - ultimoEnvio >= INTERVALO_MS) {
    
    // Si la respuesta es SÍ, es hora de actuar.
    
    // 1. Actualizamos la hora del "último envío" para el próximo ciclo.
    ultimoEnvio = millis();
    
    Serial.println("\n==============================================");
    Serial.println("¡Intervalo de 10 minutos cumplido! Iniciando ciclo...");

    // 2. Leemos el sensor
    uint8_t result = node.readHoldingRegisters(REGISTER_ADDRESS, 1);
    
    if (result == node.ku8MBSuccess) {
      // 3. Si la lectura es exitosa, procesamos y enviamos el dato
      uint16_t valorCrudo = node.getResponseBuffer(0);
      float valorReal = valorCrudo / 1000.0;
      
      String dataParaEnviar = "nivel=" + String(valorReal, 3);
      
      modemSerial.print(dataParaEnviar);
      modemSerial.flush(); 
      
      // Esperamos la respuesta del servidor (con un timeout)
      unsigned long tiempoInicioEspera = millis();
      String respuestaServidor = "";
      while (millis() - tiempoInicioEspera < 15000) {
        if (modemSerial.available()) {
          respuestaServidor = modemSerial.readString();
          break;
        }
      }
      
      if (respuestaServidor != "") {
        Serial.println("Respuesta del servidor recibida: " + respuestaServidor);
      } else {
        Serial.println("Timeout: No se recibió respuesta del servidor.");
      }

    } else {
      Serial.print("Fallo en la lectura del sensor. Código: ");
      Serial.println(result, HEX);
    }
  }

  // El loop() continúa ejecutándose miles de veces por segundo,
  // permitiendo añadir otras tareas aquí si fuera necesario (ej: leer un botón)
  // y lo más importante: ¡NO BLOQUEA EL PROCESADOR, evitando el reinicio del Watchdog!
}

// --- FUNCIONES DE CONTROL (preTransmission y postTransmission) ---
void preTransmission() {
  digitalWrite(SENSOR_DE_RE_PIN, HIGH);
  delay(10);
}

void postTransmission() {
  delay(10);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
}