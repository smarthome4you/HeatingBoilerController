#include <OneWire.h>
#include <DallasTemperature.h>
#include "TemperatureSensor.h"
#include "Config.h"


Temperature::Temperature(int pin)
{
  this->pin = pin;
  this->tempCursor = 0;
  this->lastRead = tempInterval;
  this->errorCounter = 3;
  this->globalErrorCounter = 0;
  this->iserror = false;
}


void Temperature::init(int wait)
{
  delay(wait);
  this->oneWire = new OneWire(this->pin);
  this->sensor.setOneWire(oneWire);
  this->sensor.begin();
  this->sensor.requestTemperatures();
  this->currTemp = this->sensor.getTempCByIndex(0);
  this->lastRead = millis();
}


float Temperature::get()
{
  if ( ! (millis() - this->lastReadMillis() > tempInterval)) return currTemp;
  this->lastRead = millis();
  this->sensor.requestTemperatures();
  float temperature = this->sensor.getTempCByIndex(0);
  if ( temperature == -127 || abs(this->currTemp - temperature) > 5 )
  {
    this->oneWire->reset();
    errorCounter--;
    globalErrorCounter++;
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
    this->currTemp = -273;
  }
  return this->iserror;
}


unsigned long Temperature::lastReadMillis()
{
  if ( millis() < this->lastRead ) this->lastRead = millis(); 
  return this->lastRead;
}


int Temperature::countOfError()
{
  return this->globalErrorCounter;
}
