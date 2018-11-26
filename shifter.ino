#include "shifter.h"
#include "Arduino.h"

static const char shifterDataPin = 2;
static const char shifterClockPin = 3;

void configureShifterPins() {
    pinMode(shifterDataPin, OUTPUT);
    pinMode(shifterClockPin, OUTPUT);
}

void shifterWriteValue(char v) {
    static const int NUM_BITS = 5;
    for (int i = 0; i < NUM_BITS; ++i) {
        bool bit = v & 0b00010000;
        shifterWriteBit(bit);
        v = v << 1;
    }
    shifterClockCycle();
}

void shifterWriteBit(int value) {
    digitalWrite(shifterDataPin, value);
    shifterClockCycle();
}

void shifterClockCycle() {
    digitalWrite(shifterClockPin, HIGH);
    digitalWrite(shifterClockPin, LOW);
}

