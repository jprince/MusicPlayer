#include "NFCReader.h"
#include "SpotifyClient.h"
#include "settings.h"

#define BUZZER_PIN D2  // The Arduino Nano ESP32 pin connected to piezo buzzer
#define PIN_RED    D11 // The Arduino Nano ESP32 pin connected to R pin
#define PIN_GREEN  D10 // The Arduino Nano ESP32 pin connected to G pin
#define PIN_BLUE   D9  // The Arduino Nano ESP32 pin connected to B pin

NFCReader::NFCReader() : pn532i2c(Wire), pn532(pn532i2c), connected(false) {}

SpotifyClient spotify = SpotifyClient(clientId, clientSecret, deviceName, refreshToken);

void NFCReader::begin() {
  Serial.begin(115200);
  pn532.begin();
  while (!connected) {
    delay(1000);
    connectToReader();
  }

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  pn532.setPassiveActivationRetries(0xFF);
  connectWifi();
  spotify.FetchToken();
  spotify.GetDevices();

  pn532.SAMConfig();

  Serial.println("Ready to scan");
  setColor(0, 255, 0); // green
  delay(1000);
  clearColor();
}

void NFCReader::connectToReader() {
  uint32_t versiondata = pn532.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("PN532 card not found!");
    connected = false;
    return;
  }
  Serial.println("PN532 card found!");
  connected = true;
}

void NFCReader::loop() {
  if (nfc.tagPresent()) {
    NfcTag tag = nfc.read();
    if (tag.hasNdefMessage()) {
      NdefMessage message = tag.getNdefMessage();
      if (message.getRecordCount() > 0) {
        NdefRecord record = message.getRecord(0); // Get the first record

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);
        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) {
            payloadAsString += (char)payload[c];
        }
        String formattedPayload = payloadAsString.substring(1); // ignore one leading whitespace character
        playSpotifyUri(formattedPayload);
        delay(2000);
      } else {
        Serial.println("No records found on tag.");
      }
    }
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
      setColor(255, 102, 0); // orange
      spotify.GetDevices();
      clearColor();
      spotify.Play(context_uri);
      setColor(successRGB[0], successRGB[1], successRGB[2]);
      delay(5000);
      clearColor();
      spotify.Shuffle();
      break;
    }
    case 401:
    {
      // auth token expired, get new one
      setColor(255, 102, 0); // orange
      spotify.FetchToken();
      clearColor();
      spotify.Play(context_uri);
      setColor(successRGB[0], successRGB[1], successRGB[2]);
      delay(5000);
      clearColor();
      spotify.Shuffle();
      break;
    }
    default:
    {
      setColor(successRGB[0], successRGB[1], successRGB[2]);
      playTone();
      delay(3000);
      clearColor();
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

void NFCReader::playTone() {
  tone(BUZZER_PIN, 2794, 288);
  noTone(BUZZER_PIN);
}

void NFCReader::setColor(int R, int G, int B) {
  analogWrite(PIN_RED,   R);
  analogWrite(PIN_GREEN, G);
  analogWrite(PIN_BLUE,  B);
}

void NFCReader::clearColor() {
  analogWrite(PIN_RED,   0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE,  0);
}
