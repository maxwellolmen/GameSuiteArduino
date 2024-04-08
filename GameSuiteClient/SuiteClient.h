#include "HardwareSerial.h"

// Client to Server commands
#define PING 0

// Server to Client commands
#define PONG 0

class SuiteClient {
    public:
        SuiteClient(HardwareSerial* espSerial) {
            this->espSerial = espSerial;
        }

        void handleServerCommand(char command, char length, char* data);
        void sendCommand(char command, char* data);
    private:
        HardwareSerial* espSerial;
};