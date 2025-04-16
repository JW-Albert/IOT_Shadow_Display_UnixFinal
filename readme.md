# Frontend README – Central Control Web Interface

This frontend interface is part of the IOT_Shadow_Display_UnixFinal project. It is designed to serve as the central control panel (control room) for monitoring and managing remote IoT devices using a simple HTML + JavaScript interface. It communicates directly with the Flask-based Shadow API.

## 中文版說明
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/blob/frontend/readme_zh-tw.md

---

## ✅ Features

1. **Display Device Status**
   - Fetches current device state from `/shadow/get?type=reported`
   - Displays device status as "ON" or "OFF"

2. **Control Remote Device Switch**
   - Buttons allow turning the device ON or OFF
   - Sends update to `/shadow/update` by modifying `desired.status`

3. **Manage Local Gateway Permission**
   - Toggle switch controls `desired.permission`
   - `permission = 1`: allow local device control
   - `permission = 0`: deny local control (central control only)

---

## 🛠️ How to Use

1. Open `index.html` and configure your settings:

```js
const apiKey = "your-api-key";               // Replace with your backend Authorization key
const apiBase = "http://your-api-host:5000"; // Replace with your Flask API host
```

2. Deploy `index.html` on any web server:
   - Apache2 (`/var/www/html/index.html`)
   - Or simply open with a local browser

3. Upon loading, the page will:
   - Automatically fetch and display the current device status
   - Apply the current `permission` state

---

## 🔧 Required API Endpoints

| Method | Endpoint | Description |
|--------|----------|-------------|
| GET    | `/shadow/get?type=reported` | Get current device status |
| GET    | `/shadow/get?type=desired`  | Get current control permissions |
| POST   | `/shadow/update` | Send updates to `desired.status` or `desired.permission` |

---

## 🧪 Example Control Flow

1. User clicks "Turn ON" button → Sends:

```json
POST /shadow/update
{
  "type": "desired",
  "data": { "status": 1 }
}
```

2. Backend compares with `reported`, generates `delta`

3. Device polls `delta`, performs action, and reports back `reported.status = 1`

4. `delta` becomes empty → System is synchronized

---

## 📁 File Structure

- `index.html` – Central control panel UI
- HTML + Bootstrap + JavaScript (no external libraries)

---

## 📜 License

This frontend page is provided for educational purposes only. Free to modify and use in academic projects.

