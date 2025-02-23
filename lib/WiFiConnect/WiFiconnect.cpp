#include "WiFiConnect.h"
#include <Arduino.h>

WiFiConnect::WiFiConnect(const char* ssid, const char* password) {
    this->ssid = ssid;
    this->password = password;
}

void WiFiConnect::connect() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");

    int retries = 30;
    while (WiFi.status() != WL_CONNECTED && retries > 0) {
        delay(500);
        Serial.print(".");
        retries--;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nFailed to connect to WiFi.");
    }
}

bool WiFiConnect::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WiFiConnect::loop() {
    if (!isConnected() && wasConnected) {  
      Serial.println("WiFi disconnected. Reconnecting...");
      connect();  
      delay(2000);  
      wasConnected = isConnected();  
    }
}