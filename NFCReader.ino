#include <Wire.h>
#include "NFCReader.h"

#define PIN_RED    D11 // The Arduino Nano ESP32 pin connected to R pin
#define PIN_GREEN  D10 // The Arduino Nano ESP32 pin connected to G pin
#define PIN_BLUE   D9  // The Arduino Nano ESP32 pin connected to B pin

NFCReader nfcReader;

void setup() {
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  nfcReader.begin();
}

void loop() {
  nfcReader.loop();
}
