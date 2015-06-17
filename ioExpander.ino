#include "ioExpander.h"
#include <Wire.h>

static const char IODIRA = 0;
static const char IODIRB = 1;
static const char GPIOA = 0x12;
static const char GPIOB = 0x13;
static const char ALL_OUTPUTS = 0x00;
static const char ALL_INPUTS = 0xFF;
static const byte IO_EXPANDER_ADDRESS = 0x20;

void configureGPAAsOutput() {
    writeByteToIOExpanderRegister(ALL_OUTPUTS, IODIRA);
}

void configureGPBAsOutput() {
    writeByteToIOExpanderRegister(ALL_OUTPUTS, IODIRB);
}

void configureGPBAsInput() {
    writeByteToIOExpanderRegister(ALL_INPUTS, IODIRB);
}

void writeByteToGPA(byte v) {
    writeByteToIOExpanderRegister(v, GPIOA);
}

void writeByteToGPB(byte v) {
    writeByteToIOExpanderRegister(v, GPIOB);
}

byte readByteFromGPB() {
    return readByteFromIOExpanderRegister(GPIOB);
}


void writeByteToIOExpanderRegister(byte value, char reg) {
    Wire.beginTransmission(IO_EXPANDER_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    int rc = Wire.endTransmission();
    if (rc) {
        Serial.print("endTransmission returned error");
        Serial.println(rc);
    }
}

byte readByteFromIOExpanderRegister(char reg) {
    Wire.beginTransmission(IO_EXPANDER_ADDRESS);
    Wire.write(reg);
    int rc = Wire.endTransmission();
    if (rc) {
        Serial.print("endTransmission returned error");
        Serial.println(rc);
    }
    Wire.requestFrom((byte)IO_EXPANDER_ADDRESS, (byte)1);
    byte value = Wire.read();
    return value;
}


