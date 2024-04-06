#include <EEPROM.h>

// ESP to client codes
#define NETWORK_ACK 0

// Client to ESP codes
#define SET_NETWORK 0
#define SET_NETWORK_LEN 96

#define debugSerial Serial
#define espSerial Serial1

struct WifiNetwork {
    char ssid[32];
    char pass[64];
};

long millisOnStart;
bool connected;
bool verified;

void setup() {
    debugSerial.begin(9600);
    espSerial.begin(115200);
    connected = false;
    verified = false;
    debugSerial.println(F("Started! Waiting for init ack from ESP..."));
    
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
        EEPROM.get(0, network);
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
    debugSerial.print(F("Sending SET_NETWORK to connect to SSID '"));
    debugSerial.print(ssid);
    debugSerial.println("'...");

    espSerial.write(SET_NETWORK);
    espSerial.write(SET_NETWORK_LEN);
    espSerial.write(ssid, 32);
    espSerial.write(pass, 64);
}

void processESP(byte command, byte length, char* data) {
    switch (command) {
        case NETWORK_ACK:
            bool success = data[0];

            if (!success) {
                debugSerial.println(F("Network connection failed! Trying again..."));
                connected = false;
                verified = false;
            } else {
                debugSerial.println(F("Network connection successful!"));
                verified = true;
            }
            break;
    }
}