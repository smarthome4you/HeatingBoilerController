#include "Arduino.h"
#include "Config.h"
#include "FloorHeating.h"
#include "RelaySSR.h"



FloorHeating::FloorHeating(int ssrPin)
{
  this->ssrPin      = ssrPin;
  this->relay       = new RelaySSR(this->ssrPin);
  this->startTime   = millis();
  this->lastRun     = millis();
  this->onLength    = 5;
  this->offLength   = 240;
}

  
void FloorHeating::setHeatingPeriods(int onLength, int offLength)
{
  this->onLength  = ((unsigned long) onLength)  * 1000 * 60; //minute to millis
  this->offLength = ((unsigned long) offLength) * 1000 * 60; //minute to millis
}

void FloorHeating::process()
{
  if ((millis() - lastRun > this->offLength) && !this->isOn())
  {
    this->pumpOn();
    this->startTime = millis();
  }
  if ((millis() - startTime > this->onLength) && this->isOn())
  {
    this->pumpOff();
    this->lastRun = millis();
  }
}

void FloorHeating::pumpOn()
{
   this->relay->on();
}

void FloorHeating::forcePumpOn()
{
  this->lock = true;
  this->relay->on();
}

void FloorHeating::pumpOff()
{
  if(!this->lock)
  this->relay->off();
}

bool FloorHeating::isOn()
{
  return this->relay->isOn();
}
