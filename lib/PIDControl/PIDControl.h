#ifndef PIDCONTROL_H
#define PIDCONTROL_H

#include <ESP32Servo.h>
#include <MagneticEn.h>  

class PIDControl {
  public:
    PIDControl(float Kp, float Ki, float Kd);  // Constructor
    void begin(int pin); 
    void setTargetAngle(float targetAngle);
    void update();
    // float getCurrentAngle();
    void getCurrentAngle(float currentAngle);
    float warpDegree(float degree);

  private:
    float Kp, Ki, Kd;
    float previousError, integral;
    float targetAngle;
    float currentAngle;
    MagneticEn encoder;
    Servo myServo;
};

#endif