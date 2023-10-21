#include <Wire.h>
#include "NFCReader.h"

NFCReader nfcReader;

void setup() {
  nfcReader.begin();
}

void loop() {
  nfcReader.loop();
}
