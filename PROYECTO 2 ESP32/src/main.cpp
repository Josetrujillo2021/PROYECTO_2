
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#define cs 33
File archivo;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  pinMode(cs, OUTPUT);
  //Se esta inicializando la tarjeta SD
  if (!SD.begin(cs)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  //Parametros: 1:archivo a utilizar 2: es para ver si es para escritura o lectura
  //Si es lectura  se puede omitir
  archivo = SD.open("datos.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (archivo) {
    Serial.print("Writing to test.txt...");
    archivo.println("testing 1, 2, 3.");
    // Siempre cerrar el archivo cuando se modifique
    archivo.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  archivo = SD.open("test.txt");
  if (archivo) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (archivo.available()) {
      Serial.write(archivo.read());
    }
    // close the file:
    archivo.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void loop() {
  // nothing happens after setup
}