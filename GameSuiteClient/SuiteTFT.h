#ifndef SUITE_TFT_H
#define SUITE_TFT_H

#include <UTFT.h>
#include <Arduino.h>

extern uint8_t BigFont[];

class SuiteTFT {
    public:
        void printDebug(char* message);

        SuiteTFT() : screen(CTE40, 38, 39, 40, 41) {
            screen.InitLCD();
            screen.setBackColor(255, 255, 255);
            screen.setFont(BigFont);
        }

        UTFT * getScreen(){
            return &this->screen;
        }

    private:
        UTFT screen;
};

#endif