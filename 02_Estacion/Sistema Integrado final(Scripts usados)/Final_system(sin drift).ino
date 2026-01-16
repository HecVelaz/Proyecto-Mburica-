#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>

// --- OBJETOS Y CONFIGURACIONES  ---
RTC_DS3231 rtc;
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
#define SDCARD_CS_PIN   2

// --- ¡S&F! Variables de control ---
const int MINUTOS_INTERVALO = 5;
int ultimoMinutoRegistrado = -1;
String archivoLogMaestro = "/datalog.csv";
String archivoBuffer = "/buffer.csv";

// --- Reinicio programado 2:02 AM ---
bool reinicioProgramadoEjecutado = false;

// --- FUNCIÓN DE CONFIGURACIÓN  ---
void setup() {
  Serial.begin(115200);
  // while (!Serial); // Comentar en versión final
  Serial.println("\n--- DATALOGGER IoT CON STORE & FORWARD ---");

  // Inicializar RTC
  if (!rtc.begin()) {
    Serial.println("¡ERROR CRÍTICO! RTC no encontrado.");
    while (1);
  }
  Serial.println("RTC inicializado.");

  // Inicializar módem y sensor
  modemSerial.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  sensorSerial.begin(SENSOR_BAUD_RATE, SWSERIAL_8N1, SENSOR_RX_PIN, SENSOR_TX_PIN);
  node.begin(SENSOR_MODBUS_ADDRESS, sensorSerial);
  pinMode(SENSOR_DE_RE_PIN, OUTPUT);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  
  // Inicializar SD
  SPI.begin();
  if (!SD.begin(SDCARD_CS_PIN)) {
    Serial.println("ADVERTENCIA: Fallo al inicializar la tarjeta SD!");
  } else {
    Serial.println("Tarjeta SD inicializada.");
    // --- ELIMINAR BUFFER ANTERIOR ---
    if (SD.exists(archivoBuffer)) {
      SD.remove(archivoBuffer);
      Serial.println("Buffer anterior eliminado.");
    }
    // --- FIN DE LIMPIEZA ---
    if (!SD.exists(archivoLogMaestro)) {
      File dataFile = SD.open(archivoLogMaestro, FILE_WRITE);
      if (dataFile) {
        dataFile.println("Timestamp,Valor(m)");
        dataFile.close();
      }
    }
  }
  
  reinicioProgramadoEjecutado = false;
  Serial.println("Reinicio programado a las 2:02 AM activado.");
  Serial.println("Configuración completa. Sistema activo.");
}

// --- BUCLE PRINCIPAL  ---
void loop() {
  // TAREA 1: REGISTRAR DATO (sincronizado con RTC)
  DateTime ahora = rtc.now();
  int minutoActual = ahora.minute();

  if ((minutoActual % MINUTOS_INTERVALO == 0) && (minutoActual != ultimoMinutoRegistrado)) {
    ultimoMinutoRegistrado = minutoActual;
    Serial.println("\n==============================================");
    Serial.println("¡Intervalo cumplido! Registrando medición...");
    leerYGuardarEnLogs(ahora);
  }

  // TAREA 2: PROCESAR BUFFER DE ENVÍO
  procesarBufferDeEnvio();
  
  // TAREA 3: VERIFICAR REINICIO PROGRAMADO 2:02 AM
  if (ahora.hour() == 2 && ahora.minute() == 2 && !reinicioProgramadoEjecutado) {
    Serial.println("\nREINICIO PROGRAMADO 2:02 AM - Iniciando...");
    reinicioProgramadoEjecutado = true;
    realizarReinicioSeguro();
  }
  
  // Resetear bandera de reinicio (para el próximo día)
  if (ahora.hour() == 2 && ahora.minute() == 3) {
    reinicioProgramadoEjecutado = false;
  }
  
  delay(1000);
}

// --- FUNCIÓN DE REINICIO SEGURO 2:02 AM ---
void realizarReinicioSeguro() {
  // Guardar log del reinicio
  guardarLogReinicio();
  
  Serial.println("Cerrando recursos para reinicio seguro...");
  delay(2000);
  
  Serial.println("Reiniciando ESP32...");
  delay(3000);
  ESP.restart();
}

