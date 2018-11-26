#include <Wire.h>
#include <avr/pgmspace.h>
#include "shifter.h"
#include "ioExpander.h"

const int WRITE_PULSE_WIDTH_uS = 150; // min 100uS
const int pOE_ = 5;
const int pWE_ = 4;

const byte program[] PROGMEM= {
//  #include "io.h"
//#include "simpleLoader.h"
//  #include "echo.h"
//    #include "bidir.h"
//  #include "ramTest.h"
//   #include "testPrintString.h"
//   #include "loader3.h"
 #include "rom.h"
#if 0
  0x3e, 0x4f, 0xd3, 0x02, 0x3e, 0xff, 0xd3, 0x03, 
    0x3e, 0x00, 0xd3, 0x03, 0x3e, 0x83, 0xd3, 0x02, 
    0x3e, 0x03, 0xd3, 0x03, 0xed, 0x56, 0xfb, 0xdb,   q
    0x00, 0x76, 0xc3, 0x19, 0x00
#endif
 };

void setup() {
    pinMode(pOE_, OUTPUT);
    digitalWrite(pOE_, HIGH);
    pinMode(pWE_, OUTPUT);
    digitalWrite(pWE_, HIGH);

    Wire.begin();
    Serial.begin(9600);

    configureShifterPins();
    configureGPAAsOutput();
    configureGPBAsOutput();

    delay(10); // EEProm Erase Protection requires 5ms delay before write
    Serial.println(F("Writing"));
    int origin = 0x0000;
#if 1
    for (int i = 0; i < sizeof(program)/sizeof(byte); ++i) {
      byte b = pgm_read_byte_near(program + i);
        writeByteToAddress(b, i + origin);
         delay(10); // write cycle takes max 10 ms
    }
#endif

    Serial.println(F("\nReading"));
    configureGPBAsInput();
    delay(1);
    int sum = 0;
    for (int i = 0; i < sizeof(program) / sizeof(byte); ++i) {
        byte b = readByteFromAddress(i + origin);
        sum += b;
        byte expected = pgm_read_byte_near(program + i);
        if (b != expected) {
          Serial.println(F(" Error!"));
        }
        else {
          Serial.println();
        }
    }
    Serial.print("Sum: ");
    Serial.println(sum);
}

void writeByteToAddress(byte data, short unsigned address) {
    Serial.print(F("Writing "));
    Serial.print(address, HEX);
    Serial.print(": ");
    Serial.print((int)data, HEX);
    Serial.println();
    
    digitalWrite(pOE_, HIGH);
    writeAddress(address);
    digitalWrite(pWE_, LOW); // address latched
    writeData(data);
    delayMicroseconds(WRITE_PULSE_WIDTH_uS);
    digitalWrite(pWE_, HIGH); // data lateched
    // delayMicroseconds(50);
    delay(10); // write cycle takes max 10 ms
}

void writeAddress(short unsigned address) {
    char highByte = address >> 8;
    char lowByte = (byte)address & 0xff;
    shifterWriteValue(highByte);
    writeByteToGPA(lowByte);
}

void writeData(byte v) {
    writeByteToGPB(v);
}

byte readByteFromAddress(short unsigned address) {
    delay(10);
    digitalWrite(pOE_, LOW);
    writeAddress(address);
    delay(1);
    byte data = readByteFromGPB();

    Serial.print(address, HEX);
    Serial.print(": ");
    Serial.print((int)data, HEX);
    return data;
}

void loop() {
}
