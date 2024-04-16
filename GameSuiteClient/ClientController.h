/*
Waits for commands from Server
-> When command is received from server, 
send the command to the appropriate controller

Sends commands to Server

Keeps track of the current controller 

Keeps track of the espSerial for writing
*/

#ifndef CLIENT_CONTROLLER_H
#define CLIENT_CONTROLLER_H
#include "Arduino.h"
#include "SuiteClient.h"
class GameController;
#include "SuiteTFT.h"

class ClientController {
    public:
        ClientController(SuiteClient * client){
            this->client = client;
            tft.printDebug("connected!");
        };

        void handleCommand(int command, char * data);

        void setController(GameController * gameController);

    private:
    SuiteClient * client = nullptr;
    SuiteTFT tft;
    GameController * gameController;

};

#endif