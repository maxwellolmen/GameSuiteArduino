#include <LiquidCrystal.h>

#define RS1 13
#define EN1 12
#define D41 11
#define D51 10
#define D61 9
#define D71 8

#define RS2 52
#define EN2 53
#define D42 50
#define D52 51
#define D62 48
#define D72 49

class SuiteLCD {
    public:
        SuiteLCD() : screen(RS1, EN1, D41, D51, D61, D71) {
            
        }

        void printDebug(char* message);
    private:
        LiquidCrystal screen;
};