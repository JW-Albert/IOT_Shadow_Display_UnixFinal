# IOT_Shadow_Display_UnixFinal
This is a final project for a Unix course. The main goal of this project is to simulate the operation of AWS IoT Shadow and implement a complete IoT system architecture, including:
- Web-based control (frontend)
- Local device control (backend)
- Remote permission management (admin system)
- Actual hardware control (e.g., light bulb on/off)
The system emulates how device shadow works in AWS by handling **desired**, **reported**, and **delta** states through a custom JSON-based protocol and backend logic.

---

## 📦 System Environment
- OS: Debian 12
- Web Server: Apache2
- Database: MariaDB
- Programming Languages: PHP / Python / Bash
- Frontend: HTML / JS / CSS (Bootstrap)
- Backend: Python + C ++ + Bash

---

## ⚙️ System Setup and Preparation
### 1. Update and upgrade apt
```bash
apt update -y && apt upgrade -y
```

### 2. Add a new user and create a dev group
```bash
adduser NAME
groupadd dev
adduser NAME dev
```

### 3. Install Apache2
```bash
apt install apache2
```

### 4. Grant permission to dev group members
```bash
chgrp -R dev /var/www/html/*
chmod -R 774 /var/www/html/*
```

---

### 5. Install MariaDB
```bash
apt install mariadb-server -y
systemctl start mariadb
systemctl enable mariadb
mysql_secure_installation
```

### 6. Create MariaDB admin user
```bash
mysql -u root -p
```

Then in MySQL shell:
```sql
CREATE USER 'admin'@'localhost' IDENTIFIED BY 'your_password';
GRANT ALL PRIVILEGES ON *.* TO 'admin'@'localhost' WITH GRANT OPTION;
FLUSH PRIVILEGES;
```

---

### 7. Create a SQL user for the frontend
```sql
CREATE DATABASE shadow_control;
CREATE USER 'webuser'@'localhost' IDENTIFIED BY 'your_password';
GRANT SELECT, INSERT, UPDATE, DELETE ON shadow_control.* TO 'webuser'@'localhost';
FLUSH PRIVILEGES;
```

---

## 🧱 Project Structure
```bash
IOT_Shadow_Display_UnixFinal/
├── html/                     # Frontend website
│   ├── index.html            # Main control panel
│   └── admin.php             # Admin interface
│
├── scripts/
│   ├── update_shadow.py      # Backend logic to sync shadow state
│   ├── read_gpio.py          # Read status from device
│   └── control_gpio.py       # Control GPIO or virtual light
│
├── db/
│   └── init.sql              # SQL schema for the shadow database
│
└── docs/
    └── README.md             # This file
```

---

## 🧠 System Architecture
```csharp
[Frontend Website]
     |
     V
[PHP/API → MySQL]
     |
     V
[Backend Python/Bash Scripts]
     |
     V
[GPIO / COM / Device Simulator]
```

- **Frontend:** Allows users to set the desired state.
- **Database:** Stores desired/reported states, calculates delta.
- **Backend:** Periodically compares desired/reported, calculates delta, and sends commands to device.
- **Hardware Layer:** Simulates or controls actual GPIO (e.g., LEDs, relays).

---

## ☁️ Shadow System Description
Each device has the following structure:

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

- ```desired```**:** what the frontend or user wants.
- ```reported```**:** the actual status of the device.
- ```delta```**:** the difference between desired and reported. Only sent if changed.

### Workflow:
- 1. User changes status/permission via website.
- 2. Backend script checks database every few seconds.
- 3. If ```delta``` is not empty, it updates the hardware state.
- 4. After applying the change, backend updates ```reported```.

---

## 🔐 Remote Permission Control
Admin accounts can:
- Change device permission flags.
- Control whether local or remote users can override the state.
- View activity logs and delta history.

---

### 🧪 Testing Environment
You can test without GPIO using a simulation:
- ```control_gpio.py``` → prints instead of turning on actual device.
- Virtual "light" or "device" object changes color or logs state.

---

### 🙌 Contributors
- Albert Wang（王建葦） - System Design, Backend & Integration
- Andy Chean（陳稚翔） - Frontend Design

---
### 📜 License
This project is for educational purposes only. No license required.