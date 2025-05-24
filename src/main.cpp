/**
 * @file main.cpp
 * @brief IoT Device Control System with Cloud and Local Control
 *
 * This program implements a control system that can operate in both cloud and local modes.
 * It supports dual control mechanisms with priority handling and state synchronization.
 */

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <time.h>

/* Network Configuration */
const char *SSID = "iOT";
const char *PASSWORD = "Albert0801";

/* API Endpoints */
const char *SERVER_TIME_URL = "https://unix.jw-albert.dev/authapi/servertime";
const char *SHADOW_GET_DELTA_URL = "https://unix.jw-albert.dev/api/shadow/get?type=delta";
const char *SHADOW_GET_DESIRED_URL = "https://unix.jw-albert.dev/api/shadow/get?type=desired";
const char *SHADOW_UPDATE_URL = "https://unix.jw-albert.dev/api/shadow/update";

/* Pin Definitions */
const int CLOUD_SWITCH_PIN = 36;    // Cloud mode switch (Input)
const int LOCAL_AUTH_PIN = 39;      // Local permission control (Input)
const int LOCAL_CONTROL_PIN = 34;   // Local device control (Input)
const int RELAY_PIN = 2;            // Relay control (Output)

/* Authentication */
const char *TOKEN = "device-token-789";

/* Global Objects */
WiFiClientSecure client;
HTTPClient https;

/* System State Variables */
bool cloudEnabled = false;             // Cloud control status
bool localAuthEnabled = false;         // Local permission status
bool deviceStatus = false;             // Device status
bool isInitialized = false;            // System initialization status
bool isCloudMode = false;              // Current operation mode
bool wasCloudMode = false;             // Previous operation mode
unsigned long currentUnixTime = 0;     // Current server time

/* Debug Variables */
unsigned long lastResetTime = 0;
int resetCount = 0;

/* Switch State Variables */
bool lastCloudSwitchValue = false;
unsigned long lastDebounceTime = 0;
const int DEBOUNCE_DELAY = 50; // Debounce delay in milliseconds

/* Control Variables */
int cloudPermission = 0; // Cloud to local permission (0: denied, 1: allowed)

/**
 * @brief Prints system reset information
 */
void printResetReason()
{
    Serial.println("\n=== System Reset Information ===");
    Serial.print("Reset Count: ");
    Serial.println(resetCount);
    Serial.print("Time Since Last Reset: ");
    Serial.print((millis() - lastResetTime) / 1000.0);
    Serial.println(" seconds");
    Serial.println("==============================\n");
}

/**
 * @brief Updates the server time from the API
 * @return true if successful, false otherwise
 */
bool updateServerTime()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected, cannot get time");
        return false;
    }

    https.begin(client, SERVER_TIME_URL);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Cookie", String("token=") + TOKEN);

    int httpCode = https.GET();
    if (httpCode == 200)
    {
        String payload = https.getString();
        StaticJsonDocument<200> doc;
        DeserializationError err = deserializeJson(doc, payload);

        if (!err && doc.containsKey("unixtime"))
        {
            currentUnixTime = doc["unixtime"];
            Serial.print("Time updated: ");
            Serial.println(currentUnixTime);
            https.end();
            return true;
        }
    }

    Serial.print("Failed to get time, HTTP Code: ");
    Serial.println(httpCode);
    https.end();
    return false;
}

/**
 * @brief Initializes the system
 * @return true if successful, false otherwise
 */
bool initializeSystem()
{
    if (isInitialized)
        return true;

    Serial.println("Initializing system...");

    if (isCloudMode)
    {
        Serial.println("Fetching cloud configuration...");
        https.begin(client, SHADOW_GET_DESIRED_URL);
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Cookie", String("token=") + TOKEN);

        int httpCode = https.GET();
        if (httpCode == 200)
        {
            String payload = https.getString();
            StaticJsonDocument<512> doc;
            DeserializationError err = deserializeJson(doc, payload);

            if (!err && doc.size() > 0)
            {
                Serial.println("📥 Cloud configuration received:");
                serializeJsonPretty(doc, Serial);
                Serial.println();

                if (doc.containsKey("state") && doc["state"].containsKey("desired"))
                {
                    JsonObject desired = doc["state"]["desired"];
                    if (desired.containsKey("permission"))
                    {
                        cloudPermission = desired["permission"];
                        Serial.print("Cloud permission: ");
                        Serial.println(cloudPermission);
                    }

                    if (desired.containsKey("status"))
                    {
                        deviceStatus = desired["status"];
                        Serial.print("Device status: ");
                        Serial.println(deviceStatus ? "ON" : "OFF");
                        digitalWrite(RELAY_PIN, deviceStatus ? HIGH : LOW);
                    }
                }
            }
        }
        https.end();
    }
    else
    {
        Serial.println("Local mode: Reading local switch states");
        localAuthEnabled = digitalRead(LOCAL_AUTH_PIN);
        bool localControl = digitalRead(LOCAL_CONTROL_PIN);

        if (localAuthEnabled)
        {
            deviceStatus = localControl;
            digitalWrite(RELAY_PIN, deviceStatus ? HIGH : LOW);
            Serial.print("Local control status: ");
            Serial.println(deviceStatus ? "ON" : "OFF");
        }
    }

    isInitialized = true;
    Serial.println("System initialization complete");
    return true;
}

