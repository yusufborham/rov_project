#include "Thruster.h"

Thruster::Thruster(byte p1, byte p2) {
    
    pin1 = p1;
    pin2 = p2;
}

void Thruster::begin() {
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
}

void Thruster::stop() {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
}

void Thruster::move(int val) {
    // val can be -1, 0, or 1
    if (val == 0) {
        stop();
        return;
    }
    digitalWrite(pin1,(val == 1) ? HIGH : LOW);
    digitalWrite(pin2,(val == -1) ? HIGH : LOW);
}

