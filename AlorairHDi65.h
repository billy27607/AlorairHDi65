#ifndef ALORAIRHDI65_H
#define ALORAIRHDI65_H

#include <Arduino.h>
#include "mcp2515_can.h"

class AlorairHDi65
{
public:
    float currentHumidity;
    float targetHumidity;
    float currentTemperature;
    bool currentStatus;
    bool powerState;
    bool pumpState;

    AlorairHDi65(uint8_t csPin);
    bool begin();
    bool status();
    void printStatus();
    bool setPower(bool state);
    bool setTargetHumidity(int humidity);
    bool pumpOut();

private:
    uint8_t csPin;
    bool sendGet(unsigned char *to_be_sent, unsigned char *received);
};

#endif
