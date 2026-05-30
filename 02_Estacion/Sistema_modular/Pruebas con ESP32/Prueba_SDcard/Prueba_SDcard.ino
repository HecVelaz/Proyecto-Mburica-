#include <SPI.h>
#include <SD.h>

// --- Definimos solo el pin Chip Select ---
// La librería SPI ya sabe que SCK=4, MISO=5, MOSI=6 por el archivo pins_arduino.h
#define SDCARD_CS_PIN   10

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\n--- INICIANDO VERIFICADOR DE SD (VERSIÓN SIMPLE) ---");

  // --- ¡SIMPLIFICADO! Iniciamos la comunicación SPI en los pines por defecto ---
  // No necesitamos especificar los pines porque la librería ya los conoce.
  SPI.begin();
  
  // Intentar inicializar la tarjeta SD
  // Le pasamos solo el pin CS. La librería usará el bus SPI por defecto que acabamos de iniciar.
  if (!SD.begin(SDCARD_CS_PIN)) {
    Serial.println("FALLO DE INICIALIZACIÓN: Revisa el cableado de los pines 4, 5, 6, 7 y el formato de la tarjeta.");
    return; // Detiene la ejecución aquí.
  }
  
  Serial.println("Tarjeta SD inicializada con éxito.");

  // --- El resto de la lógica es idéntica ---

  // PASO 1: ESCRITURA
  File myFile = SD.open("/test_simple.txt", FILE_WRITE);

  if (myFile) {
    Serial.print("Escribiendo en archivo de prueba... ");
    myFile.print("SPI Simple OK");
    myFile.close();
    Serial.println("¡Hecho!");
  } else {
    Serial.println("Error: No se pudo abrir el archivo para escritura.");
    return;
  }

  // PASO 2: LECTURA Y VERIFICACIÓN
  myFile = SD.open("/test_simple.txt");
  String mensajeLeido = "";

  if (myFile) {
    Serial.print("Leyendo del archivo de prueba... ");
    while (myFile.available()) {
      mensajeLeido += (char)myFile.read();
    }
    myFile.close();
    Serial.println("¡Hecho!");
    
    Serial.println("Mensaje original: 'SPI Simple OK'");
    Serial.println("Mensaje leído:    '" + mensajeLeido + "'");

    // PASO 3: EL VEREDICTO FINAL
    Serial.println("\n-------------------------------------------------");
    if (mensajeLeido == "SPI Simple OK") {
      Serial.println("¡VERIFICACIÓN EXITOSA! Los datos coinciden.");
    } else {
      Serial.println("¡VERIFICACIÓN FALLIDA! Los datos leídos no coinciden.");
    }
    Serial.println("-------------------------------------------------");

  } else {
    Serial.println("Error: No se pudo abrir el archivo para lectura.");
  }
}

void loop() {
  // La prueba solo se ejecuta una vez en el setup.
}