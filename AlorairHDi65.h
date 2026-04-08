#ifndef ALORAIRHDI65_H
#define ALORAIRHDI65_H

#include <Arduino.h>
#include "mcp2515_can.h"

class AlorairHDi65
{
public:
    float currentHumidity = 0;
    float targetHumidity = 0;
    float currentTemperature = 0;
    bool currentStatus;
    bool powerState;
    bool pumpState;
    bool connected = false;

    AlorairHDi65(uint8_t csPin);
    bool begin();
    bool isConnected();
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
