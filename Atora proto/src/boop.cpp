#include "boop.h"
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;
static uint16_t boopThreshold = 50;
static bool boopActive = false;

unsigned long boopInterval;
unsigned long lastBoopCheck;

void initBoop() {
    Wire.begin();
    sensor.init();
    sensor.setTimeout(500);
    sensor.startContinuous();
}

void updateBoop() {
    uint16_t distance = sensor.readRangeContinuousMillimeters();
    if (distance < boopThreshold && !boopActive) {
        boopActive = true;
        // Trigger animation
    } else if (distance >= boopThreshold) {
        boopActive = false;
    }
}

void setBoopSettings(uint16_t threshold) {
    boopThreshold = threshold;
}

void handleBoop(unsigned long currentMillis) {
    if (currentMillis - lastBoopCheck >= boopInterval) {
        lastBoopCheck = currentMillis;
        uint16_t distance = sensor.readRangeContinuousMillimeters();

        if (distance < 100) { // Example distance threshold
            Serial.println("Boop detected!");
            // Trigger boop animation here
        }
    }
}