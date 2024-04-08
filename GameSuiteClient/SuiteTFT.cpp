#include "SuiteTFT.h"

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