#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include "fauxmoESP.h"

const uint16_t kIrLed = 4;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).
fauxmoESP fauxmo;
IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// Sequence for ON
uint16_t rawData_on[67] = {10782, 4506,  672, 598,  702, 540,  708, 562,  676, 590,  674, 564,  706, 564,  674, 590,  674, 562,  708, 1696,  680, 1696,  682, 1698,  714, 1696,  682, 1696,  682, 1696,  714, 1698,  680, 1698,  682, 590,  674, 564,  706, 564,  674, 1698,  712, 564,  674, 590,  674, 564,  704, 564,  674, 1696,  714, 1696,  682, 1696,  706, 566,  674, 1696,  682, 1696,  714, 1696,  680, 1696,  706};  // NEC FF10EF
uint32_t address = 0x0;
uint32_t command = 0x8;
uint64_t data_on = 0xFF10EF;

// Sequence for OFF
uint16_t rawData_off[67] = {10764, 4534,  696, 576,  682, 558,  736, 536,  704, 562,  678, 562,  738, 532,  680, 586,  678, 562,  710, 1694,  712, 1668,  688, 1694,  742, 1670,  712, 1668,  688, 1692,  718, 1694,  710, 1668,  686, 586,  680, 560,  712, 558,  678, 586,  678, 560,  708, 564,  678, 586,  676, 562,  706, 1698,  684, 1694,  686, 1694,  718, 1694,  712, 1668,  684, 1696,  718, 1694,  712, 1670,  648};  // NEC FF00FF
uint64_t data_off = 0xFF00FF;

// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE     115200
#define ID_CANDLE           "Kerzen"
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------
#define WIFI_SSID           "...."
#define WIFI_PASS           "...."

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void send(uint16_t data[]) {
  // 3 times with 1 sec delay. 
  for (int i=0; i<3; ++i)
    irsend.sendRaw(data, 67, 38);  // Send a raw data capture at 38kHz.
    delay(1000);
}

void setup() {

    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    // Wifi
    wifiSetup();

    // act as wireless switch
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices
    fauxmo.enable(true);

    // Add virtual devices
    fauxmo.addDevice("Kerzen"); // Say: Alexa, turn on Kerzen. (Alexa, schalte Kerzen ein)

    // Event handler
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, ID_CANDLE)==0) {
          if (state)
            send(rawData_on);
          else
            send(rawData_off);
        } 
    });

    irsend.begin();
}

void loop() {

    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();

    // for debugging only
    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }
}
