#include <Arduino.h>
#include <ESP32Servo.h>
#include "MagneticEn.h"
#include "PIDControl.h"
#include "WiFiConnect.h"
#include "MQTTManager.h"

#define WIFI_SSID "iPhone T"
#define WIFI_PASSWORD "12345678"

WiFiConnect wifi(WIFI_SSID, WIFI_PASSWORD);
bool wasConnected = true;

const char mqtt_broker[] = "test.mosquitto.org";
const char mqtt_topic[] = "group/command";
const char mqtt_client_id[] = "arduino_group_x";
int MQTT_PORT = 1883;

MQTTManager mqtt(mqtt_broker, mqtt_topic, mqtt_client_id, MQTT_PORT);

PIDControl pid(1.0, 0.1, 0.1); // Kp Ki Kd

Servo servo_joint1, servo_joint2, servo_joint3, servo_gripper;

void setup() {
    wifi.connect();
    wasConnected = wifi.isConnected();

    mqtt.setMessageCallback(messageReceived);
    mqtt.begin();

    Wire.begin();
    pid.begin(12);
    servo_joint1.attach(13);
    servo_joint2.attach(14);
    servo_joint3.attach(15);
    servo_gripper.attach(16);
}

void loop() {
    wifi.loop();
    mqtt.loop();
}

void messageReceived(String &topic, String &payload) {
    Serial.println("Incoming: " + topic + " - " + payload);

    int angle_base = payload.substring(payload.indexOf("Angle_base:") + 11, payload.indexOf("\n", payload.indexOf("Angle_base:"))).toInt();
    int angle_joint1 = payload.substring(payload.indexOf("Angle_joint1:") + 13, payload.indexOf("\n", payload.indexOf("Angle_joint1:"))).toInt();
    int angle_joint2 = payload.substring(payload.indexOf("Angle_joint2:") + 13, payload.indexOf("\n", payload.indexOf("Angle_joint2:"))).toInt();
    int angle_joint3 = payload.substring(payload.indexOf("Angle_joint3:") + 13, payload.indexOf("\n", payload.indexOf("Angle_joint3:"))).toInt();
    int angle_gripper = payload.substring(payload.indexOf("Angle_gripper:") + 15, payload.indexOf("\n", payload.indexOf("Angle_gripper:"))).toInt();

    pid.setTargetAngle(angle_base);
    pid.update();

    angle_joint1 = (angle_joint1 > 270) ? 270 : (angle_joint1 < 0 ? 0 : angle_joint1);
    angle_joint2 = (angle_joint2 > 270) ? 270 : (angle_joint2 < 0 ? 0 : angle_joint2);
    angle_joint3 = (angle_joint3 > 270) ? 270 : (angle_joint3 < 0 ? 0 : angle_joint3);
    angle_gripper = (angle_gripper > 270) ? 270 : (angle_gripper < 0 ? 0 : angle_gripper);

    servo_joint1.write(angle_joint1);
    servo_joint2.write(angle_joint2);
    servo_joint3.write(angle_joint3);
    servo_gripper.write(angle_gripper);
}