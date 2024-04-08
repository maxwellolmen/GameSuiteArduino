#include "SuiteClient.h"

// Client to ESP codes
#define SERVER_REQUEST 1

void SuiteClient::handleServerCommand(char command, char length, char* data){
    if (command == PONG) {
        Serial.println("PONG RECEIVED!");
    }
}

void SuiteClient::sendCommand(char command, char* data){
    espSerial->write(SERVER_REQUEST);
    espSerial->write(2 + sizeof(data));
    espSerial->write(command);

    if (data == NULL) {
        espSerial->write(0);
    } else {
        espSerial->write(sizeof(data));
        espSerial->write(data);
    }
    
}