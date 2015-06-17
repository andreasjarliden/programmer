#pragma once
#include <Arduino.h>

void configureGPAAsOutput();
void configureGPBAsOutput();
void configureGPBAsInput();
void writeByteToGPA(byte v);
void writeByteToGPB(byte v);
byte readByteFromGPB();
void writeByteToIOExpanderRegister(byte value, char reg);
byte readByteFromIOExpanderRegister(char reg);
