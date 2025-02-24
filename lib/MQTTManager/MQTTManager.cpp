#include "MQTTManager.h"

MQTTManager::MQTTManager(const char* mqttBroker, const char* mqttTopic, const char* mqttClientID, int mqttPort)
    : mqttBroker(mqttBroker), mqttTopic(mqttTopic), mqttClientID(mqttClientID), mqttPort(mqttPort), lastMillis(0) {
    client.begin(mqttBroker, mqttPort, net);
}

void MQTTManager::begin() {
    client.onMessage(messageCallback);
    connect();
}

void MQTTManager::loop() {
    client.loop();
    delay(10);  // <- fixes some issues with WiFi stability

    if (!client.connected()) {
        Serial.println("Lost connection, attempting to reconnect...");
        connect();
    }   
}

void MQTTManager::connect() {
    Serial.print("Connecting to MQTT...");
    while (!client.connect(mqttClientID)) {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("\nConnected!");
    client.subscribe(mqttTopic);
}

bool MQTTManager::isConnected() {
    return client.connected();
}

void MQTTManager::publish(const char* message) {
    client.publish(mqttTopic, message);
}

void MQTTManager::setMessageCallback(void (*callback)(String&, String&)) {
    messageCallback = callback;
}
