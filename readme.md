# IOT_Shadow_Display_UnixFinal

This is a final project for a Unix course. The main goal of this project is to simulate the operation of AWS IoT Shadow and implement a complete IoT system architecture, including:

- Web-based control (frontend)
- Local device control (backend)
- Remote permission management
- Actual hardware control (e.g., light bulb on/off)
- Shadow synchronization using local JSON instead of database

The system emulates AWS Shadow by handling **desired**, **reported**, and **delta** states through a custom JSON-based backend logic.

---

## 📦 System Environment

- OS: Debian 12
- Web Server: Flask (Python)
- Storage: JSON file (one per device, stored in `shadow/`)
- Programming Languages: Python / HTML / JS / Bash

---

## ⚙️ System Setup and Preparation

### 1. Create Python virtual environment and install Flask

```bash
sudo apt update && sudo apt install python3-venv -y
python3 -m venv venv
source venv/bin/activate
pip install flask
```

### 2. Project Structure

```bash
IOT_Shadow_Display_UnixFinal/
├── shadow/
│   └── shadow_device001.json     # Device shadow (auto-created)
│
├── src/
│   └── main.py                   # Flask API (handles update/get)
│
├── static/
│   └── index.html                # Web control UI (central control)
│
└── requirements.txt              # Python dependencies
```

---

## 🧠 System Architecture

```
[Frontend (HTML/JS)]
        ↓
    [Flask API]
        ↓
[JSON-based Shadow State]
        ↓
[Local Device Control Logic]
```

- Frontend controls desired state and permission via HTTP requests
- Backend (Flask) handles shadow update/get APIs and calculates delta
- Local gateway checks delta and updates reported state
- Devices act based on the updated instructions

---

## ☁️ Shadow System Description

Each device has an individual JSON file named `shadow_<device_id>.json`.

### JSON format:

```json
{
  "state": {
    "desired": {
      "status": 1,
      "permission": 1
    },
    "reported": {
      "status": 0,
      "permission": 1
    }
  },
  "delta": {
    "status": 1
  }
}
```

- `desired`: What the user or frontend wants
- `reported`: Actual status from the device
- `delta`: Difference between `desired` and `reported` (only generated when needed)

---

## 🔐 Remote Permission Control

- Admin (central control) can enable/disable local device control by setting `permission`.
- Local gateway may support an **emergency override switch** if central is unreachable.
- Local devices follow `desired.status` only if `permission = 1`, unless emergency mode is triggered.

---

## 🖥️ Frontend Control Page

Accessible via browser (hosted locally or remotely), features:

- Display current device status
- Toggle device on/off
- Toggle permission for local gateway control

---

## 🧪 Testing Environment

You can simulate device behavior with Python scripts instead of real hardware.

- Local polling scripts check delta every few seconds
- Scripts update `reported` state after acting on `delta`

---

## 🙌 Contributors

- Albert Wang（王建葦） – System Design, Backend Integration
- Andy Chean（陳稚翔） – Frontend UI & API Integration

---

## 📜 License

This project is for educational use only. No license required.
