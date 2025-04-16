# 中控室前端控制介面說明 (Frontend README)

本前端網頁為 IOT_Shadow_Display_UnixFinal 系統中的中控室控制介面，使用純 HTML + JavaScript 建構，透過呼叫 Flask API 來達成對設備的監控與控制。

---

## ✅ 功能說明

1. **顯示設備狀態**
   - 從 `/shadow/get?type=reported` 取得設備目前狀態（status: 0 或 1）
   - 將狀態顯示為「開啟」或「關閉」

2. **遠端控制設備開關**
   - 點擊「開啟設備」或「關閉設備」按鈕
   - 呼叫 `/shadow/update`，設定 `desired.status`

3. **設定地端控制權限**
   - 透過切換開關控制 `desired.permission`
   - permission = 1：允許地端操作
   - permission = 0：禁止地端操作

---

## 🛠️ 使用方式

1. 修改 `index.html` 中的設定：

```js
const apiKey = "your-api-key";               // 替換為後端的 Authorization 金鑰
const apiBase = "http://your-api-host:5000"; // 替換為後端主機位址
```

2. 部署 index.html 至任何靜態網頁伺服器，例如：
   - Apache2 (`/var/www/html/index.html`)
   - 本機開啟：用瀏覽器直接開啟 html 檔

3. 開啟網頁後，將自動：
   - 顯示目前設備狀態
   - 讀取並套用 permission 狀態

---

## 🔧 所需 API 介面

| 方法 | 路徑 | 說明 |
|------|------|------|
| GET | `/shadow/get?type=reported` | 取得目前設備狀態 |
| GET | `/shadow/get?type=desired`  | 取得目前設定的權限狀態 |
| POST | `/shadow/update` | 傳送 desired.status 或 desired.permission |

---

## 🧪 範例互動流程

1. 使用者按下「開啟設備」 → 傳送：

```json
POST /shadow/update
{
  "type": "desired",
  "data": { "status": 1 }
}
```

2. 後端比對 `reported`，產生 delta

3. 裝置後端輪詢 delta → 執行開啟 → 回報 `reported.status = 1`

4. delta 清空 → 顯示設備已開啟

---

## 📁 檔案說明

- `index.html`：前端控制主畫面
- 使用 Bootstrap + JavaScript（無額外框架）

---

## 📜 授權

本前端頁面為教育專案用途，允許自由修改與應用。

# Frontend README – Central Control Web Interface

This frontend interface is part of the IOT_Shadow_Display_UnixFinal project. It is designed to serve as the central control panel (control room) for monitoring and managing remote IoT devices using a simple HTML + JavaScript interface. It communicates directly with the Flask-based Shadow API.

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

