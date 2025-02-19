#include <Arduino.h>
#include "MagneticEn.h"
#include "TCA9548A.h"

MagneticEn magEn;
TCA9548A tca;

void setup() {
    Serial.begin(115200);
    Wire.begin();
}

void loop() {
    tca.selectBus(1);
    int angle = magEn.getAngle();
    Serial.println(angle);
}