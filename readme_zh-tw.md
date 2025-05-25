# ESP32S 地端裝置控制本地閘道器

本專案使用 ESP32S 實作本地閘道器，透過雲端 Shadow API 控制裝置。系統支援雲端與本地雙模式運作，具備優先權處理與狀態同步功能。

## 特色
- 雙模式運作（雲端/本地）
- 與雲端 Shadow API 安全通訊
- 即時裝置狀態同步
- 本地權限控制
- 雲端權限管理
- 基於時間的裝置狀態監控
- 自動模式切換
- 輸入防彈跳處理

## 硬體需求
- ESP32S 開發板
- 輸入腳位：
  - GPIO 36：雲端模式開關（ADC 輸入）
  - GPIO 39：本地權限控制（INPUT_PULLUP）
  - GPIO 34：本地裝置控制（INPUT_PULLUP）
- 輸出腳位：
  - GPIO 4：繼電器控制（OUTPUT）

## 軟體需求
- [PlatformIO](https://platformio.org/) 或 Arduino IDE
- 必要函式庫：
  - WiFi
  - WiFiClientSecure
  - HTTPClient
  - ArduinoJson
  - time.h

## 設定
編輯 `src/main.cpp` 設定網路與認證資訊：
```cpp
const char *SSID = "您的WiFi名稱";
const char *PASSWORD = "您的WiFi密碼";
const char *TOKEN = "您的裝置token";
```

## API 端點
- 伺服器時間：`https://unix.jw-albert.dev/authapi/servertime`
- Shadow Delta：`https://unix.jw-albert.dev/api/shadow/get?type=delta`
- Shadow Desired：`https://unix.jw-albert.dev/api/shadow/get?type=desired`
- Shadow Update：`https://unix.jw-albert.dev/api/shadow/update`

## 系統運作

### 1. 初始化
- 系統開機與重置資訊記錄
- GPIO 腳位配置
- 透過 ADC 輸入偵測雲端模式
- WiFi 連線（雲端模式）
- 初始狀態同步

### 2. 雲端模式運作
- 維持 WiFi 連線
- 每 1.5 秒輪詢 delta 更新
- 回報裝置狀態至雲端
- 處理權限變更
- 與伺服器時間同步
- 時間差超過 10 秒判定裝置離線

### 3. 本地模式運作
- 直接 GPIO 控制
- 本地權限管理
- 狀態回報至雲端（若連線）
- 自動模式切換

### 4. 狀態管理
- 裝置狀態（開/關）
- 雲端權限（0/1）
- 本地權限（0/1）
- 運作模式（雲端/本地）
- 時間同步

### 5. 錯誤處理
- WiFi 重連
- HTTP 錯誤恢復
- 輸入防彈跳（50ms）
- 狀態驗證
- 自動切換至本地模式

## 控制流程

### 雲端控制
1. 使用者在雲端更新期望狀態
2. ESP32S 輪詢 delta 端點
3. 系統在允許的情況下套用變更
4. 狀態回報至雲端

### 本地控制
1. 本地權限腳位啟用控制
2. 控制腳位直接影響裝置
3. 若連線則回報狀態至雲端
4. 本地權限啟用時停用雲端控制

## 除錯資訊
- 序列埠輸出（115200 baud）
- 系統重置追蹤
- 狀態變更記錄
- 錯誤回報
- 連線狀態

## 客製化
- 調整防彈跳延遲（DEBOUNCE_DELAY）
- 修改輪詢間隔（預設 1.5 秒）
- 變更 GPIO 腳位配置
- 更新 API 端點
- 修改時間同步閾值

## 安全功能
- HTTPS 通訊
- Token 認證
- 本地權限控制
- 雲端權限管理
- 安全 WiFi 連線

## 授權
本專案僅供教育用途。允許自由修改與應用於學術專案。

---

*若用於正式環境，請確保實作適當的安全措施與憑證驗證。*
