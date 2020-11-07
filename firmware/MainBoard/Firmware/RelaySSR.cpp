#include "Arduino.h"
#include "RelaySSR.h"

RelaySSR::RelaySSR(int pin)
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
}

void RelaySSR::on()
{
    if ( this->relayState == true ) return;
    digitalWrite(this->pin, HIGH);
    this->relayState = true;
    this->lastChangeState = millis();
}

void RelaySSR::off()
{
    if ( this->relayState == false ) return;
    digitalWrite(this->pin, LOW);
    this->relayState = false;
    this->lastChangeState = millis();
}

bool RelaySSR::isOn()
{
  return this->relayState;
}

 unsigned long RelaySSR::lastChange()
 {
  return this->lastChangeState;
 }
