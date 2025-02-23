#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

#include <WiFi.h>

class WiFiConnect {
public:
    WiFiConnect(const char* ssid, const char* password);
    void connect();
    bool isConnected();
    void loop();

private:
    const char* ssid;
    const char* password;
    bool wasConnected = false;
};

#endif
