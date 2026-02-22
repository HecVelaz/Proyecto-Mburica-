#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
#include <vector>

// --- OBJETOS Y CONFIGURACIONES ---
RTC_DS3231 rtc;

#define MODEM_RX_PIN  19
#define MODEM_TX_PIN  18
HardwareSerial modemSerial(1);

#define SENSOR_RX_PIN      5
#define SENSOR_TX_PIN      6
#define SENSOR_DE_RE_PIN   7
SoftwareSerial sensorSerial(SENSOR_RX_PIN, SENSOR_TX_PIN);

#define SENSOR_MODBUS_ADDRESS 1
#define SENSOR_BAUD_RATE      9600
#define REGISTER_ADDRESS      0 
ModbusMaster node;

#define SDCARD_CS_PIN   2

// --- Variables de control ---
const int MINUTOS_INTERVALO = 5;      // cada 1 minuto
int ultimoMinutoRegistrado = -1;
String archivoLogMaestro = "/datalog.csv";
String archivoBuffer = "/buffer.csv";

// --- Reinicio programado ---
bool reinicioProgramadoEjecutado = false;

// --- Control de frecuencia de envío del buffer ---
unsigned long ultimoEnvioBuffer = 0;
const unsigned long intervaloEnvioBuffer = 10000; // 10 segundos

// --- Buffer en RAM si no hay SD ---
std::vector<String> bufferRAM;
bool sdDisponible = false;

// --- FUNCIÓN DE CONFIGURACIÓN ---
void setup() {
  Serial.begin(115200);
  Serial.println("\n--- DATALOGGER IoT CON STORE & FORWARD ---");

  // Inicializar RTC
  if (!rtc.begin()) {
    Serial.println("¡ERROR CRÍTICO! RTC no encontrado.");
    while (1);
  }
  Serial.println("RTC inicializado.");

  // Inicializar módem y sensor
  modemSerial.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  sensorSerial.begin(SENSOR_BAUD_RATE, SWSERIAL_8N1);
  node.begin(SENSOR_MODBUS_ADDRESS, sensorSerial);
  pinMode(SENSOR_DE_RE_PIN, OUTPUT);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // Inicializar SD
  SPI.begin();
  if (SD.begin(SDCARD_CS_PIN)) {
    Serial.println("Tarjeta SD inicializada.");
    sdDisponible = true;
    // eliminar buffer antiguo
    if (SD.exists(archivoBuffer)) {
      SD.remove(archivoBuffer);
      Serial.println("Buffer anterior eliminado.");
    }
    // crear log maestro si no existe
    if (!SD.exists(archivoLogMaestro)) {
      File dataFile = SD.open(archivoLogMaestro, FILE_WRITE);
      if (dataFile) {
        dataFile.println("Timestamp,Valor(m)");
        dataFile.close();
      }
    }
  } else {
    Serial.println("⚠️ SD no detectada, se usará buffer en RAM");
    sdDisponible = false;
  }

  reinicioProgramadoEjecutado = false;
  Serial.println("Reinicio programado a las 2:02 AM activado.");
  Serial.println("Configuración completa. Sistema activo.");
}

// --- BUCLE PRINCIPAL ---
void loop() {
  DateTime ahora = rtc.now();
  int minutoActual = ahora.minute();

  // TAREA 1: REGISTRAR DATO (cada 1 minuto)
  if ((minutoActual % MINUTOS_INTERVALO == 0) && (minutoActual != ultimoMinutoRegistrado)) {
    ultimoMinutoRegistrado = minutoActual;
    Serial.println("\n==============================================");
    Serial.println("¡Intervalo cumplido! Registrando medición...");
    leerYGuardarEnLogs(ahora);
  }

  // TAREA 2: PROCESAR BUFFER DE ENVÍO (cada 10 segundos)
  if (millis() - ultimoEnvioBuffer >= intervaloEnvioBuffer) {
    procesarBufferDeEnvio();
    ultimoEnvioBuffer = millis();
  }

  // TAREA 3: VERIFICAR REINICIO PROGRAMADO 2:02 AM
  if (ahora.hour() == 2 && ahora.minute() == 2 && ahora.second() == 0 && !reinicioProgramadoEjecutado) {
    Serial.println("\nREINICIO PROGRAMADO 2:02 AM - Iniciando...");
    reinicioProgramadoEjecutado = true;
    realizarReinicioSeguro();
  }

  // Resetear bandera de reinicio (para el próximo día)
  if (ahora.hour() == 2 && ahora.minute() == 3 && ahora.second() == 0) {
    reinicioProgramadoEjecutado = false;
  }

  delay(1000);
}

// --- FUNCIÓN DE REINICIO SEGURO 2:02 AM ---
void realizarReinicioSeguro() {
  guardarLogReinicio();
  Serial.println("Cerrando recursos para reinicio seguro...");
  delay(2000);
  Serial.println("Reiniciando ESP32...");
  delay(3000);
  ESP.restart();
}

