#include "oled_menu.h"
#include "espnowcom.h"
#include "faces.h"
#include "scaling.h"
// Initialize the U8g2 display object (modify the constructor for your OLED module)
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);
const int BITMAP_WIDTH = 64;
const int BITMAP_HEIGHT = 32;
const int DISPLAY_WIDTH = 128;
const int DISPLAY_HEIGHT = 64;
void initOLED() {
    u8g2.begin();
    Serial.println("OLED initialized.");
}

void updateOLED(const uint16_t* currentFaceIndex, const char* faceName) {
    u8g2.clearBuffer();

    // Display face name or default message
    if (faceName != nullptr) {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(2, 10, "Face:");
        u8g2.drawStr(30, 10, faceName); // Display face name
    } else {
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(2, 10, "Face: Unknown");
    }

    int startX = (DISPLAY_WIDTH - BITMAP_WIDTH) / 2;
    int startY = DISPLAY_HEIGHT - BITMAP_HEIGHT;


    // Draw the scaled-down bitmap if available
    if (currentFaceIndex != nullptr) {
        for (int y = 0; y < BITMAP_HEIGHT; y += 1) {  //  Scale down by skipping rows
            for (int x = 0; x < BITMAP_WIDTH; x += 1) { // Scale down by skipping columns
                uint16_t color = currentFaceIndex[y * BITMAP_WIDTH + x];
                bool pixelOn = color != 0x0000; //  threshold
                if (pixelOn) {
                    u8g2.drawPixel(startX + x , startY + y );
                }
            }
        }
    }



    u8g2.sendBuffer();
}
