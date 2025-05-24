#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi 設定
const char *ssid = "iOT";
const char *password = "Albert0801";

// Shadow API base
const char *SHADOW_GET_DELTA_URL = "https://unix.jw-albert.dev/api/shadow/get?type=delta";
const char *SHADOW_GET_DESIRED_URL = "https://unix.jw-albert.dev/api/shadow/get?type=desired";
const char *SHADOW_UPDATE_URL = "https://unix.jw-albert.dev/api/shadow/update";

// IO 腳位定義
// 上雲控制開關 (輸入)
const int cloudSwitchPin = 36;
// 地端權限控制 (輸入)
const int localAuthPin = 39;
// 地端裝置控制 (輸入)
const int localControlPin = 34;
// 繼電器控制 (輸出)
const int relayPin = 02;

// Token 用作 cookie 模擬登入
const char *token = "device-token-789";

WiFiClientSecure client;
HTTPClient https;

// 系統狀態變數
bool cloudEnabled = false;     // 雲端控制是否啟用
bool localAuthEnabled = false; // 地端權限是否啟用
bool deviceStatus = false;     // 裝置狀態
bool isInitialized = false;    // 初始化狀態
bool isCloudMode = false;      // 是否為雲端模式
bool wasCloudMode = false;     // 上一次的雲端模式狀態

// 除錯用計數器
unsigned long lastResetTime = 0;
int resetCount = 0;

// 開關狀態相關變數
bool lastCloudSwitchValue = false;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50; // 防彈跳延遲（毫秒）

// 新增全域變數
int cloudPermission = 0; // 雲給地權限 0: 不允許地端, 1: 允許地端

void printResetReason()
{
  Serial.println("\n=== 系統重啟資訊 ===");
  Serial.print("重啟次數: ");
  Serial.println(resetCount);
  Serial.print("距離上次重啟時間: ");
  Serial.print((millis() - lastResetTime) / 1000.0);
  Serial.println(" 秒");
  Serial.println("===================\n");
}

void setup()
{
  Serial.begin(115200);
  delay(1000); // 等待序列埠穩定

  // 更新重啟資訊
  resetCount++;
  if (lastResetTime == 0)
  {
    lastResetTime = millis();
  }
  printResetReason();

  Serial.println("開始初始化系統...");

  // 初始化所有IO腳位
  Serial.println("初始化IO腳位...");

  // 不需要 pinMode(cloudSwitchPin, INPUT_PULLUP); // GPIO36 不支援內部上拉
  delay(100); // 等待穩定

  int initialValue = analogRead(cloudSwitchPin);
  Serial.print("GPIO 36 初始 ADC 值: ");
  Serial.println(initialValue);
  isCloudMode = (initialValue > 2000); // ADC 大於 2000 視為開關打開（狀態1）

  pinMode(localAuthPin, INPUT_PULLUP);    // 地端權限控制
  pinMode(localControlPin, INPUT_PULLUP); // 地端裝置控制
  pinMode(relayPin, OUTPUT);              // 繼電器控制
  digitalWrite(relayPin, LOW);            // 確保初始狀態為關閉
  Serial.println("GPIO 15 (繼電器) 初始化為 LOW");

  // 初始狀態設為關閉
  Serial.println("IO腳位初始化完成");

  // 檢查上雲開關狀態
  Serial.print("上雲開關狀態: ");
  Serial.println(isCloudMode ? "啟用" : "禁用");

  if (isCloudMode)
  {
    // 雲端模式：連接WiFi並初始化
    Serial.print("雲端模式：連接 WiFi...");
    WiFi.begin(ssid, password);

    int wifiTimeout = 0;
    while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20)
    {
      delay(500);
      Serial.print(".");
      wifiTimeout++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nWiFi 已連線");
      Serial.print("IP位址: ");
      Serial.println(WiFi.localIP());

      // 設定 SSL 客戶端
      client.setInsecure();     // 跳過 HTTPS 驗證（僅限內部使用）
    }
    else
    {
      Serial.println("\nWiFi 連接失敗，切換到地端模式");
      isCloudMode = false;
    }
  }
  else
  {
    Serial.println("地端模式：跳過網路連接");
  }

  Serial.println("系統初始化完成");
}

