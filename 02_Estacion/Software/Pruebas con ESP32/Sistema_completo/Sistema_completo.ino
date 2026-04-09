/********************************************************************************
 * SCRIPT FINAL V5: LECTOR + RTC + GPRS + REGISTRO EN SD
 * Mantiene la configuración de pines original y añade la SD en pines libres.
 ********************************************************************************/

#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <Wire.h>           // Para la comunicación I2C con el RTC
#include <RTClib.h>         // Para controlar el DS3231
#include <SPI.h>
#include <SD.h>

// --- OBJETOS Y CONFIGURACIONES ---

// RTC
RTC_DS3231 rtc;

// Módem GPRS
#define MODEM_RX_PIN  19
#define MODEM_TX_PIN  18
HardwareSerial modemSerial(1);

// Sensor Modbus
#define SENSOR_RX_PIN      5
#define SENSOR_TX_PIN      6
#define SENSOR_DE_RE_PIN   7
SoftwareSerial sensorSerial;
#define SENSOR_MODBUS_ADDRESS 1
#define SENSOR_BAUD_RATE      9600
#define REGISTER_ADDRESS      0 
ModbusMaster node;

// Tarjeta SD
#define SDCARD_CS_PIN   10  // Pin CS para la tarjeta SD

// Intervalo
const long INTERVALO_MS = 60000; // 1 minutos
unsigned long ultimoEnvio = 0;

// --- FUNCIÓN DE CONFIGURACIÓN ---
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n--- INICIANDO SISTEMA CON TIMESTAMP UNIX Y SD CARD ---");

  // Iniciar el RTC
  if (! rtc.begin()) {
    Serial.println("No se pudo encontrar el RTC.");
    while (1) delay(1000); 
  }
  Serial.println("RTC inicializado correctamente.");

  // Configurar Módem
  modemSerial.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  
  // Configurar Sensor
  sensorSerial.begin(SENSOR_BAUD_RATE, SWSERIAL_8N1, SENSOR_RX_PIN, SENSOR_TX_PIN);
  node.begin(SENSOR_MODBUS_ADDRESS, sensorSerial);
  pinMode(SENSOR_DE_RE_PIN, OUTPUT);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
  // Inicializar tarjeta SD
  SPI.begin();
  if (!SD.begin(SDCARD_CS_PIN)) {
    Serial.println("Fallo al inicializar la tarjeta SD!");
  } else {
    Serial.println("Tarjeta SD inicializada correctamente.");
    // Crear archivo de cabecera si no existe
    if (!SD.exists("/datalog.csv")) {
      File dataFile = SD.open("/datalog.csv", FILE_WRITE);
      if (dataFile) {
        dataFile.println("Timestamp,Valor(m)");
        dataFile.close();
      }
    }
  }
  
  Serial.println("Configuración completa. El sistema está activo.");
  
  // Forzamos el primer envío al arrancar.
  ultimoEnvio = millis() - INTERVALO_MS;
}

// --- BUCLE PRINCIPAL ---
void loop() {
  // ¿Ha pasado suficiente tiempo desde el último envío?
  if (millis() - ultimoEnvio >= INTERVALO_MS) {
    ultimoEnvio = millis();
    
    Serial.println("\n==============================================");
    Serial.println("¡Intervalo cumplido! Iniciando ciclo...");

    // 1. Leemos el sensor
    uint8_t result = node.readHoldingRegisters(REGISTER_ADDRESS, 1);
    
    if (result == node.ku8MBSuccess) {
      // 2. Si la lectura es exitosa, procesamos el dato del sensor
      uint16_t valorCrudo = node.getResponseBuffer(0); //valor del sensor leido en mm
      float valorReal = valorCrudo / 1000.0; //pasamos a metros
      
      // 3. Obtenemos la hora del RTC y la convertimos a Timestamp Unix+3 horas(Py)
      DateTime ahora = rtc.now();         // Leemos la hora completa del RTC
      uint32_t timestamp = ahora.unixtime()+10800; // Convertimos la hora a segundos desde 1970 timestamp
      
      // 4. Construimos la cadena de datos en el formato exacto para el servidor
      String dataParaEnviar = "d=" + String(timestamp) + "," + String(valorReal, 3); // Usamos 3 decimales 
      
      Serial.print("Sensor: ");
      Serial.print(valorReal, 3);
      Serial.print(" | Timestamp: ");
      Serial.println(timestamp);
      Serial.print("Cadena final a enviar: '");
      Serial.print(dataParaEnviar);
      Serial.println("'");
      
      // 5. Guardamos en la tarjeta SD
      guardarEnSD(timestamp, valorReal);
      
      // 6. Enviamos al módem
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
}

// Función para guardar datos en la tarjeta SD
void guardarEnSD(uint32_t timestamp, float valor) {
  if (SD.cardType() == CARD_NONE) {
    Serial.println("No hay tarjeta SD presente, no se guardarán datos.");
    return;
  }
  
  File dataFile = SD.open("/datalog.csv", FILE_APPEND);
  
  if (dataFile) {
    dataFile.print(timestamp);
    dataFile.print(",");
    dataFile.println(valor, 3);
    dataFile.close();
    Serial.println("Datos guardados en SD correctamente.");
  } else {
    Serial.println("Error al abrir el archivo en la SD!");
  }
}

// --- FUNCIONES DE CONTROL (sin cambios) ---
void preTransmission() {
  digitalWrite(SENSOR_DE_RE_PIN, HIGH);
  delay(10);
}

void postTransmission() {
  delay(10);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
}