#include "scaling.h"
#include "faces.h"
void scaleBitmap(const uint16_t* source, uint8_t* destination, int srcWidth, int srcHeight, int destWidth, int destHeight) {
    for (int y = 0; y < destHeight; ++y) {
        for (int x = 0; x < destWidth; ++x) {
            int srcX = x * srcWidth / destWidth;
            int srcY = y * srcHeight / destHeight;
            int bitIndex = srcY * (srcWidth / 8) + srcX / 8;
            int bit = (source[bitIndex] >> (7 - (srcX % 8))) & 0x01;
            if (bit) {
                destination[y * (destWidth / 8) + x / 8] |= (1 << (7 - (x % 8)));
            } else {
                destination[y * (destWidth / 8) + x / 8] &= ~(1 << (7 - (x % 8)));
            }
        }
    }
}
