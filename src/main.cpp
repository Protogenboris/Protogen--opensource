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
#define R1_PIN 42
#define G1_PIN 41
#define B1_PIN 40
#define R2_PIN 14
#define G2_PIN 12
#define B2_PIN 13
#define A_PIN 19
#define B_PIN 16
#define C_PIN 5
#define D_PIN 17
#define E_PIN -1 // required for 1/32 scan panels, like 64x64. Any available pin would do, i.e. IO32
#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 18

// These are already defined in other files, so we use extern here
extern bool faceUpdateRequested;
extern const uint16_t* currentFaceBitmap;
extern const char* currentFaceName;

unsigned long currentMillis = millis();

// Initialize MatrixPanel_I2S_DMA for LED panels
MatrixPanel_I2S_DMA *display = nullptr;
HUB75_I2S_CFG mxconfig(
  64, // Module width
  32, // Module height
  2  // Chain length

);

QuickEspNow espnow;

SemaphoreHandle_t displayMutex = xSemaphoreCreateMutex();

void Task2code(void * pvParameters);
// This is already defined in espnowcom.cpp
extern void processFaceUpdate();

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

/// Predefining up task variables for dual core operation
TaskHandle_t Task1; 
TaskHandle_t Task2;

// Function to draw a bitmap to the display with mirroring on the second panel
void drawBitmap(const uint16_t* bitmap, int16_t x, int16_t y, int16_t width, int16_t height) {
    if (bitmap == nullptr) {
        ESP_LOGE("DRAW", "Bitmap is null");
        return;
    }
    // Draw on the first panel (left side, 0-63)
    for (int16_t j = 0; j < height; j++) {
        for (int16_t i = 0; i < width && i < 64; i++) {
            uint16_t color = pgm_read_word(&bitmap[j * width + i]);
            
            // Enhance brightness by boosting RGB components
            uint8_t r = ((color >> 11) & 0x1F) << 3;  // Extract and scale red (5 bits)
            uint8_t g = ((color >> 5) & 0x3F) << 2;   // Extract and scale green (6 bits)
            uint8_t b = (color & 0x1F) << 3;          // Extract and scale blue (5 bits)
            
            // Boost brightness by 30%
            r = min(255, (int)(r *  1));
            g = min(255, (int)(g *  1));
            b = min(255, (int)(b *  80));
            
            // Convert back to 565 format
            uint16_t enhancedColor = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
            
            display->drawPixel(x + i, y + j, enhancedColor);
        }
    }
    // Draw mirrored on the second panel (right side, 64-127)
    for (int16_t j = 0; j < height; j++) {
        for (int16_t i = 0; i < width && i < 64; i++) {
            uint16_t color = pgm_read_word(&bitmap[j * width + i]);
            
            // Enhance brightness by boosting RGB components
            uint8_t r = ((color >> 11) & 0x1F) << 3;  // Extract and scale red (5 bits)
            uint8_t g = ((color >> 5) & 0x3F) << 2;   // Extract and scale green (6 bits)
            uint8_t b = (color & 0x1F) << 3;          // Extract and scale blue (5 bits)
            
            // Boost brightness by 30%
            r = min(255, (int)(r *  1));
            g = min(255, (int)(g *  1));
            b = min(255, (int)(b *  80));
            
            // Convert back to 565 format
            uint16_t enhancedColor = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
            
            // Mirror horizontally by drawing at (127-i) instead of (64+i)
            display->drawPixel(127 - i, y + j, enhancedColor);
        }
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000); // Wait for serial to be ready
    
    ESP_LOGI("SETUP", "Starting setup with free heap: %d", ESP.getFreeHeap());
    ESP_LOGI("SETUP", "Initializing display...");

    // Configure pins explicitly
    mxconfig.gpio.r1 = R1_PIN;
    mxconfig.gpio.g1 = G1_PIN;
    mxconfig.gpio.b1 = B1_PIN;
    mxconfig.gpio.r2 = R2_PIN;
    mxconfig.gpio.g2 = G2_PIN;
    mxconfig.gpio.b2 = B2_PIN;
    mxconfig.gpio.a = A_PIN;
    mxconfig.gpio.b = B_PIN;
    mxconfig.gpio.c = C_PIN;
    mxconfig.gpio.d = D_PIN;
    mxconfig.gpio.e = E_PIN;
    mxconfig.gpio.lat = LAT_PIN;
    mxconfig.gpio.oe = OE_PIN;
    mxconfig.gpio.clk = CLK_PIN;
    
    // Set double buffering
    mxconfig.double_buff = double_buffer;

    mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_20M;  // Fastest clock
    mxconfig.min_refresh_rate = 200;              // Minimum refresh rate for maximum brightness
    //mxconfig.latch_blanking = 1;                // Minimal blanking
    mxconfig.clkphase = true;                  // Try false for ICN2037BP
    
    // Initialize the display
    display = new MatrixPanel_I2S_DMA(mxconfig);
    if (display == nullptr) {
        ESP_LOGE("SETUP", "Error: Failed to allocate display memory");
        while(1) { delay(100); } // Halt execution
    }
    
    ESP_LOGI("SETUP", "Display allocated, beginning initialization...");
    
    if (not display->begin()) {
        ESP_LOGE("SETUP", "Error: Panel failed to initialize");
        delete display;
        display = nullptr;
        while(1) { delay(100); } // Halt execution
    }
    
    ESP_LOGI("SETUP", "Display initialized successfully with free heap: %d", ESP.getFreeHeap());
    
    display->setBrightness8(255);
    display->fillScreen(display->color565(0, 0, 0));
    
    // Initialize the first face
    initFaces();
    setFace(0); // Start with the first face

    //This sets up dual core processing
    xTaskCreatePinnedToCore(
        Task2code,    // Task function.
        "Task2",      // name of task.
        10000,        // Stack size of task
        NULL,         // parameter of the task
        10,           // priority of the task
        &Task2,       // Task handle to keep track of created task
        0             // pin task to core 0
    );

    // Initialize custom modules
    initBoop();
    initSpeech();
    initOLED();
    initESPNow();
}

