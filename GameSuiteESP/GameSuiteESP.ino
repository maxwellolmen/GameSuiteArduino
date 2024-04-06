#include <ESP8266WiFi.h>

// Client to ESP codes
#define SET_NETWORK 0
#define SERVER_REQUEST 1

// ESP to client codes
#define NETWORK_ACK 0
#define SERVER_RESPONSE 1
#define SERVER_ACK 2

// ESP to client static lengths
#define NETWORK_ACK_LEN 1

#define HOST "maxwellolmen.com"
#define PORT 1337

WiFiClient client;

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);

    char init[] = "\0\0\0\0\0\0\0\0";
    Serial.write(init, 8);

    // Serial.println(F("Receiver started! Waiting for commands..."));
}

void loop() {
    if (Serial.available() >= 2) {
        byte command = Serial.read();
        byte length = Serial.read();

        while (Serial.available() < length) {
            delay(10);
        }

        char data[length];
        Serial.readBytes(data, length);

        processCommand(command, length, data);
    }

    if (client.available()) {
        byte command = client.read();
        byte length = client.read();

        byte data[length];
        client.readBytes(data, length);

        Serial.write(SERVER_RESPONSE);
        Serial.write(length);
        Serial.write(data, length);
    }
}

void processCommand(byte command, byte length, char* data) {
    switch (command) {
        case SET_NETWORK:
            char ssid[33];
            memcpy(ssid, data, 32);
            ssid[32] = '\0';

            char pass[65];
            memcpy(pass, data + 32, 64);
            pass[64] = '\0';

            /*Serial.print(F("SSID: "));
            Serial.println(ssid);
            Serial.print(F("Passkey: "));
            Serial.println(pass);*/

            connectWifi(ssid, pass);
            break;
        case SERVER_REQUEST:
            client.write(data, length);
            break;
    }
}

char* parseString(char* buffer, byte length) {
    char str[length + 1];

    int i;
    for (i = 0; i < length; i++) {
        str[i] = buffer[i];
    }

    str[i + 1] = '\0';
    return str;
}

void connectWifi(char* ssid, char* pass) {
    WiFi.begin(ssid, pass);

    /*Serial.print(F("Connecting to "));
    Serial.print(ssid);
    Serial.print(F("..."));*/

    long startTime = millis();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        if (millis() - startTime >= 5000) {
            byte response[] = {NETWORK_ACK, NETWORK_ACK_LEN, 0};
            Serial.write(response, 3);
            return;
        }
        // Serial.print(F("."));
    }

    byte response[] = {NETWORK_ACK, NETWORK_ACK_LEN, 1};
    Serial.write(response, 3);

    /*Serial.print(F("\n"));
    Serial.println(F("WiFi connected"));
    Serial.println(F("IP address: "));
    Serial.println(WiFi.localIP());*/

    client.connect(HOST, PORT);

    while (!client.connected()) {
        delay(50);
    }
}