#include "PIDControl.h"

PIDControl::PIDControl(float Kp, float Ki, float Kd) {
  this -> Kp = Kp;
  this -> Ki = Ki;
  this -> Kd = Kd;
  previousError = 0;
  integral = 0;
}

void PIDControl::begin(int pin) {
  myServo.attach(pin);
  targetAngle = 0;
}

void PIDControl::setTargetAngle(float targetAngle) {
  this -> targetAngle = targetAngle;
}

float PIDControl::getCurrentAngle() {
  currentAngle = encoder.getAngle(); // return
  return currentAngle;
}

void PIDControl::update() {
  float error = targetAngle - getCurrentAngle();

  integral += error;
  float derivative = error - previousError;

  float output = Kp * error + Ki * integral + Kd * derivative;
  Serial.println(output);

  if (output > 0) {
    myServo.writeMicroseconds(1500 + output);
  } else if (output < 0) {
    myServo.writeMicroseconds(1500 + output);
  } else {
    myServo.writeMicroseconds(1500);
  }

  previousError = error;
}