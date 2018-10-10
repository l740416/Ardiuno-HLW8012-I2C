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

void HLW8012_I2C::SetRegisters(double voltage_up, double voltage_down, double current)
{
	uint16_t v_up   = (uint16_t)(voltage_up / 100.0);
	uint16_t v_down = (uint16_t)(voltage_down / 100.0);
	uint16_t c      = (uint16_t)(current * 1000.0);
	
	write16(CMD_SET_VOLTAGE_UPSTREAM_REG, v_up);
	write16(CMD_SET_VOLTAGE_DOWNSTREAM_REG, v_down);
	write16(CMD_SET_CURRENT_REG, c);
	write0(CMD_UPDATE_REGISTERS);
}

void HLW8012_I2C::CalibrateVoltage(double expect_value)
{
	uint16_t value = (uint16_t)(expect_value * 100.0);
	write16(CMD_SET_EXPECT_VOLTAGE, value);
	write0(CMD_CALIBRATE_VOLTAGE);
}	

void HLW8012_I2C::CalibrateCurrent(double expect_value)
{
	uint16_t value = (uint16_t)(expect_value * 1000.0);
	write16(CMD_CALIBRATE_CURRENT, value);
	write0(CMD_CALIBRATE_VOLTAGE);
}	

void HLW8012_I2C::CalibratePower(double expect_value)
{
	uint16_t value = (uint16_t)(expect_value * 100.0);
	write16(CMD_SET_EXPECT_VOLTAGE, value);
	write0(CMD_CALIBRATE_POWER);
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

