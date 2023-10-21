#ifndef NFCReader_h
#define NFCReader_h

#include <WiFi.h>
#include <SoftwareSerial.h>
#include <PN532_SWHSU.h>
#include <PN532.h>
#include <NfcAdapter.h>

class NFCReader {
public:
  NFCReader();
  void begin();
  void connectToReader();
  void loop();
  void playSpotifyUri(String context_uri);
  void connectWifi();

private:
  SoftwareSerial SWSerial; // RX, TX
  PN532_SWHSU pn532swhsu;
  PN532 nfc;
  bool connected;
};

#endif
