# IoT Shadow 系統認證伺服器

這是一個基於 Flask 的認證伺服器，為 IoT Shadow 系統提供使用者認證與時間同步服務。它處理使用者登入、會話管理和伺服器時間同步。

## 功能特色
- 具備角色權限的使用者認證
- 使用 HTTP-only cookies 的安全會話管理
- 伺服器時間同步
- 簡單的記憶體使用者資料庫
- RESTful API 端點

## API 端點

### 認證
- **POST /login**
  - 驗證使用者並建立會話
  - 請求內容：
    ```json
    {
      "username": "字串",
      "password": "字串"
    }
    ```
  - 回應：
    ```json
    {
      "status": "ok",
      "username": "字串",
      "role": "字串"
    }
    ```
  - 設定包含使用者 token 的 HTTP-only cookie

### 使用者資訊
- **GET /whoami**
  - 根據會話 token 回傳目前使用者資訊
  - 回應：
    ```json
    {
      "username": "字串",
      "role": "字串"
    }
    ```
  - 需要有效的會話 cookie

### 時間同步
- **GET /servertime**
  - 回傳目前伺服器時間（Unix 時間戳）
  - 回應：
    ```json
    {
      "unixtime": 整數
    }
    ```

## 使用者角色
- **管理員**
  - 完整系統存取權限
  - 可管理裝置權限
  - Token: "admin-token-123"
- **一般使用者**
  - 基本裝置控制權限
  - 有限權限
  - Token: "user-token-456"

## 安全功能
- 使用 HTTP-only cookies 進行會話管理
- SameSite cookie 政策
- 角色權限控制
- 安全密碼處理
- Token 認證機制

## 設定
預設設定：
- 主機：0.0.0.0
- 連接埠：6000
- 開發模式：啟用

## 相依套件
- Flask
- Python 3.x

## 快速開始

1. 安裝相依套件：
   ```bash
   pip install flask
   ```

2. 執行伺服器：
   ```bash
   python src/main.py
   ```

3. 伺服器將在連接埠 6000 啟動

## 開發注意事項
- 目前實作使用記憶體儲存使用者資料
- 正式環境應實作適當的資料庫儲存
- 加入密碼雜湊處理
- 實作 token 過期機制
- 加入請求頻率限制
- 啟用 HTTPS

## 授權
本專案僅供教育用途。允許自由修改與應用於學術專案。

---

*若用於正式環境，請確保實作適當的安全措施。*
