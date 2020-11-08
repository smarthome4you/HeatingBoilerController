#include "Arduino.h"
#include "RelaySSR.h"
#include "Config.h"

RelaySSR::RelaySSR(int pin)
{
  this->pin = pin;
  pinMode(this->pin, OUTPUT);
}

void RelaySSR::on()
{
    if ( this->relayStateOn == true ) return;
    digitalWrite(this->pin, HIGH);
    this->relayStateOn = true;
    this->lastChangeState = millis();
    this->lockOnMilliSeconds = 0;
}

void RelaySSR::forceOn()
{
    digitalWrite(this->pin, HIGH);
    this->relayStateOn = true;
}

void RelaySSR::off()
{
    if ( this->relayStateOn == false ) return;   
    if ( this->lockOnMilliSeconds > 0 && ( millis() - this->lastChangeState < this->lockOnMilliSeconds)) return;
    
    digitalWrite(this->pin, LOW);
    this->relayStateOn = false;
    this->lastChangeState = millis();
    this->lockOnMilliSeconds = 0;
}

void RelaySSR::forceOff()
{
    digitalWrite(this->pin, LOW);
    this->relayStateOn = false;
    this->lastChangeState = millis();
    this->lockOnMilliSeconds = 0;
}

bool RelaySSR::isOn()
{
  return this->relayStateOn;
}

 unsigned long RelaySSR::lastChange()
 {
  if ( millis() < this->lastChangeState ) this->lastChangeState = millis(); 
  return this->lastChangeState;
 }

 void RelaySSR::lockOn(unsigned long milliSeconds)
 {
    if ( this->lockOnMilliSeconds == 0 ) this->lockOnMilliSeconds = milliSeconds;
 }
