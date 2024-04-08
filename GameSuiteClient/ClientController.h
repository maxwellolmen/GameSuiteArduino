#include "SuiteTFT.h"
#include "Arduino.h"
#include "SuiteClient.h"

class ClientController{
    public:
        ClientController(SuiteClient * client){
            this->client = client;
            tft.printDebug("connected!");
        }
    private:

    SuiteClient * client = nullptr;
    SuiteTFT tft;
};