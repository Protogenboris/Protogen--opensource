#ifndef SPEECH_H
#define SPEECH_H

#include <stdint.h>

void initSpeech();

void handleSpeech();
void updateSpeechAnimation();
void setMicSettings(uint16_t sensitivity, uint16_t minVol, uint16_t maxVol);
void handleSpeech(unsigned long currentMillis);
#endif // SPEECH_H
