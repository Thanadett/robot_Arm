#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include <MQTT.h>
#include <WiFi.h>

class MQTTManager {
public:
    MQTTManager(const char* mqttBroker, const char* mqttTopic, const char* mqttClientID, int mqttPort);
    void begin();
    void loop();
    void connect();
    bool isConnected();
    void publish(const char* message);
    void setMessageCallback(void (*callback)(String&, String&));

private:
    const char* mqttBroker;
    const char* mqttTopic;
    const char* mqttClientID;
    int mqttPort;
    WiFiClient net;
    MQTTClient client;
    unsigned long lastMillis;
    void (*messageCallback)(String&, String&);
};

#endif