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
  this->currTemp = -1000;
  this->time = tempInterval + 1;
  this->errorCounter = 3;
  this->iserror = false;
}

float Temperature::get()
{
  if ( ! (abs(millis() - this->time) > tempInterval)) return currTemp;
  this->time = millis();
  this->sensor.requestTemperatures();
  float temperature = this->sensor.getTempCByIndex(0);
  if ( this->currTemp > -1000 && (temperature == -127 || abs(this->currTemp - temperature) > 4 ))
  {
    errorCounter--;
    return this->currTemp;
  }
  
  if ( temperature > 0 ) {
    this->errorCounter = 3;
    temp[this->tempCursor] = temperature;
    this->currTemp = 0;
    for (byte pos = 0; pos < 5; pos++) {
      if( temp[pos] == -1000) temp[pos] = temperature;
      this->currTemp += temp[pos];
    }
  }
  this->currTemp = this->currTemp / 5;
  if( this->tempCursor < 4) this->tempCursor++; else this->tempCursor = 0;
  return this->currTemp;
}


bool Temperature::isError()
{
  if ( this->errorCounter <= 0) {
    this->iserror = true;
    this->errorCounter = 0;
  }
  return this->iserror;
}
