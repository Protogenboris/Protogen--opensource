#ifndef BOOP_H
#define BOOP_H

#include <stdint.h>

void initBoop();

void handleBoop();
void updateBoop();
void setBoopSettings(uint16_t threshold);
void handleBoop(unsigned long currentMillis);
#endif // BOOP_H
