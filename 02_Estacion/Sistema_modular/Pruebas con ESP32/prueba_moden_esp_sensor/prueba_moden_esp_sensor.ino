/********************************************************************************
 * SCRIPT FINAL UNIFICADO: LECTOR DE SENSOR Y TRANSMISOR GPRS
 * 
 * Lógica de funcionamiento:
 * 1. Lee el valor de un sensor Modbus RS485 usando SoftwareSerial.
 * 2. Si la lectura es exitosa, convierte el valor a metros.
 * 3. Formatea el dato y lo envía a través de un módem GPRS usando HardwareSerial.
 * 4. Espera una confirmación del servidor.
 * 5. Repite el proceso cada 30 segundos.
 ********************************************************************************/

// --- LIBRERÍAS NECESARIAS ---
#include <HardwareSerial.h> // Para el módem GPRS (rápido y robusto)
#include <SoftwareSerial.h> // Para el sensor Modbus (baja velocidad)
#include <ModbusMaster.h>   // Para el protocolo del sensor

// --- CONFIGURACIÓN DEL MÓDEM GPRS ---
#define MODEM_RX_PIN  19  // Oído del ESP para el Módem
#define MODEM_TX_PIN  18  // Boca del ESP para el Módem
HardwareSerial modemSerial(1); // Usamos el robusto UART de Hardware 1

// --- CONFIGURACIÓN DEL SENSOR MODBUS ---
#define SENSOR_RX_PIN      5  // Oído del ESP para el Sensor
#define SENSOR_TX_PIN      6  // Boca del ESP para el Sensor
#define SENSOR_DE_RE_PIN   7  // Pin de control para el conversor del sensor (si es manual)
SoftwareSerial sensorSerial;    // Creamos un objeto SoftwareSerial

// --- PARÁMETROS DEL SENSOR (DE TU HOJA DE DATOS) ---
#define SENSOR_MODBUS_ADDRESS 1
#define SENSOR_BAUD_RATE      9600
#define REGISTER_ADDRESS      0 

ModbusMaster node; // Objeto para manejar la comunicación Modbus

// --- INTERVALO GENERAL ---
const int INTERVALO_SEGUNDOS = 35;

// --- FUNCIÓN DE CONFIGURACIÓN ---
void setup() {
  // 1. Iniciar comunicación con el PC para depuración
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n--- INICIANDO SISTEMA INTEGRADO DE TELEMETRÍA ---");

  // 2. Configurar el Módem GPRS
  modemSerial.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  Serial.println("Puerto del Módem (HardwareSerial) inicializado.");

  // 3. Configurar el Sensor Modbus
  sensorSerial.begin(SENSOR_BAUD_RATE, SWSERIAL_8N1, SENSOR_RX_PIN, SENSOR_TX_PIN);
  node.begin(SENSOR_MODBUS_ADDRESS, sensorSerial); // Le decimos a ModbusMaster que use el puerto de software

  // Configurar el control DE/RE para el conversor del sensor
  pinMode(SENSOR_DE_RE_PIN, OUTPUT);
  digitalWrite(SENSOR_DE_RE_PIN, LOW); // Modo recepción por defecto
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);
  Serial.println("Puerto del Sensor (SoftwareSerial) y ModbusMaster inicializados.");

  Serial.println("Configuración completa. Iniciando ciclo de lectura y envío...");
}

// --- BUCLE PRINCIPAL ---
void loop() {
  Serial.println("\n==============================================");
  Serial.println("PASO 1: LEYENDO DATOS DEL SENSOR...");

  uint8_t result = node.readHoldingRegisters(REGISTER_ADDRESS, 1);
  
  // Si la lectura del sensor fue exitosa...
  if (result == node.ku8MBSuccess) {
    Serial.println("¡Éxito! Respuesta recibida del sensor.");
    
    // 1. Obtenemos el valor crudo como un entero de 16 bits
    uint16_t valorCrudoEnsamblado = node.getResponseBuffer(0);

    // 2. Extraemos los bytes individuales para verlos en HEX
    uint8_t highByte = highByte(valorCrudoEnsamblado);
    uint8_t lowByte = lowByte(valorCrudoEnsamblado);

    Serial.print("  -> Trama de datos crudos (payload en HEX): ");
    if (highByte < 16) Serial.print("0");
    Serial.print(highByte, HEX);
    Serial.print(" ");
    if (lowByte < 16) Serial.print("0");
    Serial.print(lowByte, HEX);
    Serial.println();
    
    // 3. ¡NUEVO! Mostramos el valor completo en formato binario de 16 bits
    Serial.print("  -> Valor en 16 bits (binario):         ");
    // Imprimimos el número en base 2 (BIN)
    Serial.println(valorCrudoEnsamblado, BIN);

    // 4. Mostramos el valor ensamblado en formato decimal
    Serial.print("  -> Valor ensamblado (entero decimal):   ");
    Serial.println(valorCrudoEnsamblado);

    // 5. Convertimos a metros
    float valorReal = valorCrudoEnsamblado / 1000.0;
    
    Serial.print("  -> Valor Real (metros):                ");
    Serial.println(valorReal, 3);

    // --- AHORA, PROCEDEMOS A ENVIAR ESTE DATO ---
    Serial.println("\nPASO 2: ENVIANDO DATO AL SERVIDOR...");
    
    String dataParaEnviar = "nivel=" + String(valorReal, 3);
    Serial.print("Cadena a enviar: '");
    Serial.print(dataParaEnviar);
    Serial.println("'");

    // Enviar por el puerto del módem
    modemSerial.print(dataParaEnviar);
    modemSerial.flush(); 
    Serial.println("Dato enviado al módem. Esperando respuesta del servidor...");

    // Escuchar la respuesta del servidor
    unsigned long tiempoInicio = millis();
    String respuestaServidor = "";
    while (millis() - tiempoInicio < 30000) { // Esperamos hasta 30 segundos
      if (modemSerial.available()) {
        respuestaServidor = modemSerial.readString();
        break; // Salimos del bucle en cuanto recibimos algo
      }
    }

    if (respuestaServidor != "") {
      Serial.print("Respuesta del servidor: ");
      Serial.println(respuestaServidor);
      if (respuestaServidor.indexOf("NIVEL_REGISTRADO") != -1) {
        Serial.println("ESTADO: ¡CICLO COMPLETADO CON ÉXITO!");
      } else {
        Serial.println("ESTADO: ADVERTENCIA, respuesta del servidor inesperada.");
      }
    } else {
      Serial.println("ESTADO: ¡ERROR! No se recibió respuesta del servidor.");
    }

  } else {
    // Si la lectura del sensor falló...
    Serial.print("¡FALLO en la lectura del sensor! Código de error Modbus: ");
    Serial.print(result, HEX);
    Serial.println("\nNo se enviará nada esta vez.");
  }
  
  // Esperar para el siguiente ciclo completo
  Serial.print("\nEsperando ");
  Serial.print(INTERVALO_SEGUNDOS);
  Serial.println(" segundos para el próximo ciclo...");
  delay(INTERVALO_SEGUNDOS * 1000);
}


// --- FUNCIONES DE CONTROL PARA EL CONVERSOR RS485 DEL SENSOR ---
void preTransmission() {
  digitalWrite(SENSOR_DE_RE_PIN, HIGH);
  delay(10);
}

void postTransmission() {
  delay(10);
  digitalWrite(SENSOR_DE_RE_PIN, LOW);
}
