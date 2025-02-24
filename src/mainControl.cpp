#include <Arduino.h>
#include "MagneticEn.h"
#include "TCA9548A.h"
#include "WiFiConnect.h"
#include "MQTTManager.h"

#define WIFI_SSID "iPhone T"
#define WIFI_PASSWORD "12345678"

WiFiConnect wifi(WIFI_SSID, WIFI_PASSWORD);
bool wasConnected = true;

const char mqtt_broker[]="test.mosquitto.org";
const char mqtt_topic[] = "ProjectGroup1.2/command";
const char mqtt_client_id[] = "Manlnwza";
int MQTT_PORT = 1883;

MQTTManager mqtt(mqtt_broker, mqtt_topic, mqtt_client_id, MQTT_PORT);

MagneticEn magEn_base;
MagneticEn magEn_joint1;
MagneticEn magEn_joint2;
MagneticEn magEn_joint3;
MagneticEn magEn_gripper;

TCA9548A tca;

void messageReceived(String &topic, String &payload) {
    Serial.println("Incoming: " + topic + " - " + payload);
}

void setup() {
    Serial.begin(115200);
    delay(2000);
    wifi.connect();
    wasConnected = wifi.isConnected();

    mqtt.setMessageCallback(messageReceived);
    mqtt.begin();

    Wire.begin();
}

void loop() {
    wifi.loop();
    mqtt.loop();
    Serial.println("1");

    tca.selectBus(0);
    int angle_base = magEn_base.getAngle();

    tca.selectBus(1);
    int angle_joint1= magEn_joint1.getAngle();

    tca.selectBus(2);
    int angle_joint2 = magEn_joint2.getAngle();

    tca.selectBus(3);
    int angle_joint3 = magEn_joint3.getAngle();

    tca.selectBus(4);
    int angle_gripper = magEn_gripper.getAngle();


    String message = "Angle_base: " + String(angle_base) + "\nAngle_joint1: " + String(angle_joint1) + "\nAngle_joint2: " + String(angle_joint2) +"\nAngle_joint3: " + String(angle_joint3) + "\nAngle_gripper: " + String(angle_gripper);
    mqtt.publish(message.c_str());
    Serial.println("Published: " + message);
}