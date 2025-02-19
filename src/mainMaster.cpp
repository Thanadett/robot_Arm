#include <Arduino.h>
#include <MagneticEn.h>
#include "PIDControl.h"
#include <ESP32Servo.h>

PIDControl pid(1.0, 0.1, 0.1); // Kp Ki Kd
MagneticEn magEn_Arm;

void setup() {
    Wire.begin();
    pid.begin(12);
}

void loop() {
    int angle = magEn_Arm.getAngle();
    Serial.println(angle);

    pid.setTargetAngle(angle);
    pid.update();
}