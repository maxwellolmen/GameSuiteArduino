#include "GameController.h"
#include "ClientController.h" // Include the header file for ClientController here
#include "SuiteTFT.h"

GameController::GameController(ClientController * clientController, SuiteTFT * tft) {
    this->clientController = clientController;
}

ClientController * GameController::getClientController(){
    return this->clientController;
}

SuiteTFT * GameController::getTFT(){
    return this->tft;
}