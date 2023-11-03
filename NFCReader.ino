#include <Wire.h>
#include "NFCReader.h"

NFCReader nfcReader;

// Compile project
// Upload to arduino
// Unplug usb
// Plug in usb
// Open serial monitor

void setup() {
  nfcReader.begin();
}

void loop() {
  nfcReader.loop();
}
