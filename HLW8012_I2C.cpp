/*
    Copyright 2017 Chiehting Huang <j.huang.1985@gmail.com>
*/
#include "Arduino.h"
#include "HLW8012_I2C.h"
//#include <Wire.h>

HLW8012_I2C::HLW8012_I2C(uint8_t i2cAddress) {
  _exist = false;
  _address = i2cAddress;
}

#if defined(ARDUINO_ARCH_ESP8266)
bool HLW8012_I2C::begin(int SDA_pin, int SCL_pin) {
    Wire.begin(SDA_pin,SCL_pin);
#else
bool HLW8012_I2C::begin() {
    Wire.begin();
#endif
    Wire.beginTransmission(_address);
    if (Wire.endTransmission() == 0) {
        _exist = true;
    }
    return _exist;
}

bool HLW8012_I2C::exist() 
{
    return _exist;
}

uint32_t HLW8012_I2C::read32(uint8_t reg)
{
		uint32_t value = 0;
    uint8_t* data = (uint8_t*)&value;

    Wire.beginTransmission(_address);
#if ARDUINO >= 100
    Wire.write(reg);
#else
    Wire.send(reg);
#endif
    Wire.endTransmission();

    Wire.requestFrom(_address, (uint8_t) 4);
#if ARDUINO >= 100
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
#else
    data[0] = Wire.receive();
    data[1] = Wire.receive();
    data[2] = Wire.receive();
    data[3] = Wire.receive();
#endif

    return value;
}

void HLW8012_I2C::write0(uint8_t reg) {
	Wire.beginTransmission(_address);
	
#if (ARDUINO >= 100)
    Wire.write(reg);
#else
    Wire.send(reg);
#endif

    Wire.endTransmission();	
}

void HLW8012_I2C::write16(uint8_t reg, uint16_t data) {
    Wire.beginTransmission(_address); // start transmission to device 

    uint8_t x1, x2;
        
    x1 = (data >>  0 ) & 0xFF;
    x2 = (data >>  8 ) & 0xFF;
    
#if (ARDUINO >= 100)
    Wire.write(reg); // sends register address to read from
    Wire.write(x1);  // write data
    Wire.write(x2);  // write data
#else
    Wire.send(reg); // sends register address to read from
    Wire.send(x1);  // write data
    Wire.send(x2);  // write data
#endif

    Wire.endTransmission();
}

