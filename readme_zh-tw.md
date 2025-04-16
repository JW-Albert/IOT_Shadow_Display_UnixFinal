## English version description
- https://github.com/JW-Albert/IOT_Shadow_Display_UnixFinal/tree/Shadow/readme.md

# Shadow 系統說明文件

本系統中的 Shadow 模擬 AWS IoT Device Shadow，用於在前端中控、地端 Gateway 與實體設備之間同步狀態。

---

## 什麼是 Shadow？

Shadow 是設備狀態的虛擬表示。它紀錄了期望狀態（desired）、實際回報狀態（reported）以及兩者之間的差異（delta）。

---

## JSON 結構範例

```json
{
  "state": {
    "desired": {
      "status": 1,
      "permission": 1
    },
    "reported": {
      "status": 0,
      "permission": 1
    }
  },
  "delta": {
    "status": 1
  }
}
```

- `desired`: 中控希望設備達成的狀態
- `reported`: 設備實際的狀態
- `delta`: 期望與實際的差異（只有不同時才會產生）

---

## 運作流程

1. 中控透過 API 發送 desired 狀態。
2. 系統計算 delta（desired 與 reported 差異）。
3. 地端 Gateway 抓取 delta 並執行硬體動作。
4. 裝置回報實際狀態 → 更新 reported。
5. 重新計算 delta，若一致則 delta 清空。

---

## 緊急地端控制

若中控離線，地端可啟用緊急控制模式，忽略 permission 權限進行操作，確保設備安全。

---

## 檔案格式

每個裝置各自對應一個 Shadow JSON 檔：

- `shadow_device001.json`
- `shadow_device002.json`
- ...

由 Flask 後端進行管理與讀寫。
