#ifndef TCA9548A_H
#define TCA9548A_H

#include <Arduino.h>
#include <Wire.h>

class TCA9548A {
    public:
      // TCA9548A(uint8_t address);  // Constructor
      void selectBus(uint8_t bus);        // Function to get Address
    
    private:
      uint8_t _address;  // Address TCA9548A
    };

#endif