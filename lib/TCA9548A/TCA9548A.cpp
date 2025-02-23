#include <Arduino.h>
#include "TCA9548A.h"

TCA9548A::TCA9548A(uint8_t address) {
  _address = address; 
}

void TCA9548A::selectBus(uint8_t bus) {
  Wire.beginTransmission(_address);  // TCA9548A address
  Wire.write(1 << bus);               
  byte status = Wire.endTransmission();  

    // Serial.print("Selected bus: ");
    // Serial.println(bus);
}
