#include "SuiteLCD.h"

#define SCREEN_WIDTH 16

void SuiteLCD::printDebug(char* message) {
    screen.clear();

    screen.setCursor(0, 0);
    for (int i = 0; i < strlen(message) && i < SCREEN_WIDTH; i++) {
        screen.write(message[i]);
    }

    screen.setCursor(0, 1);
    for (int i = 16; i < strlen(message) && i < SCREEN_WIDTH * 2; i++) {
        screen.setCursor(i - 16, 1);
        screen.write(message[i]);
    }
}