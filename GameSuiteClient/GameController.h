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
        //constructor sets up the client and screen
        GameController(ClientController * clientController, SuiteTFT * tft);
        //reads from the server and executes a command
        virtual void read(int command, char * data);
        //writes to the server
        virtual void write(int command, char * data);
        //initializes the game and sets up resources necessary
        virtual void intializeGame();
        //clears up the game and frees resources necessary as well as resets the screen
        virtual void destroyGame();
        //returns the client controller in case the specific controller needs it 
        ClientController * getClientController();
        //returns the tft in case the current controller needs it
        SuiteTFT * getTFT();
    private:
    ClientController * clientController;
    SuiteTFT * tft;
};
#endif