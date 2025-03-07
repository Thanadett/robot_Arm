#include <Arduino.h>
#include <ESP32Servo.h>
#include "MagneticEn.h"
#include "PIDControl.h"
// #include "WiFiConnect.h"
// #include "MQTTManager.h"
#include<WiFi.h>
#include <MQTT.h>

#define WIFI_SSID "iPhone T"
#define WIFI_PASSWORD "12345678"

// WiFiConnect wifi(WIFI_SSID, WIFI_PASSWORD);
WiFiClient net;
MQTTClient mqtt;
bool wasConnected = true;

const char mqtt_broker[] = "test.mosquitto.org";
const char mqtt_client_id[] = "Chaemnaja";
int MQTT_PORT = 1883;

const char mqtt_topic[]="ProjectGroup1.2/#";
String current_mode = "Manual";


// MQTTManager mqtt(mqtt_broker, mqtt_topic, mqtt_client_id, MQTT_PORT);

PIDControl pid(2.5, 0.000001, 0); // Kp Ki Kd

MagneticEn magEn_base;
Servo servo_joint1, servo_joint2, servo_joint3, servo_gripper;

float current_angle_base = 0;
float angle_base = 0;
float angle_joint1 = 0;
float angle_joint2 = 0;
float angle_joint3 = 0;
float angle_gripper = 0;

float current_angle_joint1 = 135, current_angle_joint2 = 135, current_angle_joint3 = 135, current_angle_gripper = 135;
float current_pwm_joint1 = 1500, current_pwm_joint2 = 1500, current_pwm_joint3 = 1500, current_pwm_gripper = 1500;

bool isMaunal = true;

void messageReceived(String &topic, String &payload);
void doSomething();
void resetGripper();

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

void setup() {
    Serial.begin(115200);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    // wasConnected = wifi.isConnected();

    // mqtt.setMessageCallback(messageReceived);
    mqtt.begin(mqtt_broker, MQTT_PORT, net);
    mqtt.subscribe(mqtt_topic);
    mqtt.onMessage(messageReceived);


    Wire.begin();
    pid.begin(16);
    servo_joint1.attach(17, 500, 2500);
    servo_joint2.attach(19, 500, 2500);
    servo_joint3.attach(4, 500, 2500);
    servo_gripper.attach(2, 500, 2500);

    resetGripper();
}

void loop() {
    mqtt.loop();  
    delay(10);

    if (!mqtt.connected()) {
        connect();
    }

    doSomething();
}

void resetGripper(){
    current_angle_joint1 = 135;
    current_angle_joint2 = 135;
    current_angle_joint3 = 135;
    current_angle_gripper = 135;
    current_pwm_joint1 = map(current_angle_joint1, 0, 270, 500, 2500);
    current_pwm_joint2 = map(current_angle_joint1, 0, 270, 500, 2500);
    current_pwm_joint3 = map(current_angle_joint1, 0, 270, 500, 2500);
    current_pwm_gripper = map(current_angle_joint1, 0, 270, 500, 2500);
    servo_joint1.writeMicroseconds(1500);
    delay(200);
    servo_joint2.writeMicroseconds(1500);
    delay(200);
    servo_joint3.writeMicroseconds(1500);
    delay(200);
    servo_gripper.writeMicroseconds(1500);
    delay(200);
}

