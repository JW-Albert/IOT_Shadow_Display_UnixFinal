## English version description
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/tree/Shadow/readme.md

# Shadow 系統實作說明

這是一個基於 Flask 的裝置 Shadow 系統實作，類似於 AWS IoT Device Shadow。透過 RESTful API 管理裝置狀態與權限。

## 功能特色
- 裝置狀態管理（desired/reported/delta）
- 角色權限控制
- 權限管理
- JSON 檔案儲存
- RESTful API 端點

## API 端點

### Shadow 管理
- **GET /shadow/get**
  - 查詢參數：
    - `type`: "desired", "reported", "delta" 或 "full"
  - 回傳請求的 shadow 狀態
  - 回應範例：
    ```json
    {
      "status": 1,
      "permission": 1
    }
    ```

- **POST /shadow/update**
  - 更新 shadow 狀態
  - 請求內容：
    ```json
    {
      "type": "desired",
      "data": {
        "status": 1,
        "permission": 1
      }
    }
    ```
  - 回應：
    ```json
    {
      "message": "desired updated",
      "delta": {
        "status": 1
      }
    }
    ```

## Shadow 結構
```json
{
  "state": {
    "desired": {
      "status": 0,
      "permission": 1
    },
    "reported": {
      "status": 0,
      "permission": 1
    }
  },
  "delta": {}
}
```

## 狀態管理
- **Desired State**：系統希望裝置達成的狀態
- **Reported State**：裝置實際的狀態
- **Delta**：期望與實際狀態的差異

## 權限控制
- 只有管理員可以修改裝置權限
- 一般使用者只能控制裝置狀態
- 權限值：
  - 1：允許
  - 0：禁止

## 檔案儲存
- Shadow 狀態儲存於 JSON 檔案
- 預設位置：`../shadow/shadow.json`
- 檔案不存在時自動建立

## 安全機制
- Token 認證
- 角色權限控制
- 請求日誌記錄
- 錯誤處理

## 相依套件
- Flask
- Python 3.x
- JSON 檔案處理

## 快速開始

1. 安裝相依套件：
   ```bash
   pip install flask
   ```

2. 在 `main.py` 中設定 token：
   ```python
   TOKENS = {
       "admin-token-123": "admin",
       "user-token-456": "user",
       "device-token-789": "device"
   }
   ```

3. 執行伺服器：
   ```bash
   python src/main.py
   ```

## 開發注意事項
- 目前實作使用檔案儲存
- 正式環境建議：
  - 實作資料庫儲存
  - 加入適當的 token 管理
  - 啟用 HTTPS
  - 加入請求頻率限制
  - 實作適當的日誌記錄

## 授權
本專案僅供教育用途。允許自由修改與應用於學術專案。

---

*若用於正式環境，請確保實作適當的安全措施。*
