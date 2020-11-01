#include "Arduino.h"
#include "HallSensor.h"

Hallotron::Hallotron(int pin)
{
  this->pin = pin;
  pinMode(this->pin, INPUT_PULLUP);
}

bool Hallotron::getHallState()
{
    if (digitalRead(this->pin) == HIGH) return false;
    return true;
}
