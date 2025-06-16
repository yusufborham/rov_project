
#include <UIPEthernet.h>
#include <UIPUdp.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include "Thruster.h"

#define THRUSTER1_PIN1 2
#define THRUSTER1_PIN2 3
#define THRUSTER2_PIN1 4
#define THRUSTER2_PIN2 5
#define THRUSTER3_PIN1 6
#define THRUSTER3_PIN2 7

#define TemperatureSensorPin A0
#define slope_temperature 0.48828125
#define intercept_temperature -50.0

Adafruit_BMP280 bmp; // I2C

Thruster LeftThruster(THRUSTER1_PIN1, THRUSTER1_PIN2);
Thruster RightThruster(THRUSTER2_PIN1, THRUSTER2_PIN2);
Thruster BackThruster(THRUSTER3_PIN1, THRUSTER3_PIN2);

// Networking config
byte mac[] = { 0x70, 0x69, 0x69, 0x2D, 0x30, 0x31 };
IPAddress ip(192, 168, 1, 200);
IPAddress remoteIP(192, 168, 1, 3);
uint16_t remotePort = 6000;

EthernetUDP Udp;

float temperature = 25.4;
float pressure = 1325.7;

float readTemperature() {
    float temperature = analogRead(TemperatureSensorPin);
    temperature = slope_temperature * temperature + intercept_temperature;
    Serial.print("Temperature: ");
    Serial.println(temperature);
    return temperature;
}

void sendSensorData() {
    char buffer[80];
    char tempStr[10], pressureStr[15];
    dtostrf(temperature, 0, 2, tempStr);
    dtostrf(pressure, 0, 2, pressureStr);

    sprintf(buffer, "Temp is %s,pressure%s", tempStr, pressureStr);

    Udp.beginPacket(remoteIP, remotePort);
    Udp.write(buffer);
    Udp.endPacket();

    Serial.print("Sent UDP: ");
    Serial.println(buffer);
}

void udpReceive() {
    int packetSize = Udp.parsePacket();
    if (packetSize) {
        char packetBuffer[50];
        int len = Udp.read(packetBuffer, 50);
        if (len > 0) packetBuffer[len] = '\0';

        Serial.print("Received UDP: ");
        Serial.println(packetBuffer);

        char command = packetBuffer[0];
        int val1 = 0, val2 = 0, val3 = 0;

        switch (command) {
            case 'F': Serial.println("Forward"); val1 = 1; break;
            case 'B': Serial.println("Backward"); val1 = -1; break;
            case 'L': Serial.println("ROLL Left"); val2 = 1; break;
            case 'R': Serial.println("ROLL Right"); val3 = 1; break;
            case 'U': Serial.println("Up"); val2 = 1; val3 = 1; break;
            case 'D': Serial.println("Down"); val2 = -1; val3 = -1; break;
            case 'S':
                Serial.println("Stop");
                LeftThruster.stop();
                RightThruster.stop();
                BackThruster.stop();
                return;
            default:
                Serial.println("Unknown Command");
                return;
        }

        LeftThruster.move(val2);
        RightThruster.move(val3);
        BackThruster.move(val1);
    }
}

void setup() {
    Serial.begin(57600);
    Serial.println("[ENC28J60 ROV - UIPEthernet Mode]");

    Ethernet.begin(mac, ip);
    Udp.begin(1337);  // listen on this port for control commands

    bmp.begin();
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_X2,
                    Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16,
                    Adafruit_BMP280::STANDBY_MS_500);

    LeftThruster.begin();
    RightThruster.begin();
    BackThruster.begin();
    LeftThruster.stop();
    RightThruster.stop();
    BackThruster.stop();
}

unsigned long lastSendTime = 0;

void loop() {
    udpReceive();  // check for incoming control messages

    if (millis() - lastSendTime > 100) {
        // temperature = readTemperature();
        // pressure = bmp.readPressure();
        temperature = 10 ;
        pressure = 20 ;
        sendSensorData();
        lastSendTime = millis();
    }
}
