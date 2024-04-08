#ifndef SUITE_IO_H
#define SUITE_IO_H

#include "SuiteLCD.h"
#include "SuiteTFT.h"

#define LCD
// #define TFT

struct SuiteIO {
    #ifdef LCD
    SuiteLCD* lcd1;
    SuiteLCD* lcd2;
    #endif

    #ifdef TFT
    SuiteTFT* tft;
    #endif

    HardwareSerial* debugSerial;
    HardwareSerial* espSerial;
};

#endif