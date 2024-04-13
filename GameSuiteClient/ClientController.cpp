#include "ClientController.h"
#include "GameController.h" // Include the header file for ClientController here
#include "SuiteClient.h"

void ClientController::handleCommand(int command, char * data) {
    this->gameController->read(command, data);
};

void ClientController::setController(GameController * gameController){
    this->gameController->destroyGame();
    this->gameController = gameController;
    this->gameController->intializeGame();
};
