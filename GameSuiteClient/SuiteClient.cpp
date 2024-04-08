#include "SuiteClient.h"
#include <Arduino.h>

// Client to ESP codes
#define SERVER_REQUEST 1

// Server to Client codes
#define PONG 0

void SuiteClient::handleServerCommand(char command, char length, char* data) {
    io->debugSerial->print(F("Server Command: "));
    io->debugSerial->println((int) command);
    io->debugSerial->print(F("Length: "));
    io->debugSerial->println((int) length);

    if (command == PONG) {
        io->lcd2->printDebug("PONG RECEIVED");
    }
}

void SuiteClient::sendCommand(char command, char length, char* data) {

    espSerial->write(SERVER_REQUEST);
    espSerial->write(2 + length);
    espSerial->write(command);

    if (data == NULL) {
        espSerial->write(0);
    } else {
        espSerial->write(length);
        espSerial->write(data);
    }
}