void loop() {
    unsigned long currentMillis = millis();

    #ifdef ENABLE_MIC
    handleSpeech(currentMillis);
    #endif
    #ifdef ENABLE_BOOP
    handleBoop(currentMillis);
    #endif

    // Update OLED info panel
    updateOLED(currentFaceBitmap, currentFaceName);

    // Check and process face update
    if (faceUpdateRequested) {
        faceUpdateRequested = false;  // Reset the flag
        processFaceUpdate();
    }
    
    // Cycle through faces every 5 seconds for testing
    static unsigned long lastFaceChange = 0;
    static int currentFaceIndex = 0;
    if (millis() - lastFaceChange > 5000) {
        lastFaceChange = millis();
        currentFaceIndex = (currentFaceIndex + 1) % 7; // There are 7 faces in the array
        setFace(currentFaceIndex);
        ESP_LOGI("FACE", "Changed to face %d: %s", currentFaceIndex, currentFaceName);
    }
    
    delay(10); // Small delay to prevent watchdog issues
}

void Task2code(void * pvParameters) {
    ESP_LOGI("TASK2", "Task started");
    
    // Wait a bit for the display to be fully initialized
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    
    for (;;) {
        vTaskDelay(25 / portTICK_PERIOD_MS); // Update at 20fps

        // Check if display is valid
        if (display == nullptr) {
            ESP_LOGE("TASK2", "Error: display is null");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            continue; // Skip this iteration instead of deleting the task
        }

        // Synchronize access to display
        if (xSemaphoreTake(displayMutex, portMAX_DELAY)) {
            // Clear the display
            display->fillScreen(display->color565(0, 0, 0));
            
            // Draw the current face bitmap if available
            if (currentFaceBitmap != nullptr) {
                // Draw the bitmap on both panels (original and mirrored)
                drawBitmap(currentFaceBitmap, 0, 0, 64, 32);
                
                // Log that we're drawing the face
                ESP_LOGI("TASK2", "Drawing face: %s", currentFaceName);
            }
            
            // Only flip if double buffering is enabled
            #if defined(double_buffer) && double_buffer == true
            display->flipDMABuffer();
            #endif
            
            xSemaphoreGive(displayMutex);
        }
    }
}
