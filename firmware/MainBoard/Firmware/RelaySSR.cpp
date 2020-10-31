#include "Arduino.h"
#include "RelaySSR.h"

RelaySSR::RelaySSR(int pin)
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
}

void RelaySSR::on()
{
    digitalWrite(this->pin, HIGH);
    this->relayState = true;
}

void RelaySSR::off()
{
    digitalWrite(this->pin, LOW);
    this->relayState = false;
}

bool RelaySSR::isOn()
{
  return this->relayState;
}
