#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi 設定
const char* ssid = "Albert";
const char* password = "Albert0801";

// Shadow API base
const char* SHADOW_GET_URL = "https://unix.jw-albert.dev/api/shadow/get?type=delta";
const char* SHADOW_UPDATE_URL = "https://unix.jw-albert.dev/api/shadow/update";

// 模擬設備 GPIO（使用內建 LED）
const int ledPin = 2;

// Token 用作 cookie 模擬登入
const char* token = "device-token-789";

WiFiClientSecure client;
HTTPClient https;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.print("連接 WiFi：");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 已連線");
  Serial.println(WiFi.localIP());

  client.setInsecure();  // 跳過 HTTPS 驗證（僅限內部使用）
}

void reportState(int status) {
  https.begin(client, SHADOW_UPDATE_URL);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("Cookie", String("token=") + token);

  String jsonPayload = "{\"type\":\"reported\",\"data\":{\"status\":" + String(status) + "}}";

  int httpCode = https.POST(jsonPayload);
  if (httpCode == 200) {
    Serial.println("回報 reported 成功！");
  } else {
    Serial.print("回報失敗，HTTP Code: ");
    Serial.println(httpCode);
  }
  https.end();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi 斷線，重連中...");
    WiFi.reconnect();
    delay(2000);
    return;
  }

  // === Step 1: 取得 delta ===
  https.begin(client, SHADOW_GET_URL);
  https.addHeader("Content-Type", "application/json");
  https.addHeader("Cookie", String("token=") + token);
  int httpCode = https.GET();

  if (httpCode == 200) {
    String payload = https.getString();
    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (!err && doc.size() > 0) {
      Serial.println("📥 取得 delta:");
      serializeJsonPretty(doc, Serial);
      Serial.println();

      if (doc.containsKey("status")) {
        int status = doc["status"];
        Serial.print("控制設備狀態：");
        Serial.println(status ? "開啟" : "關閉");
        digitalWrite(ledPin, status ? HIGH : LOW);

        delay(1000);  // 模擬處理時間

        // === Step 2: 回報 reported 狀態 ===
        reportState(status);
      }
    } else {
      Serial.println("沒有有效 delta 或 JSON 解析失敗");
    }
  } else {
    Serial.print("取得 delta 失敗，HTTP Code: ");
    Serial.println(httpCode);
  }
  https.end();

  delay(2000);  // 每 2 秒輪詢一次
}
