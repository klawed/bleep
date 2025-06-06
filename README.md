# bleep

An ESP32 IoT project built from a standardized template with GitHub Pages integration for web-based firmware installation.

## Features

- 🚀 **Web-based firmware installation** using ESP Web Tools
- 🔧 **Multi-board support** (ESP32, ESP32-S3)
- 🎯 **GitHub Actions CI/CD** with automatic building and deployment
- 📱 **Interactive demo firmware** with multiple blink modes
- 🌐 **WiFi scanning and connectivity**
- 📊 **GitHub Pages integration** for easy distribution

## Quick Start

### 1. Hardware Requirements
- ESP32 or ESP32-S3 development board
- USB cable for programming
- Optional: OLED display (SSD1306, I2C)

### 2. Web Installation (Recommended)
1. Visit the [project's GitHub Pages]() <!-- Add your GitHub Pages URL here -->
2. Connect your ESP32 via USB
3. Click "Install Firmware"
4. Select your device and follow the prompts

### 3. Local Development
```bash
# Clone the repository
git clone <your-repo-url>
cd bleep

# Install PlatformIO
pip install platformio

# Build and upload
pio run -e esp32dev -t upload
```

## Usage

The demo firmware includes several interactive features:

- **Button Control**: Press the BOOT button to cycle through blink modes
- **Serial Commands**: 
  - Send 's' to scan WiFi networks
  - Send 'r' to restart the device
- **Blink Modes**:
  - Mode 0: Slow blink (1 second)
  - Mode 1: Fast blink (200ms)
  - Mode 2: Heartbeat pattern
  - Mode 3: SOS pattern

## Customization

### Adding WiFi Credentials
Edit `src/main.cpp` and update:
```cpp
const char* ssid = "YourWiFiName";
const char* password = "YourWiFiPassword";
```

### Adding Libraries
Update `platformio.ini` in the `lib_deps` section:
```ini
lib_deps = 
    your/library@version
```

### Customizing the Web Interface
- Edit `web/index.html` to change the installation page
- Replace `web/img/espPinout.png` with your project's pinout diagram
- Update the manifest in `generate_esp_web_tools.py`

## GitHub Pages Setup

1. Enable GitHub Pages in your repository settings
2. Set source to "GitHub Actions"
3. Push changes to trigger the build workflow
4. Your firmware will be available at: `https://username.github.io/repository-name/`

## Project Structure

```
bleep/
├── .github/workflows/
│   └── build-sketch.yml      # CI/CD workflow
├── src/
│   └── main.cpp             # Main firmware code
├── web/
│   ├── index.html           # Installation web page
│   ├── img/
│   │   └── espPinout.png    # Pinout diagram
│   └── firmware/            # Generated firmware files
├── generate_esp_web_tools.py # ESP Web Tools manifest generator
├── generate_merge_firmware_command.py # Firmware merging script
├── platformio.ini           # PlatformIO configuration
└── README.md               # This file
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Built with [PlatformIO](https://platformio.org/)
- Web installation powered by [ESP Web Tools](https://esphome.github.io/esp-web-tools/)
- Template based on [ESP32 IoT Template](https://github.com/your-username/esp32-iot-template)
