#include "SuiteLCD.h"

void SuiteLCD::printDebug(char* message) {
    screen.clear();
    screen.setCursor(0, 0);
    screen.print(message);
}