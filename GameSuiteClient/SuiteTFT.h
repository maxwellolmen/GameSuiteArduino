#include <UTFT.h>

extern uint8_t BigFont[];

class SuiteTFT {
    public:
        void printDebug(char* message);

        SuiteTFT() : screen(CTE40, 38, 39, 40, 41) {
            screen.InitLCD();
            screen.setBackColor(255, 255, 255);
            screen.setFont(BigFont);
        }
    private:
        UTFT screen;
};