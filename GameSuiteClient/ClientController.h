#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H

#include "Arduino.h"
#include "SuiteClient.h"

class ClientController {
    public:
        ClientController(SuiteClient * client){
            this->client = client;
            tft.printDebug("connected!");
        }
    private:

    SuiteClient * client = nullptr;
    SuiteTFT tft;
};

#endif