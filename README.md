# 🤖 Protogen - Open Source LED Matrix Fursuit Head

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![Platform](https://img.shields.io/badge/platform-ESP32-blue.svg)]()
[![License](https://img.shields.io/badge/license-Open%20Source-green.svg)]()
[![Version](https://img.shields.io/badge/version-1.0.0--stable-orange.svg)]()

A comprehensive ESP32-based system for controlling LED matrix displays in fursuit heads, featuring dual 64x32 HUB75 LED panels, OLED status display, wireless control, and interactive sensors.
## ✨ Features
### 🎯 **Current Stable Build (v1.0.0)**
- ✅ **Dual LED Matrix Control** - Two 64x32 HUB75 panels with mirrored display
- ✅ **Button Face Switching** - Manual control via GPIO21 button
- ✅ **OLED Status Display** - Real-time face and system information
- ✅ **7 Face Expressions** - Pre-loaded animated face bitmap collection
- ✅ **Optimized Performance** - Efficient rendering with 50ms refresh cycles
- ✅ **Dual-Core Processing** - Dedicated display task on Core 0
- ✅ **Memory Efficient** - Only 7.2% RAM usage on ESP32
### 🚧 **Advanced Features (In Development)**
- 🔄 **ESP-NOW Wireless Control** - Remote ProtoPaw controller (temporarily disabled)
- 📱 **Interactive Sensors** - Boop detection and speech animation
- 🎵 **Audio Reactive** - Microphone-based face animations
- 🌐 **Web Interface** - Browser-based configuration (planned)
- 📡 **NRF Integration** - Low-power wireless communication (planned)

## 🛠️ Hardware Requirements
### **Core Components**
- **ESP32** or **ESP32-S3** development board
- **2x 64x32 HUB75 LED Matrices** (P3 or P4 pitch recommended)
- **128x64 OLED Display** (I2C interface)
- **5V Power Supply** (minimum 4A for full brightness)
- **Push Button** (for face switching)
  ### **Optional Sensors**
- **MAX4466 Microphone** (for speech animation)
- **VL53L0X ToF Sensor** (for boop detection)
- **ProtoPaw Controller** (wireless remote)
### **Pin Configuration**
#### HUB75 LED Matrix Pins:
```
R1: GPIO42  | G1: GPIO41  | B1: GPIO40
R2: GPIO14  | G2: GPIO12  | B2: GPIO13
A:  GPIO19  | B:  GPIO16  | C:  GPIO5
D:  GPIO17  | CLK: GPIO18 | LAT: GPIO4
OE: GPIO15  | E:  Not used (32px height)
```

#### Control & Sensors:
```
Button:     GPIO21 (with internal pullup)
I2C SDA:    GPIO21 (OLED)
I2C SCL:    GPIO22 (OLED)
Microphone: GPIO34 (when enabled)
ToF Sensor: GPIO25 (when enabled)
```

## 🚀 Quick Start

### **Prerequisites**
- [Visual Studio Code](https://code.visualstudio.com/)
- [PlatformIO Extension](https://platformio.org/install/ide?install=vscode)
- USB cable for ESP32 programming

### **Installation**

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Protogenboris/Protogen--opensource.git
   cd Protogen--opensource
   ```
2. **Open in VS Code:**
   ```bash
   code .
   ```

3. **Install dependencies:**
   PlatformIO will automatically install required libraries on first build.

4. **Build and upload:**
   ```bash
   pio run --target upload
   ```

### **First Boot**
1. Connect power supply (5V, 4A+)
2. Watch serial monitor for boot messages
3. Press button on GPIO21 to cycle through faces
4. Check OLED display for current face status

## 📖 Documentation

- 📋 **[WARP.md](WARP.md)** - Complete development guide and project overview
- 📝 **[IMPROVEMENTS.md](IMPROVEMENTS.md)** - Detailed changelog and technical improvements
- 🔧 **[Hardware Wiring Guide](#hardware-requirements)** - Pin configurations and connections

## 🏗️ Project Structure

```
Protogen--opensource/
├── src/
│   ├── main.cpp              # Main application logic
│   ├── faces.cpp/.h          # Face bitmap management
│   ├── oled_menu.cpp/.h      # OLED display control
│   ├── scaling.cpp/.h        # Image scaling utilities
│   ├── boop.cpp/.h           # Boop sensor (disabled)
│   ├── speech.cpp/.h         # Speech animation (disabled)
│   └── espnowcom.disabled    # ESP-NOW communication (disabled)
├── lib/
│   └── ESP32-HUB75-MatrixPanel-I2S-DMA/  # LED matrix library
├── WARP.md                   # Development guide
├── IMPROVEMENTS.md           # Changelog
└── platformio.ini           # PlatformIO configuration
```

## 🎮 Usage

### **Basic Operation**
- **Power On:** Connect 5V power supply
- **Face Switching:** Press button on GPIO21
- **Status Check:** View OLED display for current face info
- **Serial Monitor:** Connect at 115200 baud for debugging

### **Available Faces**
1. Default Face (index 0)
2. Calm Face (index 1) 
3. Happy Face (index 2)
4. Sad Face (index 3)
5. Angry Face (index 4)
6. Surprised Face (index 5)
7. Love Face (index 6)

## 🛠️ Development

### **Building from Source**
```bash
# Clean build
pio run -t clean

# Build only
pio run

# Build and upload
pio run -t upload

# Monitor serial output
pio device monitor
```

### **Memory Usage**
- **ESP32:** 7.2% RAM (23,664 bytes), 26.2% Flash (342,861 bytes)
- **ESP32-S3:** 6.4% RAM (21,096 bytes), 10.5% Flash (350,437 bytes)

### **Re-enabling Advanced Features**
To re-enable ESP-NOW or sensors, rename the disabled files and uncomment the relevant includes in `main.cpp`.

## 🤝 Contributing

We welcome contributions! Please read our contributing guidelines before submitting PRs:

1. Fork the repository
2. Create a feature branch
3. Test your changes thoroughly
4. Submit a pull request with clear description

## 🐛 Known Issues

- ESP-NOW communication temporarily disabled for stability
- Sensor modules require initialization fixes before re-enabling
- Web interface not yet implemented

## 📅 Roadmap

- [ ] **v1.1.0** - Re-enable ESP-NOW with improved stability
- [ ] **v1.2.0** - Sensor integration and interactive features
- [ ] **v2.0.0** - Web configuration interface
- [ ] **v2.1.0** - NRF-based low-power ProtoPaw controller
- [ ] **v3.0.0** - WS2812B and MAX7219 panel support

## 📞 Support

**Need help?** We're here for you!

- 📧 **Email:** tankst445@gmail.com
- 💬 **Telegram:** @Fulldayfullnight
- 🐙 **GitHub Issues:** [Create an Issue](https://github.com/Protogenboris/Protogen--opensource/issues)

## 📄 License

This project is open source and available under the [MIT License](LICENSE).

## 🙏 Acknowledgments

- **ESP32-HUB75-MatrixPanel-I2S-DMA** library for LED matrix control
- **U8g2** library for OLED display support  
- **PlatformIO** for excellent development experience
- **Furry community** for inspiration and support

---

⭐ **Star this repo** if you found it helpful!
