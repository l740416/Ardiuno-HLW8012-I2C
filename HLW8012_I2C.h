/*
  Copyright 2018 Chiehting Huang <j.huang.1985@gmail.com>

*/
#ifndef HLW8012_I2C_h
#define HLW8012_I2C_h

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#ifdef __AVR_ATtiny85__
 #include "TinyWireM.h"
 #define Wire TinyWireM
#elif defined(ARDUINO_ARCH_ESP8266)
 #include <Wire.h>
#else
 #include <avr/pgmspace.h>
 #include <Wire.h>
#endif


// I2C commands
#define SLAVE_ADDRESS                  0x04
#define MAX_SENT_BYTES                 5
#define CMD_VERSION                    0x00   // Out:4B
#define CMD_GET_POWER                  0x01   // Out:4B, UNIT: uW
#define CMD_GET_VOLTAGE                0x02   // Out:4B, UNIT: uV
#define CMD_GET_CURRENT                0x03   // Out:4B, UNIT: uA
#define CMD_GET_POWER_MULTIPLIER       0x04   // Out:4B, UINT: 1000x
#define CMD_GET_VOLTAGE_MULTIPLIER     0x05   // Out:4B, UNIT: 1000x
#define CMD_GET_CURRENT_MULTIPLIER     0x06   // Out:4B, UNIT: 1000x
#define CMD_GET_POWER_PULSEWIDTH       0x07   // Out:4B, UNIT: us
#define CMD_GET_VOLTAGE_PULSEWIDTH     0x08   // Out:4B, UNIT: us
#define CMD_GET_CURRENT_PULSEWIDTH     0x09   // Out:4B, UNIT: us
#define CMD_SET_VOLTAGE_UPSTREAM_REG   0x10   // In :2B, UNIT: 0.1KOhm 
#define CMD_SET_VOLTAGE_DOWNSTREAM_REG 0x11   // In :2B, UNIT: 0.1KOhm
#define CMD_SET_CURRENT_REG            0x12   // In :2B, UNIT: 1.0mOhm
#define CMD_SET_EXPECT_VOLTAGE         0x13   // In :2B, UNIT: 0.01 V
#define CMD_SET_EXPECT_CURRENT         0x14   // In :2B, UNIT: 0.001 A
#define CMD_SET_EXPECT_POWER           0x15   // In :2B, UNIT: 0.01 W
#define CMD_UPDATE_REGISTERS           0x20
#define CMD_PERFORM_MEASUREMENT        0x21
#define CMD_CALIBRATE_VOLTAGE          0x22
#define CMD_CALIBRATE_CURRENT          0x23
#define CMD_CALIBRATE_POWER            0x24

class HLW8012_I2C
{
  public:
    HLW8012_I2C(uint8_t i2cAddress = SLAVE_ADDRESS);
#if defined(ARDUINO_ARCH_ESP8266)
    bool begin(int SDA_pin, int SCL_pin);
#else
    bool begin();
#endif
    bool exist();
    uint32_t Firmware()  { return read32(CMD_VERSION); }
    uint32_t Power()     { return read32(CMD_GET_POWER); }
    uint32_t Voltage()   { return read32(CMD_GET_VOLTAGE); }
    uint32_t Current()   { return read32(CMD_GET_CURRENT); }
    
    uint32_t PowerMultiplier()     { return read32(CMD_GET_POWER_MULTIPLIER); }
    uint32_t VoltageMultiplier()   { return read32(CMD_GET_VOLTAGE_MULTIPLIER); }
    uint32_t CurrentMultiplier()   { return read32(CMD_GET_CURRENT_MULTIPLIER); }
    
    uint32_t PowerPulseWidth()     { return read32(CMD_GET_POWER_PULSEWIDTH); }
    uint32_t VoltagePulseWidth()   { return read32(CMD_GET_VOLTAGE_PULSEWIDTH); }
    uint32_t CurrentPulseWidth()   { return read32(CMD_GET_CURRENT_PULSEWIDTH); }
    
    void     Measure()   { write0(CMD_PERFORM_MEASUREMENT); }

    void     SetRegisters(double voltage_up, double voltage_down, double current);
    void     CalibrateVoltage(double expect_value);
    void     CalibrateCurrent(double expect_value);
    void     CalibratePower(double expect_value);

  private:
    void     write0(uint8_t reg);
    void     write16(uint8_t reg, uint16_t data);
    uint32_t read32(uint8_t reg);
    
    bool    _exist;
    uint8_t _address;
};

#endif

