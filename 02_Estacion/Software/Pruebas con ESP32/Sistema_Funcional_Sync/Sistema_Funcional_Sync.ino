

#include <HardwareSerial.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>


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

// Tarjeta SD micro sd adapter
#define SDCARD_CS_PIN   2  // Pin CS para la tarjeta SD

// --- CORRECCIÓN DRIFT ---
// Nuevas variables para el control de tiempo basado en el RTC
const int MINUTOS_INTERVALO = 5;  // El intervalo deseado en minutos
int ultimoMinutoRegistrado = -1;  // Memoria para saber en qué minuto se actuó por última vez.
                                  // Se inicializa a -1 para forzar la primera ejecución.

// --- FUNCIÓN DE CONFIGURACIÓN ---
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n--- INICIANDO SISTEMA SINCRONIZADO CON RTC ---");

  // Iniciar el RTC
  if (! rtc.begin()) {
    Serial.println("¡ERROR CRÍTICO! No se pudo encontrar el RTC.");
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
    Serial.println("ADVERTENCIA: Fallo al inicializar la tarjeta SD!");
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
  
  Serial.println("Configuración completa. El sistema está activo y esperando el próximo intervalo.");
}

// --- BUCLE PRINCIPAL (LÓGICA DE SINCRONIZACIÓN) ---
void loop() {
  // 1. En cada ciclo, obtenemos la hora actual del reloj del mundo real (RTC)
  DateTime ahora = rtc.now();
  int minutoActual = ahora.minute();

  // --- CORRECCIÓN DRIFT ---
  // 2. Comprobamos dos condiciones:
  //    a) ¿Es el minuto actual un múltiplo de nuestro intervalo? (ej: 0, 5, 10, 15...)
  //    b) ¿Y... no hemos actuado ya en este minuto? (para evitar envíos repetidos)
  if ((minutoActual % MINUTOS_INTERVALO == 0) && (minutoActual != ultimoMinutoRegistrado)) {

    // Si ambas condiciones son verdaderas, ¡es hora de ejecutar el ciclo completo!
    
    // Guardamos el minuto actual en nuestra "memoria" para no volver a actuar hasta que el minuto cambie
    ultimoMinutoRegistrado = minutoActual;
    
    Serial.println("\n==============================================");
    char buffer[20];
    sprintf(buffer, "Hora de activación: %02d:%02d:%02d", ahora.hour(), ahora.minute(), ahora.second());
    Serial.println(buffer);
    Serial.println("¡Intervalo de 5 minutos cumplido! Iniciando ciclo de trabajo...");

    // lógica de lectura y envío 
    uint8_t result = node.readHoldingRegisters(REGISTER_ADDRESS, 1);
    
    if (result == node.ku8MBSuccess) {
      uint16_t valorCrudo = node.getResponseBuffer(0);
      float valorReal = valorCrudo / 1000.0;
      uint32_t timestamp = ahora.unixtime() + 10800; // Se mantiene tu ajuste de +3 horas
      
      String dataParaEnviar = "d=" + String(timestamp) + "," + String(valorReal, 3);
      
      Serial.print("Sensor: ");
      Serial.print(valorReal, 3);
      Serial.print(" | Timestamp: ");
      Serial.println(timestamp);
      Serial.print("Cadena final a enviar: '");
      Serial.print(dataParaEnviar);
      Serial.println("'");
      
      guardarEnSD(timestamp, valorReal);
      
      modemSerial.print(dataParaEnviar);
      modemSerial.flush(); 
      
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

  // Pequeña pausa para que el loop no se ejecute a máxima velocidad,
  // siendo más eficiente y estable.
  delay(100); 
}

// --- FUNCIONES AUXILIARES (Sin cambios) ---

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

// Funciones de control del conversor RS485 del sensor
void preTransmission() {
  digitalWrite(SENSOR_DE_RE_PIN, HIGH);
  delay(10);
}

void postTransmission() {
  delay(10);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
}
