#include "SuiteTFT.h"

#define SIZE_Y 320
#define SIZE_X 480
#define X_SIZEOFTEXT 16

void SuiteTFT::printDebug(char* message) {
    screen.fillScr(255, 0 ,0);
    screen.setColor(0, 0, 0);
    screen.print(message, 240, 0);

    int yCoord = 0;
    int length = strlen(message);

    while (length > 30) {
        char submessage[31];
        memcpy(submessage, message, 30);
        submessage[30] = '\0';

        screen.print(submessage, 0, yCoord);
        length -= 30;
        yCoord += 16;
        message += 30;
    }

    screen.print(message, 0, yCoord);
}

void SuiteTFT::writeMessageCenter(String message, bool wrapAround){
    screen.setColor(0, 0, 0);
    screen.print(message, 240, 0);

    if (wrapAround){    
        int start = 0;
        for ( int i = 0; i < message.length(); i++ ){
            int end = start + message.length();
            if ( end * X_SIZEOFTEXT > SIZE_X ){
                start = 0;  
            }
            start += 1;    
            screen.print(message, start, SIZE_Y / 2);
        }
    }else{
        screen.print(message, 0, SIZE_Y / 2);
    }
}