//test
#ifndef PING_GAME_CONTROLLER_H
#define PING_GAME_CONTROLLER_H
#include "GameController.h"
#include "ClientController.h"
#include "SuiteTFT.h"
class ClientController;

class PingGameController: public GameController{
    PingGameController(ClientController * clientController, SuiteTFT * tft):GameController(clientController, tft){
        this->intializeGame();
    }
    
    void intializeGame();
    void destroyGame();
    void write(int command, char *data);
    void read(int command, char *data);
    
    ~PingGameController();
};

#endif