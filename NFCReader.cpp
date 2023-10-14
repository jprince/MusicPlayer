#include "NFCReader.h"
#include "settings.h"

NFCReader::NFCReader() : pn532i2c(Wire), nfc(pn532i2c), connected(false) {}

void NFCReader::begin() {
  Serial.begin(115200);
  Serial.println("*** Testing Module PN532 NFC RFID ***");
  connectWifi();
}

bool NFCReader::readCard(uint8_t uid[], uint8_t &uidLength) {
  boolean success;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  return success;
}

void NFCReader::connect() {
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 card not found!");
    connected = false;
    return;
  }
  connected = true;
}

void NFCReader::loop() {
  uint8_t uid[7];
  uint8_t uidLength = 0;

  while (!connected) {
    delay(5000);
    connect();
  }

  if (readCard(uid, uidLength)) {
    Serial.println("Card Detected");
    Serial.print("Size of UID: "); Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
    }
    Serial.println("");
    Serial.println("");

    connect();
  } else {
    // PN532 probably timed out waiting for a card
    // Serial.println("Timed out waiting for a card");
  }
}

void NFCReader::connectWifi()
{
  WiFi.begin(ssid, pass);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
