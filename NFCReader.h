#ifndef NFCReader_h
#define NFCReader_h

#include <Wire.h>
#include <WiFi.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

class NFCReader {
public:
  NFCReader();
  void begin();
  bool readCard(uint8_t uid[], uint8_t &uidLength);
  void connect();
  void loop();
  void playSpotifyUri(String context_uri);
  void connectWifi();

private:
  PN532_I2C pn532i2c;
  PN532 nfc;
  bool connected;
};

#endif
