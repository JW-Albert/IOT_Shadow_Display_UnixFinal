# IOT_Shadow_Display_UnixFinal – 專案總說明

## English version description
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/blob/main/readme.md
- 
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

- **後端：** Python（Flask）
- **前端：** HTML / JavaScript（AJAX）
- **儲存方式：** JSON 檔案（每台裝置一份）
- **系統平台：** Debian 12（Linux）

---

## 📁 專案目錄結構

```
IOT_Shadow_Display_UnixFinal/
├── src/             # Flask 後端
├── shadow/          # Shadow JSON 檔案
├── static/          # 網頁前端
├── docs/            # 文件資料夾
└── venv/            # Python 虛擬環境
```

---

## 🔐 適用情境

- IoT 裝置控制與狀態同步
- 地端邊緣運算應用
- 中控失效時需緊急控制的場景
- 教學用途或嵌入式開發原型

---

## 🙌 參與成員

- 王建葦（Albert Wang）– 系統架構設計、後端與 Shadow 實作
- 陳稚翔（Andy Chean）– 前端介面設計與整合

---

## 📜 授權條款

本專案為課程作業用途，開放自由修改與延伸使用。
