# IOT_Shadow_Display_UnixFinal – Project Overview (English)

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

- **Backend:** Python (Flask)
- **Frontend:** HTML / JavaScript (AJAX)
- **Storage:** JSON file (one per device)
- **System:** Debian 12 (Linux)

---

## 📁 Directory Structure

```
IOT_Shadow_Display_UnixFinal/
├── src/             # Flask backend
├── shadow/          # Shadow JSON files
├── static/          # Web frontend
├── docs/            # Documentation
└── venv/            # Python virtual environment
```

---

## 🔐 Use Cases

- IoT device control and monitoring
- Local edge computing scenarios
- Systems requiring emergency fallback control
- Educational use or embedded system prototyping

---

## 🙌 Credits

- Albert Wang（王建葦） – System Architecture, Backend, Shadow Logic
- Andy Chean（陳稚翔） – Frontend Interface Design

---

## 📜 License

This project is provided for educational use only. Free to modify and extend.
