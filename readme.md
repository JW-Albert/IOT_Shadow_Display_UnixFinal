# IOT_Shadow_Display_UnixFinal – Project Overview

## 中文版說明
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/blob/main/readme_zh-tw.md

## 🔍 Project Summary

IOT_Shadow_Display_UnixFinal is a Unix course final project that simulates the operation of AWS IoT Shadow. It implements a complete IoT control architecture, including:

- Central control via web interface
- Local gateway operation and override logic
- Device status synchronization through a custom Shadow system

The system uses Flask and JSON files to manage device state, eliminating the need for cloud services or databases, making it lightweight and suitable for offline or embedded environments.

---

## 🧠 Key Features

- Device Shadow mechanism (desired / reported / delta)
- RESTful API with API key authentication
- Frontend web interface for control and monitoring
- Local gateway emergency override support
- Support for multiple devices via per-device JSON files

---

## 🖥️ Technology Stack

- **Backend:** 
  - Python (Flask)
  - ESP32 (PlatformIO)
- **Frontend:** 
  - HTML / JavaScript (AJAX)
  - Apache2 web server
- **Storage:** 
  - JSON files (one per device)
  - Local file system
- **System:** 
  - Debian 12 (Linux)
  - ESP32 microcontroller
- **Security:**
  - API key authentication
  - Web-based login system

---

## 📁 Directory Structure

```
IOT_Shadow_Display_UnixFinal/
├── LocalGateway_ESP-32S/  # ESP32 Gateway implementation
│   ├── src/              # ESP32 source code
│   └── platformio.ini    # PlatformIO configuration
├── frontend/             # Web interface
│   ├── index.html       # Main control interface
│   ├── login.html       # Authentication page
│   └── apache2/         # Web server configuration
├── auth/                 # Authentication system
├── shadow/              # Shadow JSON files
└── main/                # Core system components
```

---

## 🔐 Use Cases

- IoT device control and monitoring
- Local edge computing scenarios
- Systems requiring emergency fallback control
- Educational use or embedded system prototyping
- ESP32-based gateway implementations
- Secure web-based device management

---

## 🙌 Credits

- Albert Wang（王建葦） – System Architecture, Backend, Shadow Logic
- Andy Chen（陳稚翔） – Frontend Interface Design

---

## 📜 License

This project is provided for educational use only. Free to modify and extend.

## 🚀 Setup and Installation Guide

### System Requirements
- Debian 12 (Linux) or compatible system
- Python 3.x
- Apache2 web server
- ESP32 development board (for gateway)
- PlatformIO or Arduino IDE (for ESP32 development)

### 1. Backend Setup

#### Python Environment
```bash
# Create and activate virtual environment
python -m venv venv
source venv/bin/activate  # On Windows: venv\Scripts\activate

# Install required packages
pip install flask
pip install flask-cors
```

#### Shadow System Setup
1. Navigate to the shadow directory:
   ```bash
   cd shadow
   ```
2. Configure tokens in `src/main.py`:
   ```python
   TOKENS = {
       "admin-token-123": "admin",
       "user-token-456": "user",
       "device-token-789": "device"
   }
   ```
3. Start the shadow server:
   ```bash
   python src/main.py
   ```

#### Authentication System Setup
1. Navigate to the auth directory:
   ```bash
   cd auth
   ```
2. Start the authentication server:
   ```bash
   python src/main.py
   ```

### 2. Frontend Setup

#### Apache2 Configuration
1. Install Apache2:
   ```bash
   sudo apt update
   sudo apt install apache2
   ```

2. Enable required modules:
   ```bash
   sudo a2enmod proxy
   sudo a2enmod proxy_http
   sudo a2enmod ssl
   ```

3. Configure SSL virtual host:
   ```bash
   sudo nano /etc/apache2/sites-available/www-le-ssl.conf
   ```
   Add the following configuration:
   ```apache
   ProxyPreserveHost On
   ProxyPass "/api/" "http://127.0.0.1:5000/"
   ProxyPassReverse "/api/" "http://127.0.0.1:5000/"
   ProxyPass "/authapi/" "http://127.0.0.1:6000/"
   ProxyPassReverse "/authapi/" "http://127.0.0.1:6000/"
   ```

4. Deploy frontend files:
   ```bash
   sudo cp frontend/*.html /var/www/html/
   sudo cp frontend/LED.jpg /var/www/html/
   ```

### 3. ESP32 Gateway Setup

#### Development Environment
1. Install PlatformIO:
   - Follow instructions at https://platformio.org/install

2. Configure ESP32 project:
   ```bash
   cd LocalGateway_ESP-32S
   ```

3. Edit `src/main.cpp` to set your network credentials:
   ```cpp
   const char *SSID = "your-wifi-name";
   const char *PASSWORD = "your-wifi-password";
   const char *TOKEN = "device-token-789";
   ```

4. Build and upload:
   ```bash
   pio run -t upload
   ```

### 4. System Verification

1. Verify backend services:
   - Shadow server should be running on port 5000
   - Auth server should be running on port 6000
   - Check logs for any errors

2. Verify frontend:
   - Access https://your-domain.com/login.html
   - Test login with credentials
   - Verify device control interface

3. Verify ESP32 gateway:
   - Check serial monitor for connection status
   - Verify WiFi connection
   - Test local and cloud mode switching

### 5. Security Considerations

1. Change default tokens and passwords
2. Enable HTTPS for all communications
3. Implement proper user authentication
4. Set up firewall rules
5. Regular security updates
6. Monitor system logs

### 6. Troubleshooting

Common issues and solutions:
1. Port conflicts:
   - Check if ports 5000 and 6000 are available
   - Use `netstat -tulpn | grep LISTEN`

2. Apache configuration:
   - Check syntax: `sudo apache2ctl configtest`
   - View logs: `sudo tail -f /var/log/apache2/error.log`

3. ESP32 connection:
   - Verify WiFi credentials
   - Check server accessibility
   - Monitor serial output

4. Shadow synchronization:
   - Verify JSON file permissions
   - Check API endpoint accessibility
   - Monitor server logs
