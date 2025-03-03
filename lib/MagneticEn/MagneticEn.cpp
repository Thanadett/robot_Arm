#include "MagneticEn.h"

MagneticEn::MagneticEn() {}  // Constructor

int MagneticEn::getAngle() {
    Wire.beginTransmission(0x36);
    Wire.write(0x0C);
    Wire.endTransmission(false);
    
    Wire.requestFrom(0x36, 2);
    if (Wire.available() == 2) {
        int highByte = Wire.read();
        int lowByte = Wire.read();
        int rawAngle = (highByte << 8) | lowByte;
        
        int in = map(rawAngle, 0, 4095, 0, 360);
        
        if (in > 180) {
            in -= 360;
        }
        if (in < -180) in += 360;
        
        return in;
    }
    
    return 0; 
}
