#ifndef THRUSTERS_H
#define THRUSTERS_H

#include "Arduino.h"

class Thruster {
    public:
        // Pin definitions
        byte pin1;
        byte pin2;

        Thruster(byte p1, byte p2);

        void begin();
        void stop();
        void move(int val);
       
};





#endif