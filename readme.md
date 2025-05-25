## 中文版說明
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/blob/frontend/readme_zh-tw.md

# Frontend README – Central Control Web Interface

This frontend interface is part of the IOT_Shadow_Display_UnixFinal project. It is designed to serve as the central control panel (control room) for monitoring and managing remote IoT devices using a simple HTML + JavaScript interface. It communicates with the Flask-based Shadow API and includes user authentication.

---

## ✅ Features

1. **User Authentication**
   - Login system with username/password
   - Role-based access control (admin vs regular users)
   - Session management with cookies
   - Automatic redirection to login page when unauthorized

2. **Display Device Status**
   - Fetches current device state from `/shadow/get?type=reported`
   - Shows device status as "Online" or "Offline" based on time sync
   - Device is considered offline when its reported time differs from server time by more than 10 seconds
   - Updates status every 10 seconds
   - Visual indicators for device state

3. **Control Remote Device Switch**
   - Toggle switch controls device power state
   - Sends update to `/shadow/update` by modifying `desired.status`
   - Visual feedback during state transitions
   - Disables control when device is offline or locked

4. **Manage Local Gateway Permission**
   - Toggle switch controls `desired.permission`
   - `permission = 1`: allow local device control
   - `permission = 0`: deny local control (central control only)
   - Admin-only access for permission management
   - Disables when device is locked (`localper = 1`)

---

## 🛠️ How to Use

1. **Apache Server Setup**
   - Enable required Apache modules:
     ```bash
     sudo a2enmod proxy
     sudo a2enmod proxy_http
     ```
   - Configure SSL virtual host (www-le-ssl.conf):
     ```apache
     ProxyPreserveHost On
     ProxyPass "/api/" "http://127.0.0.1:5000/"
     ProxyPassReverse "/api/" "http://127.0.0.1:5000/"
     ProxyPass "/authapi/" "http://127.0.0.1:6000/"
     ProxyPassReverse "/authapi/" "http://127.0.0.1:6000/"
     ```

2. Deploy the files:
   - Place `index.html` and `login.html` in your web server directory
   - Ensure `LED.jpg` is available for device image
   - Configure API endpoints in the JavaScript code:
     ```js
     const apiBase = "https://your-domain.com/api/shadow";
     const authapiBase = "https://your-domain.com/authapi";
     ```

3. Access the interface:
   - Open the web interface in a browser
   - Login with valid credentials
   - The control panel will automatically load and start monitoring

---

## 🔧 Required API Endpoints

| Method | Endpoint                    | Description                                              |
| ------ | --------------------------- | -------------------------------------------------------- |
| GET    | `/shadow/get?type=reported` | Get current device status                                |
| GET    | `/shadow/get?type=desired`  | Get current control permissions                          |
| POST   | `/shadow/update`            | Send updates to `desired.status` or `desired.permission` |
| POST   | `/authapi/login`            | User authentication                                      |
| GET    | `/authapi/whoami`           | Get current user info                                    |
| GET    | `/authapi/servertime`       | Get server time for sync                                 |

---

## 🧪 Example Control Flow

1. User Authentication:
   ```json
   POST /authapi/login
   {
     "username": "user",
     "password": "pass"
   }
   ```

2. Device Control:
   ```json
   POST /shadow/update
   {
     "type": "desired",
     "data": { "status": 1 }
   }
   ```

3. Permission Update:
   ```json
   POST /shadow/update
   {
     "type": "desired",
     "data": { "permission": 1 }
   }
   ```

---

## 📁 File Structure

- `index.html` – Main control panel UI
- `login.html` – User authentication page
- `LED.jpg` – Device image
- Pure HTML + CSS + JavaScript (no external libraries)

---

## 📜 License

This frontend interface is provided for educational purposes only. Free to modify and use in academic projects.

