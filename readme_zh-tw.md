# ESP32S 地端裝置控制本地閘道器

本專案利用 ESP32S 作為本地閘道器，透過雲端 Shadow API 控制地端裝置。ESP32S 連接 WiFi，定期向雲端查詢裝置狀態（delta），並根據狀態更新本地裝置，同時回報目前裝置狀態（reported）至雲端。

## 特色
- 作為地端裝置控制的本地閘道器
- 與雲端 Shadow API 安全通訊
- 定期查詢裝置狀態（delta）
- 控制本地裝置（如板載 LED）
- 回報裝置狀態（reported）至雲端
- WiFi 與驗證 token 設定簡單

## 硬體需求
- ESP32S 開發板
- 板載 LED（或外接於 GPIO2 的裝置）

## 軟體需求
- [PlatformIO](https://platformio.org/) 或 Arduino IDE
- Arduino 函式庫：
  - WiFi
  - WiFiClientSecure
  - HTTPClient
  - ArduinoJson（版本請參考 `platformio.ini`）

## 快速開始

### 1. 下載專案原始碼
```bash
git clone -b LocalGateway_ESP-32S https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal.git
cd IOT_Shadow_Display_UnixFinal
```

### 2. 設定 WiFi 與 Token
編輯 `src/main.cpp`，設定您的 WiFi SSID、密碼與裝置 token：
```cpp
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* token = "your-device-token";
```

### 3. 編譯與燒錄
使用 PlatformIO 或 Arduino IDE 編譯並燒錄韌體至 ESP32S。

### 4. 開啟序列埠監控
以 115200 baud 開啟序列埠監控，查看日誌與除錯資訊。

## 運作方式
1. **WiFi 連線：** ESP32S 連接指定 WiFi。
2. **查詢雲端 Shadow API：**
   - 定期發送 GET 請求至 Shadow API 取得最新裝置狀態（delta）。
   - 若收到新狀態，則更新本地裝置（如開關 LED）。
3. **回報狀態：**
   - 裝置狀態更新後，發送 POST 請求至 Shadow API 回報目前狀態（reported）。
4. **錯誤處理：**
   - 自動處理 WiFi 斷線與 HTTP 錯誤。

## 檔案結構
- `src/main.cpp` — 主程式邏輯
- `platformio.ini` — PlatformIO 專案設定

## 相依套件
請參考 `platformio.ini`。主要外部函式庫：
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

## 雲端 API 端點
- **取得 Delta：** `https://unix.jw-albert.dev/api/shadow/get?type=delta`
- **回報 Reported：** `https://unix.jw-albert.dev/api/shadow/update`

## 客製化
- 如需控制其他裝置，可修改 `ledPin` 或於 `main.cpp` 增加 GPIO 控制邏輯。
- 輪詢間隔可調整 `loop` 內的 `delay(2000);`。

## 授權
請於此處標註您的授權條款（如 MIT、Apache 2.0 等）。

---

*本專案僅供教育與內部使用。若用於正式環境，請務必加強安全性與憑證驗證。*