// 初始化系統設定
bool initializeSystem()
{
  if (isInitialized)
    return true;

  Serial.println("開始初始化系統設定...");

  if (isCloudMode)
  {
    // 雲端模式：從API獲取設定
    Serial.println("嘗試從雲端獲取設定...");
    https.begin(client, SHADOW_GET_DESIRED_URL);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Cookie", String("token=") + token);

    int httpCode = https.GET();
    if (httpCode == 200)
    {
      String payload = https.getString();
      StaticJsonDocument<512> doc;
      DeserializationError err = deserializeJson(doc, payload);

      if (!err && doc.size() > 0)
      {
        Serial.println("📥 取得雲端初始設定:");
        serializeJsonPretty(doc, Serial);
        Serial.println();

        // 讀取初始設定
        if (doc.containsKey("state") && doc["state"].containsKey("desired"))
        {
          JsonObject desired = doc["state"]["desired"];
          if (desired.containsKey("permission"))
          {
            cloudPermission = desired["permission"];
            Serial.print("雲給地權限初始狀態: ");
            Serial.println(cloudPermission);
          }

          if (desired.containsKey("status"))
          {
            deviceStatus = desired["status"];
            Serial.print("裝置初始狀態: ");
            Serial.println(deviceStatus ? "開啟" : "關閉");
            digitalWrite(relayPin, deviceStatus ? HIGH : LOW);
          }
        }
      }
      else
      {
        Serial.println("JSON解析失敗");
      }
    }
    else
    {
      Serial.print("獲取設定失敗，HTTP Code: ");
      Serial.println(httpCode);
    }
    https.end();
  }
  else
  {
    // 地端模式：直接讀取開關狀態
    Serial.println("地端模式：讀取本地開關狀態");
    localAuthEnabled = digitalRead(localAuthPin);
    bool localControl = digitalRead(localControlPin);

    if (localAuthEnabled)
    {
      deviceStatus = localControl;
      digitalWrite(relayPin, deviceStatus ? HIGH : LOW);
      Serial.print("地端控制狀態: ");
      Serial.println(deviceStatus ? "開啟" : "關閉");
    }
  }

  isInitialized = true;
  Serial.println("系統設定初始化完成");
  return true;
}

void reportState()
{
  if (!isCloudMode)
    return; // 地端模式不進行狀態回報

  Serial.println("回報狀態到雲端...");
  https.begin(client, SHADOW_UPDATE_URL);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("Cookie", String("token=") + token);

  // 直接讀取腳位狀態
  int permission = digitalRead(localAuthPin) ? 1 : 0;
  String jsonPayload = "{\"type\":\"reported\",\"data\":{\"status\":" + String(deviceStatus ? "1" : "0") + ",\"permission\":" + String(permission) + ",\"localper\":" + String(permission) + "}}";

  int httpCode = https.POST(jsonPayload);
  if (httpCode == 200)
  {
    Serial.println("回報狀態成功！");
  }
  else
  {
    Serial.print("回報失敗，HTTP Code: ");
    Serial.println(httpCode);
  }
  https.end();
}

void updateDeviceStatus(bool newStatus)
{
  if (deviceStatus != newStatus)
  {
    deviceStatus = newStatus;
    digitalWrite(relayPin, deviceStatus ? HIGH : LOW);
    Serial.print("繼電器狀態更新為：");
    Serial.println(deviceStatus ? "HIGH (開啟)" : "LOW (關閉)");

    // 驗證繼電器狀態
    delay(100); // 等待繼電器穩定
    int relayState = digitalRead(relayPin);
    Serial.print("驗證繼電器狀態：");
    Serial.println(relayState == HIGH ? "HIGH" : "LOW");
  }
}

// 在地端模式下，如果地端權限開啟，則更新 desired 狀態
void updateDesiredState(bool newStatus)
{
  if (!isCloudMode)
    return;

  Serial.println("更新雲端 desired 狀態...");
  https.begin(client, SHADOW_UPDATE_URL);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("Cookie", String("token=") + token);

  String jsonPayload = "{\"type\":\"desired\",\"data\":{\"status\":" + String(newStatus ? "1" : "0") + "}}";

  int httpCode = https.POST(jsonPayload);
  if (httpCode == 200)
  {
    Serial.println("更新 desired 狀態成功！");
  }
  else
  {
    Serial.print("更新 desired 狀態失敗，HTTP Code: ");
    Serial.println(httpCode);
  }
  https.end();
}

