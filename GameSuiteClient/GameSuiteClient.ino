#include <EEPROM.h>
#include <UTFT.h>
#include <LiquidCrystal.h>
#include "SuiteLCD.h"
#include "SuiteTFT.h"
#include "SuiteClient.h"
#include "SuiteIO.h"
#include "ClientController.h"

// DEFINES FOR LCD/UTFT
#define LCD
// #define TFT

// ESP to client codes/Users/maxwellolmen/Documents/GitHub/GameSuiteArduino/GameSuiteClient/SuiteIO.h
#define NETWORK_ACK 0
#define SERVER_RESPONSE 1
#define SERVER_ACK 2

// Client to ESP codes
#define SET_NETWORK 0
#define SET_NETWORK_LEN 96
#define SERVER_REQUEST 1

#define debugSerial Serial
#define espSerial Serial1

// LCD pin defines
#define RS1 13
#define EN1 12
#define D41 11
#define D51 10
#define D61 9
#define D71 8

#define RS2 52
#define EN2 53
#define D42 50
#define D52 51
#define D62 48
#define D72 49

// LCD SEGMENT
#ifdef LCD
SuiteLCD lcd1(RS1, EN1, D41, D51, D61, D71);
SuiteLCD lcd2(RS2, EN2, D42, D52, D62, D72);
#endif

// UTFT SEGMENT
#ifdef TFT
SuiteTFT tft;
#endif

SuiteIO io = {
    #ifdef LCD
    &lcd1,
    &lcd2,
    #endif

    #ifdef TFT
    &tft,
    #endif

    &debugSerial,
    &espSerial
};

SuiteClient client(&espSerial, &io);

// initial position of the point is the middle of the screen
// initial position of the point is the middle of the screen
int xPos = 80;
int yPos = 64;

// direction and speed
int xDir = 1;
int yDir = 1;

// variables to keep track of the point's location
int xPrev = xPos;
int yPrev = yPos;

struct WifiNetwork {
  char ssid[32];
  char pass[64];
};

long millisOnStart;
bool connected;
bool verified;
bool pinged = false;
bool serverConnected = false;

void printDebug(char* message) {
    debugSerial.println(message);

    #ifdef LCD
    lcd1.printDebug(message);
    #endif

    #ifdef TFT
    tft.printDebug(message);
    #endif
}

#ifdef LCD
void printDebug2(char* message) {
    debugSerial.println(message);

    lcd2.printDebug(message);
}
#endif

//320Y
//480X
void setup() {
    // Uncomment to reset settings
    // EEPROM.put(0, (int) 0);
    debugSerial.begin(9600);
    espSerial.begin(115200);
    connected = false;
    verified = false;

    delay(100);

    analogWrite(2, 30);
    
    printDebug("Waiting for ESP...");
    printDebug2("SECOND SCREEN!");

    int firstBootFlag;
    EEPROM.get(0, firstBootFlag);

    if (firstBootFlag != 3103) {
        debugSerial.println(F("First boot! Default settings loading..."));
        WifiNetwork network;

        char ssid[] = "CIA-TAP\0                        ";
        memcpy(&network.ssid, ssid, 32);

        char pass[] = "minecraF44!\0                                                    ";
        memcpy(&network.pass, pass, 64);

        EEPROM.put(8, network);
        EEPROM.put(0, (int) 3103);
    }

    int count = 0;

    do {
        while (!espSerial.available()) {
            delay(50);
        }

        char c = espSerial.read();

        if (c != '\0') {
            count = 0;
        } else {
            count++;
        }
    } while (count < 8);

    millisOnStart = millis();
}

void loop() {
    if (!connected && millis() - millisOnStart > 500) {
        WifiNetwork network;
        EEPROM.get(8, network);
        // char ssid[33] = "Maxwell\0                        ";
        // char pass[65] = "bigkev2019\0                                                     ";
        setNetwork(network.ssid, network.pass);
        connected = true;
    }

    if (espSerial.available() >= 2) {
        byte command = espSerial.read();
        printDebug2("Command         Received");
        debugSerial.print(F("Cmd "));
        debugSerial.println(command);

        byte length = espSerial.read();
        debugSerial.print(F("Length "));
        debugSerial.println(length);

        while (espSerial.available() < length) {
            delay(10);
        }

        char data[length];
        espSerial.readBytes(data, length);

        processESP(command, length, data);
    }

    if (serverConnected && !pinged) {
        client.sendCommand(PING, 0, NULL);
        printDebug2("SENDING PING");
        pinged = true;
    }

    // handle input from modules
}

void setNetwork(char* ssid, char* pass) {
    printDebug("Sent SET_NETWORK");
    debugSerial.print(F("Sending SET_NETWORK to connect to SSID '"));
    debugSerial.print(ssid);
    debugSerial.println(F("'..."));

    espSerial.write(SET_NETWORK);
    espSerial.write(SET_NETWORK_LEN);
    espSerial.write(ssid, 32);
    espSerial.write(pass, 64);
}

void processESP(byte command, byte length, char* data) {
    debugSerial.println(F("PROCESS ESP"));
    debugSerial.println(command);

    if (command == NETWORK_ACK) {
        debugSerial.println(F("NETWORK ACK"));
        byte success = data[0];

        if (success == 0) {
            debugSerial.println(F("Network connection failed! Trying again..."));
            connected = false;
            verified = false;
        } else {
            debugSerial.println(F("Network connection successful!"));
            verified = true;
        }
    } else if (command == SERVER_RESPONSE) {
        debugSerial.println(F("SERVER RESPONSE"));

        byte serverCommand = data[0];
        debugSerial.println(serverCommand);
        debugSerial.println(length);
        // byte serverData[length - 2];
        /*memcpy(serverData, data + 2, length - 2);

        client.handleServerCommand(serverCommand, length - 2, serverData);*/
    } else if (command == SERVER_ACK) {
        debugSerial.println(F("SERVER ACK"));
        bool connectSuccess = data[0];

        debugSerial.println(connectSuccess);

        if (connectSuccess) {
            debugSerial.println(F("SERVER CONNECTED!"));
            serverConnected = true;
        }
    }
}