#include <Arduino.h>
#include <PxMatrix.h>
#include <U8g2lib.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <QuickEspNow.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include "faces.h"
#include "boop.h"
#include "speech.h"
#include "espnowcom.h"
#include "scaling.h"
#include "oled_menu.h"

//#define ENABLE_BOOP 
//#define ENABLE_MIC 

// Define display pins
#define P_LAT 22
#define P_A 5
#define P_B 18
#define P_C 19
#define P_D 21
#define P_E -1
#define P_OE 23


unsigned long currentMillis = millis();

// Initialize PxMatrix for LED panels
PxMATRIX display(64, 32, P_LAT, P_OE, P_A, P_B, P_C, P_D);

//const uint16_t* currentFaceBitmap = nullptr; // Pointer to the currently displayed face bitmap
//const char* currentFaceName = ]; // Name of the current face

QuickEspNow espnow;




void setup() {
    Serial.begin(115200);

    // Initialize custom modules
    initBoop();
    initSpeech();
    initOLED();
    initESPNow();
}
void boopInit() {
    
}

void speechInit() {
    
}



void loop() {
    unsigned long currentMillis = millis();

    #ifdef ENABLE_MIC
    handleSpeech(currentMillis);
    #endif
    #ifdef ENABLE_BOOP
    handleBoop(currentMillis);
    #endif
    // Update LED display
    display.showBuffer();
    

    // Update OLED info panel
    updateOLED(currentFaceBitmap, currentFaceName);

}


void displayBitmap(const uint8_t* currentFaceBitmap) {
    if (currentFaceBitmap == nullptr) {
        Serial.println("Bitmap is null, nothing to display.");
        return;
    }
    for (int y = 0; y < 32; y++) {
        for (int x = 0; x < 64; x++) {
            // Draw bitmap on the display (example logic)
            uint8_t pixel = currentFaceBitmap[y * 64 + x];
            display.drawPixel(x, y, pixel);
        }
    }
    display.showBuffer();
}