void loop()
{
  // 確保系統已初始化
  if (!isInitialized)
  {
    if (!initializeSystem())
    {
      Serial.println("初始化失敗，等待5秒後重試");
      delay(5000);
      return;
    }
  }

  // 讀取開關狀態並進行防彈跳處理
  int adcValue = analogRead(cloudSwitchPin);
  bool cloudSwitchHigh = (adcValue > 2000); // 開關打開時為1

  // 如果狀態改變，重置防彈跳計時器
  if (cloudSwitchHigh != lastCloudSwitchValue)
  {
    lastDebounceTime = millis();
  }

  // 如果狀態穩定超過防彈跳時間，更新狀態
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    cloudEnabled = cloudSwitchHigh; // 開關打開為啟用
    isCloudMode = cloudSwitchHigh;
  }

  lastCloudSwitchValue = cloudSwitchHigh;

  // 檢查雲端模式是否改變
  if (isCloudMode != wasCloudMode)
  {
    if (isCloudMode)
    {
      // 從地端模式切換到雲端模式
      Serial.println("切換到雲端模式，讀取雲端狀態...");
      // 強制重啟 WiFi
      WiFi.disconnect(true); // 斷線並清除記憶
      delay(100);
      WiFi.begin(ssid, password);
      int wifiTimeout = 0;
      while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20)
      {
        delay(500);
        Serial.print(".");
        wifiTimeout++;
      }
      if (WiFi.status() == WL_CONNECTED)
      {
        Serial.println("\nWiFi 已連線");
        Serial.print("IP位址: ");
        Serial.println(WiFi.localIP());
        client.setInsecure();
      }
      else
      {
        Serial.println("\nWiFi 連接失敗，切換到地端模式");
        isCloudMode = false;
      }
      // 讀取雲端狀態
      https.begin(client, SHADOW_GET_DESIRED_URL);
      https.addHeader("Content-Type", "application/json");
      https.addHeader("Cookie", String("token=") + token);
      int httpCode = https.GET();
      if (httpCode == 200)
      {
        String payload = https.getString();
        StaticJsonDocument<512> doc;
        DeserializationError err = deserializeJson(doc, payload);
        if (!err && doc.size() > 0)
        {
          Serial.println("📥 取得雲端初始設定:");
          serializeJsonPretty(doc, Serial);
          Serial.println();
          if (doc.containsKey("status"))
          {
            deviceStatus = doc["status"];
            digitalWrite(relayPin, deviceStatus ? HIGH : LOW);
          }
        }
      }
      https.end();
    }
    else
    {
      // 從雲端模式切換到地端模式
      Serial.println("切換到地端模式");
      // 讀取地端開關狀態
      localAuthEnabled = digitalRead(localAuthPin);
      bool localControl = digitalRead(localControlPin);
      if (localAuthEnabled)
      {
        deviceStatus = localControl;
        digitalWrite(relayPin, deviceStatus ? HIGH : LOW);
      }
    }
    wasCloudMode = isCloudMode;
  }

  // 根據當前模式處理控制邏輯
  if (isCloudMode)
  {
    // 雲端模式：檢查雲端指令
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("WiFi 斷線，重連中...");
      WiFi.reconnect();
      delay(2000);
      return;
    }

    // 取得雲端控制指令
    https.begin(client, SHADOW_GET_DELTA_URL);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Cookie", String("token=") + token);
    int httpCode = https.GET();

    if (httpCode == 200)
    {
      String payload = https.getString();
      StaticJsonDocument<512> doc;
      DeserializationError err = deserializeJson(doc, payload);

      if (!err && doc.size() > 0)
      {
        bool stateChanged = false;
        if (doc.containsKey("permission"))
        {
          cloudPermission = doc["permission"];
          Serial.print("雲給地權限: ");
          Serial.println(cloudPermission);
          stateChanged = true;
        }
        if (doc.containsKey("status"))
        {
          bool cloudControl = doc["status"];
          // 只有在沒有地端控制時才接受雲端控制
          if (!localAuthEnabled)
          {
            updateDeviceStatus(cloudControl);
            stateChanged = true;
          }
        }

        // 如果有任何狀態改變，立即回報地端狀態
        if (stateChanged)
        {
          reportState();
        }
      }
    }
    https.end();

    // 地端權限開啟時，優先處理地端控制
    localAuthEnabled = digitalRead(localAuthPin);
    bool localControl = digitalRead(localControlPin);
    if (localAuthEnabled)
    {
      updateDeviceStatus(localControl);
      reportState();
    }
  }
  else
  {
    // 地端模式：只有 localAuthPin 為接通時才允許地端操作
    cloudPermission = digitalRead(localAuthPin) ? 1 : 0;
    if (digitalRead(localAuthPin))
    {
      bool newStatus = digitalRead(localControlPin);
      updateDeviceStatus(newStatus);
      updateDesiredState(newStatus); // 更新雲端 desired 狀態
    }
  }

  // 顯示系統狀態
  Serial.print("系統狀態 - 雲端模式: ");
  Serial.print(isCloudMode);
  Serial.print(", 地端權限: ");
  Serial.print(localAuthEnabled);
  Serial.print(", 裝置狀態: ");
  Serial.println(deviceStatus);

  // 顯示雲給地權限或地端權限狀態
  if (isCloudMode)
  {
    Serial.print("雲給地權限: ");
    Serial.println(cloudPermission);
  }
  else
  {
    Serial.print("地端權限: ");
    Serial.println(digitalRead(localAuthPin) ? 1 : 0);
  }

  delay(1000); // 每秒檢查一次狀態
}