void messageReceived(String &topic, String &payload) {
    // Serial.println("Incoming: " + topic + " - " + payload);
    
    // ProjectGroup1.2/command
    if (topic == "ProjectGroup1.2/command" && payload == "Auto"){
        current_mode = "Auto";
    }

    else if (topic == "ProjectGroup1.2/command" && payload == "Manual"){
        current_mode = "Manual";
        resetGripper();
    }

    if (current_mode == "Auto"){
        if (topic == "ProjectGroup1.2/command/angle_base") {
            angle_base = payload.toFloat();
        }
        else if (topic == "ProjectGroup1.2/command/angle_joint1") {
            angle_joint1 = payload.toFloat();
        }
        else if (topic == "ProjectGroup1.2/command/angle_joint2") {
            angle_joint2 = payload.toFloat();
        }
        else if (topic == "ProjectGroup1.2/command/angle_joint3") {
            angle_joint3 = payload.toFloat();
        }
        else if (topic == "ProjectGroup1.2/command/angle_gripper") {
            angle_gripper = payload.toFloat();
        }
    }

    if(current_mode == "Manual"){
        if (topic == "ProjectGroup1.2/command/option" && payload == "setZero"){
            resetGripper();
        }

        if (topic == "ProjectGroup1.2/command/angle_base") {
            if (payload == "up") {
                current_angle_base = constrain(current_angle_base + 3, 0, 360); // Increase by 1 degree
            }
            else if (payload == "down") {
                current_angle_base = constrain(current_angle_base - 3, 0, 360); // Decrease by 1 degree
            }
        } 
        else if (topic == "ProjectGroup1.2/command/angle_joint1") {
            if (payload == "up") {
                // Serial.println("Hello");
                current_angle_joint1 = constrain(current_angle_joint1 + 3, 45, 225);
            }
            else if (payload == "down") {
                current_angle_joint1 = constrain(current_angle_joint1 - 3, 45, 225);
            }
            current_pwm_joint1 = map(current_angle_joint1, 0, 270, 500, 2500);
        }
        else if (topic == "ProjectGroup1.2/command/angle_joint2") {
            if (payload == "up") {
                current_angle_joint2 = constrain(current_angle_joint2 + 3, 0, 270);
            }
            else if (payload == "down") {
                current_angle_joint2 = constrain(current_angle_joint2 - 3, 0, 270);
            }
            current_pwm_joint2 = map(current_angle_joint2, 0, 270, 500, 2500);
        }
            else if (topic == "ProjectGroup1.2/command/angle_joint3") {
            if (payload == "up") {
                current_angle_joint3 = constrain(current_angle_joint3 + 3, 0, 270);
            }
            else if (payload == "down") {
                current_angle_joint3 = constrain(current_angle_joint3 - 3, 0, 270);
            }
            current_pwm_joint3 = map(current_angle_joint3, 0, 270, 500, 2500);
        }
        else if (topic == "ProjectGroup1.2/command/angle_gripper") {
            if (payload == "up") {
                current_angle_gripper = constrain(current_angle_gripper + 3, 135, 180);
                // current_angle_gripper = constrain(current_angle_gripper + 3, -35, 0);
            }
            else if (payload == "down") {
                current_angle_gripper = constrain(current_angle_gripper - 3, 135, 180);
                // current_angle_gripper = constrain(current_angle_gripper - 3, -35, 0);
            }
            current_pwm_gripper = map(current_angle_gripper, 0, 270, 500, 2500);
        }
    }    
}

void doSomething(){
    float angle_base_main = magEn_base.getAngle();
    if (current_mode == "Auto"){
        current_angle_base = angle_base;
        pid.setTargetAngle(angle_base);
        pid.getCurrentAngle(angle_base_main);
        pid.update();
        // Serial.println("Angle base: " + String(angle_base));

        angle_joint1 = (angle_joint1 > 225) ? 225 : (angle_joint1 < 45 ? 45 : angle_joint1);
        float angle_joint1_map = map(angle_joint1, 0, 270, 500, 2500);
        servo_joint1.writeMicroseconds(angle_joint1_map);
        // Serial.println("Angle joint1: " + String(angle_joint1));

        angle_joint2 = (angle_joint2 > 270) ? 270 : (angle_joint2 < 0 ? 0 : angle_joint2);
        float angle_joint2_map = map(angle_joint2, 0, 270, 500, 2500);
        servo_joint2.writeMicroseconds(angle_joint2_map);
        // Serial.println("Angle joint2: " + String(angle_joint2));

        angle_joint3 = (angle_joint3 > 270) ? 270 : (angle_joint3 < 0 ? 0 : angle_joint3);
        float angle_joint3_map = map(angle_joint3, 0, 270, 500, 2500);
        servo_joint3.writeMicroseconds(angle_joint3_map);
        // Serial.println("Angle joint3: " + String(angle_joint3));

        angle_gripper = (angle_gripper > 170) ? 170 : (angle_gripper < 135 ? 135 : angle_gripper);
        float angle_gripper_map = map(angle_gripper, 0, 270, 500, 2500);
        servo_gripper.writeMicroseconds(angle_gripper_map);
        // Serial.println("Angle gripper: " + String(angle_gripper));
    }
    
    if (current_mode == "Manual"){
        pid.setTargetAngle(current_angle_base);
        pid.getCurrentAngle(angle_base_main);
        pid.update();

        mqtt.publish("ProjectGroup1.2/command/angle_base/possition", String(current_angle_base));
        // Serial.println("Adjusted angle_base: " + String(current_angle_base));

        servo_joint1.writeMicroseconds(current_pwm_joint1);
        mqtt.publish("ProjectGroup1.2/command/angle_joint1/possition", String(current_angle_joint1));
        // Serial.println("Adjusted angle_joint1: " + String(current_angle_joint1));

        servo_joint2.writeMicroseconds(current_pwm_joint2);
        mqtt.publish("ProjectGroup1.2/command/angle_joint2/possition", String(current_angle_joint2));
        // Serial.println("Adjusted angle_joint2: " + String(current_angle_joint2));

        servo_joint3.writeMicroseconds(current_pwm_joint3);
        mqtt.publish("ProjectGroup1.2/command/angle_joint3/possition", String(current_angle_joint3));
        // Serial.println("Adjusted angle_joint3: " + String(current_angle_joint3));

        servo_gripper.writeMicroseconds(current_pwm_gripper);
        mqtt.publish("ProjectGroup1.2/command/angle_gripper/possition", String(current_angle_gripper));
        // Serial.println("Adjusted angle_gripper: " + String(current_angle_gripper));
    }
}