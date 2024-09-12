#include <Arduino.h>
#include <SPI.h>
#include "mcp2515_can.h"
#include <AlorairHDi65.h>

uint8_t csPin = D7;
mcp2515_can CAN(csPin);
unsigned long interval = 5000;
unsigned long = last_read = 0;

AlorairHDi65::AlorairHDi65(uint8_t Pin) {
    // Serial.print("Initializing CAN with csPin: ");
    csPin = Pin;
    // Serial.println(csPin);
}


bool AlorairHDi65::begin() {
    unsigned long int start = millis();
    while (CAN_OK != CAN.begin(CAN_50KBPS)) {
        Serial.println("CAN init fail, retrying...");
        delay(100);
        if (millis() - start > 5000) return 0;
    }
    // Serial.println("CAN init ok!");
    return true;
}

bool AlorairHDi65::sendGet(unsigned char *to_be_sent, unsigned char *received) {
    CAN.sendMsgBuf(0x123, 0, 8, to_be_sent);
    unsigned long start_time = millis();
    unsigned long time_out = 400;
    unsigned char len = 0;

    while (millis() - start_time < time_out) {
        if (CAN_MSGAVAIL == CAN.checkReceive()) {
            CAN.readMsgBuf(&len, received);
            return true;
        }
    }
    return false;
}

bool AlorairHDi65::status() {
    unsigned char out_msg[8] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char in_msg[8];
    if (sendGet(out_msg, in_msg)) {
        char bit1 = (in_msg[5] & 0b1) != 0;
        char bit2 = (in_msg[5] & 0b10) != 0;
        pumpState = (in_msg[5] & 0b10000) != 0;

        powerState = !!(bit1 | bit2);
        currentStatus = bit1;
        targetHumidity = in_msg[1];
        currentHumidity = in_msg[0];
        currentTemperature = in_msg[3];
        return true;
    } else return false;
}

void AlorairHDi65::printStatus() {
    Serial.print("Current Humidity: ");
    Serial.println(currentHumidity);
    Serial.print("Target Humidity: ");
    Serial.println(targetHumidity);
    Serial.print("Current Temperature: ");
    Serial.println(currentTemperature);
    Serial.print("Current Status: ");
    Serial.println(currentStatus ? "On" : "Off");
    Serial.print("Power State: ");
    Serial.println(powerState ? "On" : "Off");
    Serial.print("Pump State: ");
    Serial.println(pumpState ? "On" : "Off");
    Serial.println("");
}

bool AlorairHDi65::setPower(bool state) {
    unsigned char out_msg[8] = {0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char in_msg[8];
    if (!state) out_msg[2] = 0x00;
    if (sendGet(out_msg, in_msg)) return true;
    return false;
}

bool AlorairHDi65::setTargetHumidity(int humidity) {
    unsigned char out_msg[8] = {0x01, 0x05, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char in_msg[8];
    out_msg[2] = humidity;
    if (sendGet(out_msg, in_msg)) return true;
    return false;
}

bool AlorairHDi65::pumpOut() {
    unsigned char out_msg[8] = {0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
    unsigned char in_msg[8];
    if (sendGet(out_msg, in_msg)) return true;
    return false;
}
