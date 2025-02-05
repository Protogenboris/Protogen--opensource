#include <Arduino.h>
#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <U8g2lib.h>
#include <Adafruit_GFX.h>
#include <WiFi.h>
#include <QuickEspNow.h>
#include <esp_now.h>
#include <esp_wifi.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include "faces.h"
#include "boop.h"
#include "speech.h"
#include "espnowcom.h"
#include "scaling.h"
#include "oled_menu.h"

//#define ENABLE_BOOP 
//#define ENABLE_MIC 
#define double_buffer true

// Define display pins
#define P_LAT 22
#define P_A 5
#define P_B 18
#define P_C 19
#define P_D 21
#define P_E -1
#define P_OE 23

unsigned long currentMillis = millis();
extern bool faceUpdateRequested;
extern void processFaceUpdate();

// Initialize MatrixPanel_I2S_DMA for LED panels
MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(
  64, // Module width
  32, // Module height
  2   // Chain length
);

QuickEspNow espnow;

void Task2code(void * pvParameters);

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

/// Predefining up task variables for dual core operation
TaskHandle_t Task1; 
TaskHandle_t Task2;

void setup() {
    //This sets up dual core processing
    xTaskCreatePinnedToCore(
        Task2code,    // Task function.
        "Task2",      // name of task.
        11000,        //Stack size of task
        NULL,         // parameter of the task
        24,           // priority of the task
        &Task2,       // Task handle to keep track of created task
        1             // pin task to core 1
    );

    Serial.begin(115200);

    // Initialize custom modules
    initBoop();
    initSpeech();
    initOLED();
    initESPNow();

    // Initialize the display
    display = new MatrixPanel_I2S_DMA(mxconfig);
    display->begin();
    display->setBrightness8(255);
    display->fillScreen(display->color565(0, 0, 0));
    display->setTextColor(display->color565(0, 0, 255));
    display->setCursor(16, 0);
    display->print("BOOTING UP....");
    display->setTextColor(display->color565(255, 255, 255));
    display->setCursor(16, 8);
    display->print("PROTOGEN OS");
    delay(3000);
    display->fillScreen(display->color565(0, 0, 0));
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
    display->flipDMABuffer();

    // Update OLED info panel
    updateOLED(currentFaceBitmap, currentFaceName);

    // Check and process face update
    if (faceUpdateRequested) {
        faceUpdateRequested = false;  // Reset the flag
        processFaceUpdate();
    }
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
            display->drawPixel(x, y, pixel);
        }
    }
    display->flipDMABuffer();
}

void Task2code(void * pvParameters) {  
  for (;;) {
    delay(1);
    display->flipDMABuffer();
  }
}