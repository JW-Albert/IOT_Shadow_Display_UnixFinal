## English version description
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/blob/frontend/readme.md

# 中控室前端控制介面說明 (Frontend README)

本前端網頁為 IOT_Shadow_Display_UnixFinal 系統中的中控室控制介面，使用純 HTML + JavaScript 建構，透過呼叫 Flask API 來達成對設備的監控與控制，並包含使用者認證功能。

---

## ✅ 功能說明

1. **使用者認證系統**
   - 使用者名稱/密碼登入系統
   - 角色權限控制（管理員與一般使用者）
   - 使用 Cookie 進行會話管理
   - 未認證時自動重定向至登入頁面

2. **顯示設備狀態**
   - 從 `/shadow/get?type=reported` 取得設備目前狀態
   - 根據時間同步顯示設備「上線」或「離線」狀態
   - 當設備回報時間與伺服器時間差距超過 10 秒時，判定設備離線
   - 每 10 秒自動更新狀態
   - 提供視覺化狀態指示

3. **遠端控制設備開關**
   - 使用切換開關控制設備電源狀態
   - 呼叫 `/shadow/update` 設定 `desired.status`
   - 狀態轉換時提供視覺回饋
   - 設備離線或鎖定時禁用控制

4. **設定地端控制權限**
   - 使用切換開關控制 `desired.permission`
   - `permission = 1`：允許地端操作
   - `permission = 0`：禁止地端操作
   - 僅管理員可修改權限設定
   - 設備鎖定時（`localper = 1`）禁用權限控制

---

## 🛠️ 使用方式

1. **Apache 伺服器設定**
   - 啟用必要的 Apache 模組：
     ```bash
     sudo a2enmod proxy
     sudo a2enmod proxy_http
     ```
   - 設定 SSL 虛擬主機 (www-le-ssl.conf)：
     ```apache
     ProxyPreserveHost On
     ProxyPass "/api/" "http://127.0.0.1:5000/"
     ProxyPassReverse "/api/" "http://127.0.0.1:5000/"
     ProxyPass "/authapi/" "http://127.0.0.1:6000/"
     ProxyPassReverse "/authapi/" "http://127.0.0.1:6000/"
     ```

2. 部署檔案：
   - 將 `index.html` 和 `login.html` 放置於網頁伺服器目錄
   - 確保 `LED.jpg` 設備圖片可用
   - 在 JavaScript 程式碼中設定 API 端點：
     ```js
     const apiBase = "https://your-domain.com/api/shadow";
     const authapiBase = "https://your-domain.com/authapi";
     ```

3. 存取介面：
   - 在瀏覽器中開啟網頁介面
   - 使用有效憑證登入
   - 控制面板將自動載入並開始監控

---

## 🔧 所需 API 介面

| 方法 | 路徑                        | 說明                                      |
| ---- | --------------------------- | ----------------------------------------- |
| GET  | `/shadow/get?type=reported` | 取得目前設備狀態                          |
| GET  | `/shadow/get?type=desired`  | 取得目前設定的權限狀態                    |
| POST | `/shadow/update`            | 傳送 desired.status 或 desired.permission |
| POST | `/authapi/login`            | 使用者認證                                |
| GET  | `/authapi/whoami`           | 取得目前使用者資訊                        |
| GET  | `/authapi/servertime`       | 取得伺服器時間進行同步                    |

---

## 🧪 範例互動流程

1. 使用者認證：
   ```json
   POST /authapi/login
   {
     "username": "user",
     "password": "pass"
   }
   ```

2. 設備控制：
   ```json
   POST /shadow/update
   {
     "type": "desired",
     "data": { "status": 1 }
   }
   ```

3. 權限更新：
   ```json
   POST /shadow/update
   {
     "type": "desired",
     "data": { "permission": 1 }
   }
   ```

---

## 📁 檔案說明

- `index.html`：主控制面板介面
- `login.html`：使用者認證頁面
- `LED.jpg`：設備圖片
- 純 HTML + CSS + JavaScript（無外部函式庫）

---

## 📜 授權

本前端介面為教育專案用途，允許自由修改與應用。
