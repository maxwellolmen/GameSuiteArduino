/*
base class for controller. has functions that need to be defined such as 
read which reads the server command and executes it
write which will be used to send a command to the server 
initializeGame which will be used to set up the UTFT and start the game's state
destroyGame, frees resources if any and resets anything that needs to be resetted.
*/
#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H
#include <UTFT.h>
#include <Arduino.h>
class ClientController;
#include "SuiteTFT.h"

class GameController{
    public:
        GameController(ClientController * clientController, SuiteTFT * tft);
        virtual void read(int command, char * data) = 0;
        virtual void write(int command, char * data) = 0;
        virtual void intializeGame() = 0;
        virtual void destroyGame() = 0;
    private:
    ClientController * clientController;
    SuiteTFT * tft;
};
#endif