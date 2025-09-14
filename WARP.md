# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Project Overview

This is the **Protogen** project - an Arduino/ESP32-based system for controlling 64x32 LED matrices with animated faces. The project includes ESP-NOW wireless communication, OLED display, microphone input, and distance sensing capabilities.

## Hardware Architecture

- **Main Unit**: ESP32 controlling dual 64x32 HUB75 LED panels (128x32 total display)
- **Secondary Unit**: ESP32-C3 "ProtoPaw" wireless controller with buttons
- **Sensors**: VL53L0X distance sensor (boop detection), MAX4466 microphone, 128x64 OLED display
- **Communication**: ESP-NOW protocol for wireless control between main unit and ProtoPaw

## Development Commands

### Building and Uploading
```bash
# Build the main project for ESP32
pio run -e esp32dev

# Build for ESP32-S3 (alternative target)
pio run -e esp32-s3-devkitm-1

# Upload to ESP32
pio run -e esp32dev -t upload

# Monitor serial output
pio device monitor
```

### ProtoPaw Controller
```bash
# The ProtoPaw controller uses Arduino IDE or can be imported to PlatformIO
# Located in: esp32c3-protopaw/esp32c3-protopaw.ino
```

### Image Conversion
```bash
# Convert PNG images to face bitmap arrays
cd lib
python convimg.py

# This generates images.h with face data arrays
# Place PNG files in lib/ directory before running
```

## Code Architecture

### Core Components

**Main Processing (main.cpp)**
- Dual-core operation: Core 0 handles display rendering, Core 1 handles sensors/communication
- Matrix panel initialization with custom HUB75 configuration
- Face bitmap rendering with mirroring across dual panels
- Hardware-specific pin configuration for ESP32

**Face Management System (faces.h/faces.cpp)**
- Pre-compiled face bitmap arrays (64x32 pixels each, RGB565 format)  
- Face switching logic with named face references
- Current faces: Basic, Calm, Energetic, Huh, Love, Really, Shy
- Bitmap data stored in PROGMEM for memory efficiency

**Modular Feature System**
- **Boop Detection** (`boop.h/cpp`): VL53L0X distance sensor integration
- **Speech Animation** (`speech.h/cpp`): Microphone-driven mouth movement
- **ESP-NOW Communication** (`espnowcom.h/cpp`): Wireless face control from ProtoPaw
- **OLED Menu** (`oled_menu.h/cpp`): Status display on 128x64 OLED
- **Scaling Utilities** (`scaling.h/cpp`): Image processing helpers

**ProtoPaw Controller**
- 7-button wireless controller using ESP32-C3
- Button-to-command mapping (A-G buttons)
- ESP-NOW transmission to main unit
- MAC address pairing required between devices

### Key Technical Details

**Display Configuration**
- Dual 64x32 HUB75 panels chained together (128x32 total)
- Right panel shows mirrored version of left panel
- Custom GPIO pin assignments for HUB75 interface
- Double buffering enabled for smooth animations
- RGB565 color format with brightness enhancement

**Wireless Protocol**
- ESP-NOW used for low-latency button commands
- ProtoPaw sends single-character commands (A-G)
- Main unit MAC address must be configured in ProtoPaw code
- Commands trigger face changes and other actions

**Memory Management**
- Face bitmaps stored in flash memory (PROGMEM)
- Dual-core task separation to prevent memory conflicts
- Semaphores used for display buffer protection

## Development Workflow

### Adding New Faces
1. Create 64x32 PNG images and place in `lib/` directory
2. Run `python convimg.py` to generate bitmap arrays
3. Copy generated arrays from `images.h` to `faces.h`
4. Update face array and names in `faces.cpp`
5. Rebuild and upload

### Modifying Hardware Pins
- Pin definitions are in `main.cpp` (lines 23-36)
- HUB75 pins, sensor pins, and I2C pins are configurable
- Update `mxconfig` structure for display pins

### Debugging Tips
- Use serial monitor for logging (115200 baud)
- ESP_LOGI/ESP_LOGE macros provide structured logging
- Check MAC addresses for ESP-NOW communication issues
- Verify power supply capacity for LED panels

## Feature Flags

Enable/disable features by uncommenting in `main.cpp`:
- `#define ENABLE_BOOP` - Distance sensor "boop" detection  
- `#define ENABLE_MIC` - Microphone speech animation

## Dependencies

Core libraries managed by PlatformIO:
- `2dom/PxMatrix LED MATRIX library` - HUB75 panel control
- `adafruit/Adafruit GFX Library` - Graphics primitives
- `olikraus/U8g2` - OLED display driver
- `gmag11/QuickEspNow` - ESP-NOW communication
- `pololu/VL53L0X` - Distance sensor driver

## Important Notes

- This is the developer's first major project - code structure reflects learning progression
- ProtoPaw controller needs hardware revision due to battery consumption issues
- ESP-NOW requires MAC address configuration between devices
- HUB75 panels require adequate power supply (5V, high current)
- Project uses Arduino framework on ESP32 platform