void guardarLogReinicio() {
  if (SD.cardType() == CARD_NONE) return;
  
  File logFile = SD.open("/reinicios.log", FILE_APPEND);
  if (logFile) {
    DateTime ahora = rtc.now();
    logFile.print("Reinicio_2:02_AM - ");
    logFile.print(ahora.day()); logFile.print("/");
    logFile.print(ahora.month()); logFile.print("/");
    logFile.print(ahora.year()); logFile.print(" ");
    logFile.print(ahora.hour()); logFile.print(":");
    logFile.println(ahora.minute());
    logFile.close();
    Serial.println("Log de reinicio guardado.");
  }
}

// --- FUNCIONES STORE & FORWARD ---
void leerYGuardarEnLogs(DateTime ahora) {
  uint8_t result = node.readHoldingRegisters(REGISTER_ADDRESS, 1);
  
  if (result == node.ku8MBSuccess) {
    uint16_t valorCrudo = node.getResponseBuffer(0);
    float valorReal = valorCrudo / 1000.0;
    uint32_t timestamp = ahora.unixtime() + 10800;
    
    String dataParaEnviar = "d=" + String(timestamp) + "," + String(valorReal, 3);
    
    guardarEnLogMaestro(timestamp, valorReal);
    guardarEnBuffer(dataParaEnviar);

  } else {
    Serial.print("Fallo lectura sensor. Código: ");
    Serial.println(result, HEX);
  }
}

void procesarBufferDeEnvio() {
  if (!SD.exists(archivoBuffer)) return;

  File bufferFile = SD.open(archivoBuffer, FILE_READ);
  if (!bufferFile || !bufferFile.available()) {
    if(bufferFile) bufferFile.close();
    SD.remove(archivoBuffer);
    return;
  }
  
  String primeraLinea = bufferFile.readStringUntil('\n');
  primeraLinea.trim();
  bufferFile.close();

  if (primeraLinea.length() > 0) {
    if (enviarDatoAlServidor(primeraLinea)) {
      Serial.println("  -> Envío exitoso. Eliminando línea del buffer.");
      eliminarPrimeraLineaDelBuffer();
    } else {
      Serial.println("  -> Envío fallido. El dato permanecerá en el buffer.");
    }
  } else {
    eliminarPrimeraLineaDelBuffer();
  }
}

bool enviarDatoAlServidor(String data) {
  Serial.print("Transmitiendo: '" + data + "'...");
  modemSerial.print(data);
  modemSerial.flush();
  
  unsigned long tiempoInicioEspera = millis();
  while (millis() - tiempoInicioEspera < 5000) {  // ⏰ SOLUCIÓN TEMPORAL: 5 segundos (antes 15)
    if (modemSerial.available()) {
      String respuesta = modemSerial.readString();
      if (respuesta.indexOf("NIVEL_REGISTRADO") != -1) {
        Serial.println(" ¡Confirmado!");
        return true;
      }
    }
  }
  Serial.println(" ¡Timeout!");
  return false;
}

void eliminarPrimeraLineaDelBuffer() {
    File bufferFile = SD.open(archivoBuffer, FILE_READ);
    File tempFile = SD.open("/temp.csv", FILE_WRITE);

    if (!bufferFile || !tempFile) {
      Serial.println("Error al abrir archivos para limpiar buffer.");
      return;
    }

    if(bufferFile.available()){ bufferFile.readStringUntil('\n'); }

    while(bufferFile.available()){ tempFile.write(bufferFile.read()); }
    
    bufferFile.close();
    tempFile.close();
    
    SD.remove(archivoBuffer);
    SD.rename("/temp.csv", archivoBuffer);
}

void guardarEnLogMaestro(uint32_t timestamp, float valor) {
  File dataFile = SD.open(archivoLogMaestro, FILE_APPEND);
  if (dataFile) {
    dataFile.print(timestamp);
    dataFile.print(",");
    dataFile.println(valor, 3);
    dataFile.close();
    Serial.println("Dato guardado en log maestro.");
  } else {
    Serial.println("¡Error al abrir log maestro!");
  }
}

void guardarEnBuffer(String data) {
  File dataFile = SD.open(archivoBuffer, FILE_APPEND);
  if (dataFile) {
    dataFile.println(data);
    dataFile.close();
    Serial.println("Dato añadido al buffer de envío.");
  } else {
    Serial.println("¡Error al abrir archivo buffer!");
  }
}

// --- FUNCIONES CONTROL SENSOR  ---
void preTransmission() {
  digitalWrite(SENSOR_DE_RE_PIN, HIGH);
  delay(10);
}

void postTransmission() {
  delay(10);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
}