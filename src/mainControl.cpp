#include <Arduino.h>
#include <ESP32Servo.h>
#include "MagneticEn.h"
#include "PIDControl.h"
// #include "WiFiConnect.h"
// #include "MQTTManager.h"
#include<WiFi.h>
#include <MQTT.h>
#include "TCA9548A.h"

#define WIFI_SSID "iPhone T"
#define WIFI_PASSWORD "12345678"

// WiFiConnect wifi(WIFI_SSID, WIFI_PASSWORD);
WiFiClient net;
MQTTClient mqtt;
bool wasConnected = true;

const char mqtt_broker[] = "test.mosquitto.org";
const char mqtt_client_id[] = "Mannaja";
int MQTT_PORT = 1883;

const char mqtt_topic[]="ProjectGroup1.2/#";


MagneticEn magEn_base;
MagneticEn magEn_joint1;
MagneticEn magEn_joint2;
MagneticEn magEn_joint3;
MagneticEn magEn_gripper;


TCA9548A tca;


void connect() {
    Serial.print("checking wifi...");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
  
    Serial.print("\nconnecting...");
    while (!mqtt.connect(mqtt_client_id)) {  
      Serial.print(".");
      delay(1000);
    }
  
    Serial.println("\nconnected!");
  
    mqtt.subscribe(mqtt_topic);
    // client.unsubscribe("/hello");
}

float normaliseDegree(float degree){
  return degree > 360 ? degree - 360 : degree;
}

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Wire.begin();
    Wire.begin(21, 22);
    // Serial.println("Scanning I2C devices...");
    // for (byte address = 1; address < 127; address++) {
    //     Wire.beginTransmission(address);
    //     if (Wire.endTransmission() == 0) {
    //         Serial.print("Found I2C device at 0x");
    //         Serial.println(address, HEX);
    //     }
    // }
    // Serial.println("Scan complete.");
    // delay(3000);

    // wasConnected = wifi.isConnected();

    // mqtt.setMessageCallback(messageReceived);
    mqtt.begin(mqtt_broker, MQTT_PORT, net);
    mqtt.subscribe(mqtt_topic);
}

void loop() {
    // wifi.loop();
    mqtt.loop();  
    delay(10);

    if (!mqtt.connected()) {
        connect();
    }

    tca.selectBus(7);
    delay(10);
    float angle_base = magEn_base.getAngle();
    String message_base = String(angle_base);
    mqtt.publish("ProjectGroup1.2/command/angle_base", message_base);
    Serial.println("Published: " + message_base);

    tca.selectBus(5);
    delay(10);
    float angle_joint1 = magEn_joint1.getAngle();
    String message_joint1 = String(normaliseDegree(angle_joint1 - 49));
    mqtt.publish("ProjectGroup1.2/command/angle_joint1", message_joint1);
    Serial.println("Published: " + message_joint1);
    
    tca.selectBus(4);
    delay(10);
    float angle_joint2 = magEn_joint2.getAngle();
    String message_joint2 = String(normaliseDegree(angle_joint2 + 11));
    mqtt.publish("ProjectGroup1.2/command/angle_joint2", message_joint2);
    Serial.println("Published: " + message_joint2);
    
    tca.selectBus(3);
    delay(10);
    float angle_joint3 = magEn_joint3.getAngle();
    String message_joint3 = String(normaliseDegree(angle_joint3 + 125));
    mqtt.publish("ProjectGroup1.2/command/angle_joint3", message_joint3);
    Serial.println("Published: " + message_joint3);
    
    tca.selectBus(6);
    delay(10);
    float angle_gripper = magEn_gripper.getAngle();
    String message_gripper = String(angle_gripper);
    mqtt.publish("ProjectGroup1.2/command/angle_gripper", message_gripper);
    Serial.println("Published: " + message_gripper);
}