/**
 * @brief Reports current state to the cloud
 */
void reportState()
{
    if (!isCloudMode)
        return;

    Serial.println("Reporting state to cloud...");

    if (!updateServerTime())
    {
        Serial.println("Failed to get time, cannot report state");
        return;
    }

    https.begin(client, SHADOW_UPDATE_URL);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Cookie", String("token=") + TOKEN);

    int permission = digitalRead(LOCAL_AUTH_PIN) ? 1 : 0;
    String jsonPayload = "{\"type\":\"reported\",\"data\":{\"status\":" + String(deviceStatus ? "1" : "0") +
                                             ",\"permission\":" + String(permission) +
                                             ",\"localper\":" + String(permission) +
                                             ",\"localtime\":" + String(currentUnixTime) + "}}";

    int httpCode = https.POST(jsonPayload);
    if (httpCode == 200)
    {
        Serial.println("State report successful");
    }
    else
    {
        Serial.print("State report failed, HTTP Code: ");
        Serial.println(httpCode);
    }
    https.end();
}

/**
 * @brief Updates device status and controls relay
 * @param newStatus New device status
 */
void updateDeviceStatus(bool newStatus)
{
    if (deviceStatus != newStatus)
    {
        deviceStatus = newStatus;
        digitalWrite(RELAY_PIN, deviceStatus ? HIGH : LOW);
        Serial.print("Relay status updated to: ");
        Serial.println(deviceStatus ? "HIGH (ON)" : "LOW (OFF)");

        delay(100);
        int relayState = digitalRead(RELAY_PIN);
        Serial.print("Relay state verification: ");
        Serial.println(relayState == HIGH ? "HIGH" : "LOW");
    }
}

/**
 * @brief Updates desired state in cloud
 * @param newStatus New desired status
 */
void updateDesiredState(bool newStatus)
{
    if (!isCloudMode)
        return;

    Serial.println("Updating cloud desired state...");
    https.begin(client, SHADOW_UPDATE_URL);
    https.addHeader("Content-Type", "application/json");
    https.addHeader("Cookie", String("token=") + TOKEN);

    String jsonPayload = "{\"type\":\"desired\",\"data\":{\"status\":" + String(newStatus ? "1" : "0") + "}}";

    int httpCode = https.POST(jsonPayload);
    if (httpCode == 200)
    {
        Serial.println("Desired state update successful");
    }
    else
    {
        Serial.print("Desired state update failed, HTTP Code: ");
        Serial.println(httpCode);
    }
    https.end();
}

/**
 * @brief Arduino setup function
 */
void setup()
{
    Serial.begin(115200);
    delay(1000);

    resetCount++;
    if (lastResetTime == 0)
    {
        lastResetTime = millis();
    }
    printResetReason();

    Serial.println("Starting system initialization...");
    Serial.println("Initializing I/O pins...");

    delay(100);
    int initialValue = analogRead(CLOUD_SWITCH_PIN);
    Serial.print("GPIO 36 initial ADC value: ");
    Serial.println(initialValue);
    isCloudMode = (initialValue > 2000);

    pinMode(LOCAL_AUTH_PIN, INPUT_PULLUP);
    pinMode(LOCAL_CONTROL_PIN, INPUT_PULLUP);
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("GPIO 2 (Relay) initialized to LOW");

    Serial.println("I/O pin initialization complete");
    Serial.print("Cloud mode: ");
    Serial.println(isCloudMode ? "Enabled" : "Disabled");

    if (isCloudMode)
    {
        Serial.print("Connecting to WiFi...");
        WiFi.begin(SSID, PASSWORD);

        int wifiTimeout = 0;
        while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20)
        {
            delay(500);
            Serial.print(".");
            wifiTimeout++;
        }

        if (WiFi.status() == WL_CONNECTED)
        {
            Serial.println("\nWiFi connected");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());

            client.setInsecure();
            updateServerTime();
        }
        else
        {
            Serial.println("\nWiFi connection failed, switching to local mode");
            isCloudMode = false;
        }
    }
    else
    {
        Serial.println("Local mode: Skipping network connection");
    }

    Serial.println("System initialization complete");
}

