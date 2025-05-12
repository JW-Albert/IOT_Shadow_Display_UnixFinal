# ESP32S Local Gateway for Device Control

This project uses an ESP32S as a local gateway to control on-premises (local) devices via a cloud-based Shadow API. The ESP32S connects to WiFi, polls the cloud for device state changes (delta), and updates the device status accordingly. It also reports the current device state back to the cloud.

## Features
- Acts as a local gateway for device control
- Communicates securely with a cloud Shadow API
- Polls for device state changes (delta)
- Controls a local device (e.g., onboard LED)
- Reports device status (reported) to the cloud
- Simple configuration for WiFi and authentication token

## Hardware Requirements
- ESP32S development board
- Onboard LED (or external device connected to GPIO2)

## Software Requirements
- [PlatformIO](https://platformio.org/) or Arduino IDE
- Arduino libraries:
  - WiFi
  - WiFiClientSecure
  - HTTPClient
  - ArduinoJson (see `platformio.ini` for version)

## Getting Started

### 1. Clone the Repository
```bash
git clone -b LocalGateway_ESP-32S https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal.git
cd IOT_Shadow_Display_UnixFinal
```

### 2. Configure WiFi and Token
Edit `src/main.cpp` and set your WiFi SSID, password, and device token:
```cpp
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* token = "your-device-token";
```

### 3. Build and Upload
Use PlatformIO or Arduino IDE to build and upload the firmware to your ESP32S board.

### 4. Serial Monitor
Open the serial monitor at 115200 baud to view logs and debug information.

## How It Works
1. **WiFi Connection:** The ESP32S connects to the specified WiFi network.
2. **Polling Cloud Shadow API:**
   - Periodically sends a GET request to the Shadow API to fetch the latest device state (delta).
   - If a new state is received, updates the local device (e.g., turns the LED on/off).
3. **Reporting State:**
   - After updating the device, sends a POST request to the Shadow API to report the current state (reported).
4. **Error Handling:**
   - Handles WiFi reconnection and HTTP errors gracefully.

## File Structure
- `src/main.cpp` — Main application logic
- `platformio.ini` — PlatformIO project configuration

## Dependencies
See `platformio.ini` for library dependencies. Main external library:
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

## Cloud API Endpoints
- **Get Delta:** `https://unix.jw-albert.dev/api/shadow/get?type=delta`
- **Update Reported:** `https://unix.jw-albert.dev/api/shadow/update`

## Customization
- Change the controlled device by modifying the `ledPin` or adding more GPIO logic in `main.cpp`.
- Adjust polling interval by changing the `delay(2000);` in the main loop.

## License
Specify your license here (MIT, Apache 2.0, etc.)

---

*This project is intended for educational and internal use. For production, ensure proper security and certificate validation.*
