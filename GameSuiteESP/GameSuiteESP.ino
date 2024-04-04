#include <ESP8266WiFi.h>

// Client to ESP codes
#define SET_NETWORK 0

// ESP to client codes
#define NETWORK_ACK 0
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

    // Serial.print("connecting to ");
    // Serial.print(host);
    // Serial.print(':');
    // Serial.println(port);

    // // Use WiFiClient class to create TCP connections
    // WiFiClient client;
    // if (!client.connect(host, port)) {
    //     Serial.println("connection failed");
    //     delay(5000);
    //     return;
    // }

    // // This will send a string to the server
    // Serial.println("sending data to server");
    // if (client.connected()) { client.println("Hey!"); }

    // // wait for data to be available
    // unsigned long timeout = millis();
    // while (client.available() == 0) {
    //     if (millis() - timeout > 5000) {
    //         Serial.println(">>> Client Timeout !");
    //         client.stop();
    //         delay(60000);
    //         return;
    //     }
    // }

    // // Read all the lines of the reply from server and print them to Serial
    // Serial.println("receiving from remote server");
    // // not testing 'client.connected()' since we do not need to send data here
    // while (client.available()) {
    //     char ch = static_cast<char>(client.read());
    //     Serial.print(ch);
    // }

    // // Close the connection
    // Serial.println();
    // Serial.println("closing connection");
    // client.stop();

    // if (wait) {
    //     delay(300000);  // execute once every 5 minutes, don't flood remote service
    // }
    // wait = true;
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
}

