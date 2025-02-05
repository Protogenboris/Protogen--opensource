#include "espnowcom.h"
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include "faces.h"
#include "oled_menu.h"
#include <U8g2lib.h>

// Callback for receiving ESP-NOW data
volatile bool faceUpdateRequested = false;
volatile char requestedCommand = 0;

void onDataReceived(const uint8_t* mac, const uint8_t* data, int len) {
    if (len > 0) {
        requestedCommand = static_cast<char>(data[0]);
        faceUpdateRequested = true;
    } else {
        Serial.println("Received empty data packet.");
    }
}

void processFaceUpdate() {
    switch (requestedCommand) {
        case 'A': currentFaceBitmap = Basic_face; currentFaceName = "Basic Face"; break;
        case 'B': currentFaceBitmap = Calm_face; currentFaceName = "Calm"; break;
        case 'C': currentFaceBitmap = engy_face; currentFaceName = "Engy"; break;
        case 'D': currentFaceBitmap = Huh_face; currentFaceName = "Huh"; break;
        case 'E': currentFaceBitmap = Love_face; currentFaceName = "Love"; break;
        case 'F': currentFaceBitmap = Rly_face; currentFaceName = "Rly"; break;
        case 'G': currentFaceBitmap = shy_face; currentFaceName = "Shy"; break;
        default: Serial.println("Unknown command."); return;
    }

    updateOLED(currentFaceBitmap, currentFaceName);
}

// Initialize ESP-NOW
void initESPNow() {
    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        updateOLED(nullptr, "Unable to connect with ProtoPaw");
        return;
    }
    
    Serial.println("ESP-NOW initialized");
    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    Serial.printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    updateOLED(nullptr, "ESP-NOW Ready");

    // Register the callback function for data reception
    esp_now_register_recv_cb(onDataReceived);
}
