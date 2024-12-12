#include "speech.h"
#include <Arduino.h>

static const uint8_t micPin = A0;  // Adjust pin if necessary
static uint16_t micSensitivity = 512;
static uint16_t minVolume = 50;
static uint16_t maxVolume = 1023;

// Predefined mouth frames
static const uint8_t mouthFrame1[] = { /* Bitmap data */ };
static const uint8_t mouthFrame2[] = { /* Bitmap data */ };
static const uint8_t mouthFrame3[] = { /* Bitmap data */ };
static const uint8_t* mouthFrames[] = {mouthFrame1, mouthFrame2, mouthFrame3};

static uint8_t currentFrame = 0;

unsigned long lastMicCheck;
unsigned long micInterval;  // Adjust interval as needed


void handleSpeech(unsigned long currentMillis) {
    if (currentMillis - lastMicCheck >= micInterval) {
        lastMicCheck = currentMillis;
        int micValue = analogRead(micPin);

        if (micValue > 500) { // Example threshold for loudness
            Serial.println("Speech detected!");
            // Trigger mouth animation here
        }
    }
}
void initSpeech() {
    pinMode(micPin, INPUT);
}

void updateSpeechAnimation() {
    uint16_t volume = analogRead(micPin);
    if (volume > minVolume) {
        currentFrame = map(volume, minVolume, maxVolume, 0, 2);
    } else {
        currentFrame = 0; // Default closed-mouth frame
    }

    // Display frame on LED matrix or OLED
}

void setMicSettings(uint16_t sensitivity, uint16_t minVol, uint16_t maxVol) {
    micSensitivity = sensitivity;
    minVolume = minVol;
    maxVolume = maxVol;
}
