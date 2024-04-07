#include <EEPROM.h>
#include <UTFT.h>

// ESP to client codes
#define NETWORK_ACK 0
#define SERVER_RESPONSE 1
#define SERVER_ACK 2

// Client to ESP codes
#define SET_NETWORK 0
#define SET_NETWORK_LEN 96
#define SERVER_REQUEST 1

#define debugSerial Serial
#define espSerial Serial1

// Client to Server commands
#define PING 0

// Server to Client commands
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
bool pinged = false;
bool serverConnected = false;

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
    // Uncomment to reset settings
    // EEPROM.put(0, (int) 0);
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

        char ssid[] = "Maxwell\0                        ";
        memcpy(&network.ssid, ssid, 32);

        char pass[] = "bigkev2019\0                                                     ";
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

    if (serverConnected && !pinged) {
        while (true) {
            debugSerial.println("SENDING PING!");
            espSerial.write(SERVER_REQUEST);
            espSerial.write(2);
            espSerial.write(PING);
            espSerial.write(0);
            pinged = true;
            delay(1500);
        }
    }
}

void setNetwork(char* ssid, char* pass) {
    debugSerial.print("Sending SET_NETWORK to connect to SSID '");
    debugSerial.print(ssid);
    debugSerial.println("'...");

    espSerial.write(SET_NETWORK);
    espSerial.write(SET_NETWORK_LEN);
    espSerial.write(ssid, 32);
    espSerial.write(pass, 64);
}

void processServerCommand(byte command, byte length, char * data){
  if (command == PONG) {
    printDebug("PONG");
  }
}

void processESP(byte command, byte length, char* data) {
    debugSerial.println("PROCESS ESP");
    debugSerial.println(command);

    if (command == NETWORK_ACK) {
        debugSerial.println("NETWORK ACK");
        byte success = data[0];

        if (success == 0) {
            printDebug("Network connection failed! Trying again...");
            connected = false;
            verified = false;
        } else {
            printDebug("Network connection successful!");
            verified = true;
        }
    } else if (command == SERVER_RESPONSE) {
        debugSerial.println("SERVER RESPONSE");
        byte serverCommand = data[0];
        processServerCommand(serverCommand, length, data);
    } else if (command == SERVER_ACK) {
        debugSerial.println("SERVER ACK");
        bool connectSuccess = data[0];

        debugSerial.println(connectSuccess);

        if (connectSuccess) {
            debugSerial.println("SERVER CONNECTED!");
            serverConnected = true;
        }
    }
}