/**
 * @brief Arduino main loop function
 */
void loop()
{
    if (!isInitialized)
    {
        if (!initializeSystem())
        {
            Serial.println("Initialization failed, retrying in 5 seconds");
            delay(5000);
            return;
        }
    }

    int adcValue = analogRead(CLOUD_SWITCH_PIN);
    bool cloudSwitchHigh = (adcValue > 2000);

    if (cloudSwitchHigh != lastCloudSwitchValue)
    {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY)
    {
        cloudEnabled = cloudSwitchHigh;
        isCloudMode = cloudSwitchHigh;
    }

    lastCloudSwitchValue = cloudSwitchHigh;

    if (isCloudMode != wasCloudMode)
    {
        if (isCloudMode)
        {
            Serial.println("Switching to cloud mode, reading cloud state...");
            WiFi.disconnect(true);
            delay(100);
            WiFi.begin(SSID, PASSWORD);
            int wifiTimeout = 0;
            while (WiFi.status() != WL_CONNECTED && wifiTimeout < 20)
            {
                delay(500);
                Serial.print(".");
                wifiTimeout++;
            }
            if (WiFi.status() == WL_CONNECTED)
            {
                Serial.println("\nWiFi connected");
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP());
                client.setInsecure();
            }
            else
            {
                Serial.println("\nWiFi connection failed, switching to local mode");
                isCloudMode = false;
            }

            https.begin(client, SHADOW_GET_DESIRED_URL);
            https.addHeader("Content-Type", "application/json");
            https.addHeader("Cookie", String("token=") + TOKEN);
            int httpCode = https.GET();
            if (httpCode == 200)
            {
                String payload = https.getString();
                StaticJsonDocument<512> doc;
                DeserializationError err = deserializeJson(doc, payload);
                if (!err && doc.size() > 0)
                {
                    Serial.println("📥 Cloud configuration received:");
                    serializeJsonPretty(doc, Serial);
                    Serial.println();
                    if (doc.containsKey("state") && doc["state"].containsKey("desired"))
                    {
                        JsonObject desired = doc["state"]["desired"];
                        if (desired.containsKey("status"))
                        {
                            deviceStatus = desired["status"];
                            digitalWrite(RELAY_PIN, deviceStatus ? HIGH : LOW);
                        }
                    }
                }
            }
            https.end();
        }
        else
        {
            Serial.println("Switching to local mode");
            localAuthEnabled = digitalRead(LOCAL_AUTH_PIN);
            bool localControl = digitalRead(LOCAL_CONTROL_PIN);
            if (localAuthEnabled)
            {
                deviceStatus = localControl;
                digitalWrite(RELAY_PIN, deviceStatus ? HIGH : LOW);
            }
        }
        wasCloudMode = isCloudMode;
    }

    if (isCloudMode)
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("WiFi disconnected, reconnecting...");
            WiFi.reconnect();
            delay(2000);
            return;
        }

        https.begin(client, SHADOW_GET_DELTA_URL);
        https.addHeader("Content-Type", "application/json");
        https.addHeader("Cookie", String("token=") + TOKEN);
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
                    Serial.print("Cloud permission: ");
                    Serial.println(cloudPermission);
                    stateChanged = true;
                }
                if (doc.containsKey("status"))
                {
                    bool cloudControl = doc["status"];
                    if (!localAuthEnabled)
                    {
                        updateDeviceStatus(cloudControl);
                        stateChanged = true;
                    }
                }

                if (stateChanged)
                {
                    reportState();
                }
            }
        }
        https.end();

        localAuthEnabled = digitalRead(LOCAL_AUTH_PIN);
        bool localControl = digitalRead(LOCAL_CONTROL_PIN);
        if (localAuthEnabled)
        {
            updateDeviceStatus(localControl);
            reportState();
        }
    }
    else
    {
        cloudPermission = digitalRead(LOCAL_AUTH_PIN) ? 1 : 0;
        if (digitalRead(LOCAL_AUTH_PIN))
        {
            bool newStatus = digitalRead(LOCAL_CONTROL_PIN);
            updateDeviceStatus(newStatus);
            updateDesiredState(newStatus);
        }
    }

    Serial.print("System status - Cloud mode: ");
    Serial.print(isCloudMode);
    Serial.print(", Local permission: ");
    Serial.print(localAuthEnabled);
    Serial.print(", Device status: ");
    Serial.println(deviceStatus);

    if (isCloudMode)
    {
        Serial.print("Cloud permission: ");
        Serial.println(cloudPermission);
    }
    else
    {
        Serial.print("Local permission: ");
        Serial.println(digitalRead(LOCAL_AUTH_PIN) ? 1 : 0);
    }

    delay(1500);
}
