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
  this -> targetAngle = warpDegree(targetAngle);
}

void PIDControl::getCurrentAngle(float currentAngle) {
  this -> currentAngle = warpDegree(currentAngle);
}

void PIDControl::update() {
  float error = warpDegree(targetAngle - currentAngle);

  integral += error;
  integral = constrain(integral, -100, 100);

  float derivative = error - previousError;

  float output = Kp * error + Ki * integral + Kd * derivative;
  output = constrain(output, -500, 500);

  // Serial.println(output);

  if (abs(error) < 0.5) { 
    myServo.writeMicroseconds(1500);
    integral = 0;
  } else {
    myServo.writeMicroseconds(1500 + output);
  }

  previousError = error;
}

float PIDControl::warpDegree(float degree){
  return degree > 180 ? degree -360 : degree < -180 ? degree + 360 : degree;
}
