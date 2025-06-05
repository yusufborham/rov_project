#include <EtherCard.h>
#include <IPAddress.h>

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
#define slope_temperature 0.48828125 // Example slope for temperature conversion
#define intercept_temperature -50.0 // Example intercept for temperature conversion


Adafruit_BMP280 bmp; // I2C

Thruster LeftThruster(THRUSTER1_PIN1, THRUSTER1_PIN2);      // pointed up to push up and down
Thruster RightThruster(THRUSTER2_PIN1, THRUSTER2_PIN2);     // pointed up to push up and down
Thruster BackThruster(THRUSTER3_PIN1, THRUSTER3_PIN2);      // pointed back to push forward and bacward


// MAC address for ENC28J60 – must be unique
static byte mymac[] = { 0x70, 0x69, 0x69, 0x2D, 0x30, 0x31 };

// Define static IP if not using DHCP
static byte myip[] = { 192, 168, 1, 200 };
static byte gwip[] = { 192, 168, 1, 3 };

byte Ethernet::buffer[700];  // Network buffer
BufferFiller bfill;

IPAddress remoteIP(192, 168, 1, 3); // Your laptop IP
uint16_t remotePort = 6000;          // Port to send data to

float temperature = 25.4;
float pressure = 101325.7;

float readTemperature() {
    // Read temperature from the sensor
    // This is a placeholder function, implement actual reading logic
    float temperature = (float)analogRead(TemperatureSensorPin);
    temperature = slope_temperature * temperature + intercept_temperature; // Convert to Celsius if needed
    Serial.print("Temperature: ");
    Serial.println(temperature);
    return temperature;
}

void sendSensorData() {
  char buffer[80];
  char tempStr[10], pressureStr[15];

  dtostrf(temperature, 6, 2, tempStr);  // width=6, precision=2
  dtostrf(pressure, 9, 2, pressureStr); // width=9, precision=2

  sprintf(buffer, "Temp is %s, pressure %s", tempStr, pressureStr);
  
  // Extract IP bytes from IPAddress object
  uint8_t remote_ip[4] = {
    remoteIP[0],
    remoteIP[1],
    remoteIP[2],
    remoteIP[3]
  };

    ether.sendUdp(buffer, strlen(buffer), 5000, remote_ip, remotePort);

    Serial.print("Sending to ");
    Serial.print(remoteIP[0]);
    Serial.print(".");
    Serial.print(remoteIP[1]);
    Serial.print(".");
    Serial.print(remoteIP[2]);
    Serial.print(".");
    Serial.print(remoteIP[3]);
    Serial.print(":");
    Serial.print(remotePort);
    Serial.print(" - ");
    Serial.print("Buffer: ");


  Serial.print("Sent UDP: ");
  Serial.println(buffer);
}


// Callback for received UDP messages
void udpReceive(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len) {
  Serial.println("Received UDP packet:");
  Serial.write(data, len);
  Serial.println();

  // Example: control logic based on incoming char
  char command = data[0];
  int val1 = 0, val2 = 0 , val3 = 0;
  switch (command) {
    case 'F': 
        Serial.println("Forward"); 
        val1 = 1 ; 
        val2 = 0 ;
        val3 = 0 ;
        break;
    case 'B': 
        Serial.println("Backward"); 
        val1 = -1 ; 
        val2 = 0 ;
        val3 = 0 ;
        break;
    case 'L':  
        Serial.println("ROLL Left"); 
        val1 = 0 ;
        val2 = 1 ;
        val3 = 0 ;
        break;
    case 'R': 
        Serial.println("ROLL Right"); 
        val1 = 0 ;
        val2 = 0 ;
        val3 = 1 ;
        break;
    case 'U':   
        Serial.println("Up"); 
        val1 = 0 ;
        val2 = 1 ;
        val3 = 1 ;
        break;
    case 'D': 
        Serial.println("Down"); 
        val1 = 0 ;
        val2 = -1 ;
        val3 = -1 ;
        break;
    case 'S': 
        Serial.println("Stop"); 
        LeftThruster.stop();
        RightThruster.stop();
        BackThruster.stop();
        break;
    default:    
        Serial.println("Unknown Command");
  }

    LeftThruster.move(val2);
    RightThruster.move(val3);
    BackThruster.move(val1);

 
}

void setup() {
  Serial.begin(57600);
  Serial.println("\n[ENC28J60 ROV Demo]");

  if (ether.begin(sizeof Ethernet::buffer, mymac, SS) == 0) {
    Serial.println(F("Failed to access Ethernet controller"));
    while (true);
  }

  ether.staticSetup(myip, gwip);

  ether.printIp("IP Address: ", ether.myip);
  ether.printIp("Gateway IP: ", ether.gwip);

  ether.udpServerListenOnPort(&udpReceive, 1337);  // Listen on port 1337

  bmp.begin();

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

    LeftThruster.begin();
    RightThruster.begin();
    BackThruster.begin();

    LeftThruster.stop();
    RightThruster.stop();
    BackThruster.stop();
}
unsigned long lastSendTime = 0;

void loop() {
    ether.packetLoop(ether.packetReceive());
        // Check for incoming UDP packets

    if (millis() - lastSendTime > 100) {  // 100ms passed
        sendSensorData();
        lastSendTime = millis();
    }

}



