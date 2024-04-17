#include "PingGameController.h"
#include "GameController.h"
#include "ClientController.h" // Include the header file for ClientController here
#include "SuiteTFT.h"


void PingGameController::intializeGame(){
    SuiteTFT * tft = GameController::getTFT();
    tft->writeMessageCenter("HAROOO ANITA MAX WYNNN", false);
}

void PingGameController::destroyGame(){

}

void PingGameController::read(int command, char *data){

}

void PingGameController::write(int command, char *data){

}

PingGameController::~PingGameController(){
    PingGameController::destroyGame();
}