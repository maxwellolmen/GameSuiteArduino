#include "GameController.h"
#include "ClientController.h" // Include the header file for ClientController here
#include "SuiteTFT.h"

GameController::GameController(ClientController * clientController, SuiteTFT * tft) {
    this->clientController = clientController;
}