# IOT_Shadow_Display_UnixFinal – 專案總說明

## English version description
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/blob/main/readme.md

## 🔍 專案簡介

IOT_Shadow_Display_UnixFinal 是一個 Unix 課程期末專案，模擬 AWS IoT Shadow 的運作，實作一套完整的 IoT 控制架構，包含：

- 中控網頁介面操作
- 地端 Gateway 控制與緊急接管邏輯
- 透過自製 Shadow 系統同步裝置狀態

本系統使用 Flask 與 JSON 檔案進行狀態管理，不需使用雲端服務與資料庫，適合輕量化、離線或嵌入式環境使用。

---

## 🧠 主要特色

- 設備 Shadow 機制（desired / reported / delta）
- RESTful API + 金鑰驗證
- 簡潔前端介面操作與監控
- 支援地端緊急模式控制
- 多裝置支援（每台裝置一個 JSON 檔）

---

## 🖥️ 技術架構

- **後端：** 
  - Python（Flask）
  - ESP32（PlatformIO）
- **前端：** 
  - HTML / JavaScript（AJAX）
  - Apache2 網頁伺服器
- **儲存方式：** 
  - JSON 檔案（每台裝置一份）
  - 本地檔案系統
- **系統平台：** 
  - Debian 12（Linux）
  - ESP32 微控制器
- **安全性：**
  - API 金鑰驗證
  - 網頁登入系統

---

## 📁 專案目錄結構

```
IOT_Shadow_Display_UnixFinal/
├── LocalGateway_ESP-32S/  # ESP32 閘道器實作
│   ├── src/              # ESP32 原始碼
│   └── platformio.ini    # PlatformIO 設定檔
├── frontend/             # 網頁介面
│   ├── index.html       # 主控制介面
│   ├── login.html       # 登入頁面
│   └── apache2/         # 網頁伺服器設定
├── auth/                 # 認證系統
├── shadow/              # Shadow JSON 檔案
└── main/                # 核心系統元件
```

---

## 🔐 適用情境

- IoT 裝置控制與狀態同步
- 地端邊緣運算應用
- 中控失效時需緊急控制的場景
- 教學用途或嵌入式開發原型
- ESP32 閘道器實作
- 安全的網頁式裝置管理

---

## 🙌 參與成員

- 王建葦（Albert Wang）– 系統架構設計、後端與 Shadow 實作
- 陳稚翔（Andy Chen）– 前端介面設計與整合

---

## 📜 授權條款

本專案為課程作業用途，開放自由修改與延伸使用。

## 🚀 環境建置與安裝指南

### 系統需求
- Debian 12（Linux）或相容系統
- Python 3.x
- Apache2 網頁伺服器
- ESP32 開發板（用於閘道器）
- PlatformIO 或 Arduino IDE（用於 ESP32 開發）

### 1. 後端建置

#### Python 環境
```bash
# 建立並啟動虛擬環境
python -m venv venv
source venv/bin/activate  # Windows：venv\Scripts\activate

# 安裝必要套件
pip install flask
pip install flask-cors
```

#### Shadow 系統建置
1. 進入 shadow 目錄：
   ```bash
   cd shadow
   ```
2. 在 `src/main.py` 中設定 token：
   ```python
   TOKENS = {
       "admin-token-123": "admin",
       "user-token-456": "user",
       "device-token-789": "device"
   }
   ```
3. 啟動 shadow 伺服器：
   ```bash
   python src/main.py
   ```

#### 認證系統建置
1. 進入 auth 目錄：
   ```bash
   cd auth
   ```
2. 啟動認證伺服器：
   ```bash
   python src/main.py
   ```

### 2. 前端建置

#### Apache2 設定
1. 安裝 Apache2：
   ```bash
   sudo apt update
   sudo apt install apache2
   ```

2. 啟用必要模組：
   ```bash
   sudo a2enmod proxy
   sudo a2enmod proxy_http
   sudo a2enmod ssl
   ```

3. 設定 SSL 虛擬主機：
   ```bash
   sudo nano /etc/apache2/sites-available/www-le-ssl.conf
   ```
   加入以下設定：
   ```apache
   ProxyPreserveHost On
   ProxyPass "/api/" "http://127.0.0.1:5000/"
   ProxyPassReverse "/api/" "http://127.0.0.1:5000/"
   ProxyPass "/authapi/" "http://127.0.0.1:6000/"
   ProxyPassReverse "/authapi/" "http://127.0.0.1:6000/"
   ```

4. 部署前端檔案：
   ```bash
   sudo cp frontend/*.html /var/www/html/
   sudo cp frontend/LED.jpg /var/www/html/
   ```

### 3. ESP32 閘道器建置

#### 開發環境
1. 安裝 PlatformIO：
   - 依照 https://platformio.org/install 的指示安裝

2. 設定 ESP32 專案：
   ```bash
   cd LocalGateway_ESP-32S
   ```

3. 編輯 `src/main.cpp` 設定網路認證資訊：
   ```cpp
   const char *SSID = "您的WiFi名稱";
   const char *PASSWORD = "您的WiFi密碼";
   const char *TOKEN = "device-token-789";
   ```

4. 編譯並上傳：
   ```bash
   pio run -t upload
   ```

### 4. 系統驗證

1. 驗證後端服務：
   - Shadow 伺服器應在連接埠 5000 運行
   - 認證伺服器應在連接埠 6000 運行
   - 檢查日誌是否有錯誤

2. 驗證前端：
   - 存取 https://您的網域/login.html
   - 使用認證資訊測試登入
   - 驗證裝置控制介面

3. 驗證 ESP32 閘道器：
   - 檢查序列埠監視器中的連線狀態
   - 驗證 WiFi 連線
   - 測試本地與雲端模式切換

### 5. 安全性考量

1. 更改預設 token 和密碼
2. 為所有通訊啟用 HTTPS
3. 實作適當的使用者認證
4. 設定防火牆規則
5. 定期安全性更新
6. 監控系統日誌

### 6. 故障排除

常見問題與解決方案：
1. 連接埠衝突：
   - 檢查連接埠 5000 和 6000 是否可用
   - 使用 `netstat -tulpn | grep LISTEN`

2. Apache 設定：
   - 檢查語法：`sudo apache2ctl configtest`
   - 查看日誌：`sudo tail -f /var/log/apache2/error.log`

3. ESP32 連線：
   - 驗證 WiFi 認證資訊
   - 檢查伺服器可存取性
   - 監控序列埠輸出

4. Shadow 同步：
   - 驗證 JSON 檔案權限
   - 檢查 API 端點可存取性
   - 監控伺服器日誌
