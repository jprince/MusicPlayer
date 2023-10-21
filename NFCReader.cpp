#include "NFCReader.h"
#include "SpotifyClient.h"
#include "settings.h"

NFCReader::NFCReader() : SWSerial( 3, 2 ), pn532swhsu(SWSerial), nfc(pn532swhsu), connected(false) {}

SpotifyClient spotify = SpotifyClient(clientId, clientSecret, deviceName, refreshToken);

void NFCReader::begin() {
  Serial.begin(115200);
  nfc.begin();
  while (!connected) {
    delay(1000);
    connectToReader();
  }
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0x01); // 1. 0xff instead is 255
  connectWifi();
  spotify.FetchToken();
  spotify.GetDevices();
}

void NFCReader::connectToReader() {
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 card not found!");
    connected = false;
    return;
  }
  Serial.println("PN532 card found!");
  connected = true;
}

void NFCReader::loop() {
  Serial.println("Looping");
  delay(1000);
  uint8_t uid[7]; // Buffer to store the returned UID
  uint8_t uidLength = 0; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  boolean success;
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength, 50);
  if (success) {
    Serial.println("Card Detected");
    Serial.print("Size of UID: "); Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID: ");
    for (uint8_t i = 0; i < uidLength; i++) {
      Serial.print(" 0x"); Serial.print(uid[i], HEX);
    }
    Serial.println("");
    Serial.println("");

    // TODO: make this URI dynamic (i.e. read from NFC tag)
    playSpotifyUri("spotify:playlist:15mly4Os7BdTWCnBEGylJW");
    delay(2000);
    // verify that connection is still live
    // connectToReader();
  }
}

void NFCReader::playSpotifyUri(String context_uri)
{
  int code = spotify.Play(context_uri);
  switch (code)
  {
    case 404:
    {
      // device id changed, get new one
      spotify.GetDevices();
      spotify.Play(context_uri);
      spotify.Shuffle();
      break;
    }
    case 401:
    {
      // auth token expired, get new one
      spotify.FetchToken();
      spotify.Play(context_uri);
      spotify.Shuffle();
      break;
    }
    default:
    {
      spotify.Shuffle();
      break;
    }
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
