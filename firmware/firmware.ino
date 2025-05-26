#include <SPI.h>
#include <Ethernet.h>
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


// MAC address of the W5500 (can be random but unique)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);  // Example static IP

EthernetServer server(80);  // Listen on port 80


char data ;

float readTemperature() {
    // Read temperature from the sensor
    // This is a placeholder function, implement actual reading logic
    float temperature = (float)analogRead(TemperatureSensorPin);
    temperature = slope_temperature * temperature + intercept_temperature; // Convert to Celsius if needed
    Serial.print("Temperature: ");
    Serial.println(temperature);
    return temperature;
}

void setup() {
    Ethernet.begin(mac, ip);
    server.begin();

    Serial.begin(9600);
    while ( !Serial ) delay(100);   // wait for native usb

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

void loop() {
    EthernetClient client = server.available();
    if (client) {

        if (client.available()) {
        data = client.read();

        int val1 = 0, val2 = 0 ;      

        if (data == 'F'){
            val1 = 1; // Move  Thruster forward
            val2 = 0;
        }
        else if (data == 'B') {
            val1 = -1; // Move Thruster backward
            val2 = 0;
        }
        else if (data == 'U') {
            val1 = 0; // Move Left Thruster
            val2 = 1;
        }
        else if (data == 'D') {
            val1 = 0; // Move Right Thruster
            val2 = -1;
        }
        else if (data == 'S') {
            LeftThruster.stop();
            RightThruster.stop();
            BackThruster.stop();
        }
        else {
            LeftThruster.stop();
            RightThruster.stop();
            BackThruster.stop();
        }
        
        LeftThruster.move(val2);
        RightThruster.move(val2);
        BackThruster.move(val1);
        
        
        // Handle commands here
        }

        Serial.print(F("Received command: "));
        Serial.println(data);

        // Print the pressure and temperature
        Serial.print(F("Pressure = "));
        Serial.print(bmp.readPressure());
        Serial.println(" Pa");

        Serial.print(F("Temperature = "));
        Serial.print(readTemperature());
        Serial.println(" C");

        // Prepare the message to send

        String message = String(readTemperature(), 1) + "," + String(bmp.readPressure(), 1) + "," + data;

        // Send it
        client.println(message);

        delay(100);
    }
}
