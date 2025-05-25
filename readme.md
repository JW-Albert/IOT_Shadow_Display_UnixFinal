# ESP32S Local Gateway for Device Control

This project implements a local gateway using ESP32S to control devices through a cloud-based Shadow API. The system supports both cloud and local control modes with priority handling and state synchronization.

## Features
- Dual-mode operation (Cloud/Local)
- Secure communication with cloud Shadow API
- Real-time device state synchronization
- Local permission control
- Cloud permission management
- Time-based device status monitoring
- Automatic mode switching
- Debounced input handling

## Hardware Requirements
- ESP32S development board
- Input pins:
  - GPIO 36: Cloud mode switch (ADC input)
  - GPIO 39: Local permission control (INPUT_PULLUP)
  - GPIO 34: Local device control (INPUT_PULLUP)
- Output pins:
  - GPIO 4: Relay control (OUTPUT)

## Software Requirements
- [PlatformIO](https://platformio.org/) or Arduino IDE
- Required libraries:
  - WiFi
  - WiFiClientSecure
  - HTTPClient
  - ArduinoJson
  - time.h

## Configuration
Edit `src/main.cpp` to set your network and authentication details:
```cpp
const char *SSID = "YourWiFiSSID";
const char *PASSWORD = "YourWiFiPassword";
const char *TOKEN = "your-device-token";
```

## API Endpoints
- Server Time: `https://unix.jw-albert.dev/authapi/servertime`
- Shadow Delta: `https://unix.jw-albert.dev/api/shadow/get?type=delta`
- Shadow Desired: `https://unix.jw-albert.dev/api/shadow/get?type=desired`
- Shadow Update: `https://unix.jw-albert.dev/api/shadow/update`

## System Operation

### 1. Initialization
- System boot and reset information logging
- GPIO pin configuration
- Cloud mode detection via ADC input
- WiFi connection (if in cloud mode)
- Initial state synchronization

### 2. Cloud Mode Operation
- Maintains WiFi connection
- Polls for delta updates every 1.5 seconds
- Reports device state to cloud
- Handles permission changes
- Synchronizes with server time
- Device considered offline if time difference > 10 seconds

### 3. Local Mode Operation
- Direct GPIO control
- Local permission management
- State reporting to cloud (if connected)
- Automatic mode switching

### 4. State Management
- Device Status (ON/OFF)
- Cloud Permission (0/1)
- Local Permission (0/1)
- Operation Mode (Cloud/Local)
- Time Synchronization

### 5. Error Handling
- WiFi reconnection
- HTTP error recovery
- Input debouncing (50ms)
- State verification
- Automatic fallback to local mode

## Control Flow

### Cloud Control
1. User updates desired state in cloud
2. ESP32S polls delta endpoint
3. System applies changes if permitted
4. State reported back to cloud

### Local Control
1. Local permission pin enables control
2. Control pin directly affects device
3. State reported to cloud if connected
4. Cloud control disabled when local permission active

## Debug Information
- Serial output at 115200 baud
- System reset tracking
- State change logging
- Error reporting
- Connection status

## Customization
- Adjust debounce delay (DEBOUNCE_DELAY)
- Modify polling interval (default 1.5s)
- Change GPIO pin assignments
- Update API endpoints
- Modify time sync threshold

## Security Features
- HTTPS communication
- Token-based authentication
- Local permission control
- Cloud permission management
- Secure WiFi connection

## License
This project is provided for educational purposes only. Free to modify and use in academic projects.

---

*For production use, ensure proper security measures and certificate validation are implemented.*
