#ifndef SUITE_CLIENT_H
#define SUITE_CLIENT_H

#include <Arduino.h>
#include "SuiteIO.h"

// Client to Server commands
#define PING 0

// Server to Client commands
#define PONG 0

class SuiteClient {
    public:
        SuiteClient(HardwareSerial* espSerial, SuiteIO* io) {
            this->espSerial = espSerial;
            this->io = io;
        }

        void handleServerCommand(char command, char length, char* data);
        void sendCommand(char command, char length, char* data);
    private:
        HardwareSerial* espSerial;
        SuiteIO* io;
};

#endif