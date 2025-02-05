#ifndef OLED_MENU_H
#define OLED_MENU_H
#include "espnowcom.h"
#include <U8g2lib.h>
void initOLED();
// Function to update the OLED display with the current face bitmap and name
void updateOLED(const uint16_t* faceBitmap, const char* faceName);

#endif // OLED_MENU_H
