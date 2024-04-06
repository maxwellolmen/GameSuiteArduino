#include <EEPROM.h>
#include <UTFT.h>

// ESP to client codes
#define NETWORK_ACK 0
#define SERVER_RESPONSE 1


// Client to ESP codes
#define SET_NETWORK 0
#define SET_NETWORK_LEN 96

#define debugSerial Serial
#define espSerial Serial1

//Server Client commands
#define PONG 0

// pin definition for the Leonardo
// #define CS   7
// #define DC   0
// #define RESET  1

UTFT myScreen(CTE40, 38, 39, 40, 41);
extern uint8_t BigFont[];

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


void printDebug(char * message){
    debugSerial.println(message);
    myScreen.fillScr(255, 0 ,0);
    myScreen.setColor(0, 0, 0);
    myScreen.print(message, 240, 0);

    int yCoord = 0;
    int length = strlen(message);

    while (length > 30) {
        char submessage[31];
        memcpy(submessage, message, 30);
        submessage[30] = '\0';

        myScreen.print(submessage, 0, yCoord);
        length -= 30;
        yCoord += 16;
        message += 30;
    }

    myScreen.print(message, 0, yCoord);
}
//320Y
//480X
void setup() {
  debugSerial.begin(9600);
  espSerial.begin(115200);
  connected = false;
  verified = false;
  myScreen.InitLCD();
  myScreen.setBackColor(255, 255, 255);
  myScreen.setFont(BigFont);
  printDebug("Started! Waiting for init ack from ESP...");

  int firstBootFlag;
    EEPROM.get(0, firstBootFlag);

    if (firstBootFlag != 3103) {
        debugSerial.println(F("First boot! Default settings loading..."));
        WifiNetwork network;

        char ssid[] = "CIA-TAP\0                        ";
        memcpy(&network.ssid, ssid, 32);

        char pass[] = "minecraF44!\0                                                      ";
        memcpy(&network.pass, pass, 64);

        EEPROM.put(4, network);
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
        EEPROM.get(4, network);
        // char ssid[33] = "Maxwell\0                        ";
        // char pass[65] = "bigkev2019\0                                                     ";
        setNetwork(network.ssid, network.pass);
        connected = true;
    }

    if (espSerial.available() >= 2) {
        byte command = espSerial.read();
        debugSerial.print(F("Cmd "));
        debugSerial.println(command);

        byte length = espSerial.read();

        while (espSerial.available() < length) {
            delay(10);
        }

        char data[length];
        espSerial.readBytes(data, length);

        processESP(command, length, data);
    }
}

void setNetwork(char* ssid, char* pass) {
    printDebug("Sending SET_NETWORK to connect to SSID '");
    printDebug(ssid);
    printDebug("'...");

    espSerial.write(SET_NETWORK);
    espSerial.write(SET_NETWORK_LEN);
    espSerial.write(ssid, 32);
    espSerial.write(pass, 64);
}

void processServerCommand(byte command, byte length, char * data){
  if (command == PONG){
    printDebug("PONG");
  }
}

void processESP(byte command, byte length, char* data) {
  switch (command) {
    case NETWORK_ACK:
      bool success = data[0];

      if (!success) {
        printDebug("Network connection failed! Trying again...");
        connected = false;
        verified = false;
      } else {
        printDebug("Network connection successful!");
        verified = true;
      }
      break;
    case SERVER_RESPONSE:
      byte serverCommand = data[0];
      processServerCommand(serverCommand, length, data);
  }
}