void guardarLogReinicio() {
  if (!sdDisponible) return;
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
  // 1. Validación de fecha
  if (ahora.year() < 2024 || ahora.year() > 2030) {
    Serial.println("⚠️ Fecha inválida detectada, descartando dato.");
    return;
  }

  // 2. Validación de que el minuto sea divisible entre 5 y segundo sea 0
  if ((ahora.minute() % 5 != 0) || (ahora.second() != 0)) {
    Serial.println("⏱ Minuto no divisible entre 5 o segundo no es 0, dato descartado.");
    return;
  }

  // 3. Leer el sensor vía Modbus
  uint8_t result = node.readHoldingRegisters(REGISTER_ADDRESS, 1);

  if (result == node.ku8MBSuccess) {
    uint16_t valorCrudo = node.getResponseBuffer(0);
    float valorReal = valorCrudo / 1000.0;
    uint32_t timestamp = ahora.unixtime() + 10800; // ajuste horario (3h)

    // 4. Validación de rango (ejemplo: nivel entre 0 y 10 m)
    if (valorReal < 0.0 || valorReal > 10.0) {
      Serial.print("⚠️ Valor fuera de rango detectado (");
      Serial.print(valorReal);
      Serial.println(" m). Dato descartado.");
      return;
    }

    // 5. Preparar y guardar dato válido
    String dataParaEnviar = "d=" + String(timestamp) + "," + String(valorReal, 3);
    guardarDato(dataParaEnviar);
    guardarLogMaestro(timestamp, valorReal);

  } else {
    Serial.print("Fallo lectura sensor. Código: ");
    Serial.println(result, HEX);
  }
}

void guardarDato(String data) {
  if (sdDisponible) {
    File dataFile = SD.open(archivoBuffer, FILE_APPEND);
    if (dataFile) {
      dataFile.println(data);
      dataFile.close();
      Serial.println("Dato añadido al buffer SD.");
    } else {
      Serial.println("¡Error al abrir buffer en SD!");
    }
  } else {
    bufferRAM.push_back(data);
    Serial.println("Dato añadido al buffer en RAM.");
  }
}

void guardarLogMaestro(uint32_t timestamp, float valor) {
  if (!sdDisponible) return;
  File dataFile = SD.open(archivoLogMaestro, FILE_APPEND);
  if (dataFile) {
    dataFile.print(timestamp);
    dataFile.print(",");
    dataFile.println(valor, 3);
    dataFile.close();
    Serial.println("Dato guardado en log maestro.");
  }
}

void procesarBufferDeEnvio() {
  if (sdDisponible) {
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
      Serial.println("Procesando buffer SD: " + primeraLinea);
      if (enviarDatoAlServidor(primeraLinea)) {
        Serial.println("  -> Envío exitoso. Eliminando línea del buffer SD.");
        eliminarPrimeraLineaDelBuffer();
      } else {
        Serial.println("  -> Envío fallido. El dato permanecerá en el buffer SD.");
      }
    } else {
      eliminarPrimeraLineaDelBuffer();
    }
  } else {
    // Buffer en RAM
    if (bufferRAM.empty()) return;
    String dato = bufferRAM[0];
    if (enviarDatoAlServidor(dato)) {
      bufferRAM.erase(bufferRAM.begin());
      Serial.println("Dato enviado y eliminado del buffer RAM.");
    } else {
      Serial.println("Envío fallido, dato permanecerá en buffer RAM.");
    }
  }
}

bool enviarDatoAlServidor(String data) {
  Serial.print("Transmitiendo: '" + data + "'...");
  modemSerial.print(data);
  modemSerial.flush();

  unsigned long tiempoInicioEspera = millis();
  while (millis() - tiempoInicioEspera < 5000) {  // espera 5s la respuesta
    if (modemSerial.available()) {
      String respuesta = modemSerial.readString();
      respuesta.trim();
      Serial.println(" Respuesta del servidor: " + respuesta);

      if (respuesta.indexOf("NIVEL_REGISTRADO") != -1) {
        Serial.println(" Confirmación exacta recibida.");
        return true;  
      } else if (respuesta.length() > 0) {
        Serial.println(" Respuesta diferente recibida, se aceptará igual.");
        return true;
      }
    }
  }
  Serial.println(" ¡Timeout sin respuesta!");
  return false;
}

void eliminarPrimeraLineaDelBuffer() {
  File bufferFile = SD.open(archivoBuffer, FILE_READ);
  File tempFile = SD.open("/temp.csv", FILE_WRITE);

  if (!bufferFile || !tempFile) {
    Serial.println("Error al abrir archivos para limpiar buffer SD.");
    return;
  }

  if(bufferFile.available()){ bufferFile.readStringUntil('\n'); }

  while(bufferFile.available()){ tempFile.write(bufferFile.read()); }

  bufferFile.close();
  tempFile.close();

  SD.remove(archivoBuffer);
  SD.rename("/temp.csv", archivoBuffer);
}

// --- FUNCIONES CONTROL SENSOR ---
void preTransmission() {
  digitalWrite(SENSOR_DE_RE_PIN, HIGH);
  delay(10);
}

void postTransmission() {
  delay(10);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
}

