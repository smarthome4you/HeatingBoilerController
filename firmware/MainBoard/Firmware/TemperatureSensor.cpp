#include <OneWire.h>
#include <DallasTemperature.h>
#include "TemperatureSensor.h"
#include "Config.h"

Temperature::Temperature(int pin)
{
  this->oneWire = new OneWire(pin);
  this->sensor.setOneWire(oneWire);
  this->sensor.begin();
  this->tempCursor = 0;
  this->time = tempInterval + 1;
}

float Temperature::get()
{
  if ( ! (abs(millis() - this->time) > tempInterval)) return currTemp;
  this->time = millis();
  this->sensor.requestTemperatures();
  temp[this->tempCursor] = this->sensor.getTempCByIndex(0);
  this->currTemp = 0;
  for (byte pos = 0; pos < 5; pos++) {
    if( temp[pos] == -1000) temp[pos] = temp[pos -1];
    this->currTemp += temp[pos];
  }
  this->currTemp = this->currTemp / 5;
  if( this->tempCursor < 4) this->tempCursor++; else this->tempCursor = 0;
  return currTemp;
}


int Temperature::getAsInt()
{
  return (int) (this->get()*100);
}
