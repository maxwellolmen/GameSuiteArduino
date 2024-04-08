#ifndef SUITE_LCD_H
#define SUITE_LCD_H

#include <LiquidCrystal.h>
#include <Arduino.h>

class SuiteLCD {
    public:
        SuiteLCD(int rs, int en, int d4, int d5, int d6, int d7) : screen(rs, en, d4, d5, d6, d7) {
            screen.begin(16, 2);
        }

        void printDebug(char* message);
    private:
        LiquidCrystal screen;
};

#endif