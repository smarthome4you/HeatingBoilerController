#include "Arduino.h"
#include "Config.h"
#include "BoilerFeeder.h"
#include "HallSensor.h"
#include "RelaySSR.h"

BoilerFeeder::BoilerFeeder(int ssrPin, int hallPin)
{
  this->ssrPin = ssrPin;
  this->hallPin = hallPin;
  this->relay = new RelaySSR(this->ssrPin);
  this->hall = new Hallotron(this->hallPin);
}


void BoilerFeeder::process()
{
    if (  this->startTime > 0 && millis() - this->lastStartTime() > timeToEmergencyFeeder )
    {
      this->errorFeeder = true;
      this->isRunning = false;
      this->startStopping = false;
      this->off();
      this->relay->forceOff();
    }
    if ( this->isRunning ) {
      bool hallState = this->hall->getHallState();
      if (hallState != this->lastHallState)
      {
        this->counterHallState--;
        this->lastHallState = hallState;
      }
      if ( this->counterHallState == 0)
      {
        this->startStopping = true;
        this->isRunning = false;
      }
    }
    if ( this->startStopping )
    {
        if (  abs(millis() - this->startTime) > timeToStopFeeder ) 
        {
          this->startStopping = false;
          this->startTime = 0;
          this->lastRun = millis();
          this->relay->off();
        }
    }
}


void BoilerFeeder::shutDown()
{
  this->errorFeeder = true;
}

void BoilerFeeder::on()
{
  if ( this->interval > 0 && millis() - this->lastRunTime() < this->interval) return;
  if ( ! this->isRun() && ! this->errorFeeder) {
     this->startTime = millis();
     this->lastRun = millis();
     this->relay->on();
     this->lastHallState = this->hall->getHallState();
     this->isRunning = true;
     this->startStopping = false;
     if (this->lastHallState == false) this->counterHallState = 2; // Hallotron nie załączony musi zmienić stan na wysoki i później na niski dla pełnego obrotu podajnika (standardowa poprawna sytuacja) 
     else this->counterHallState = 3; // Hallotron ma stan wysoki musi zmienić się na niski, wysoki i ponownie niski dla pełnego obrotu podajnika (sytuacja gdy podajnik zatrzymany w niewłaściwym miejscu)
  }
}

void BoilerFeeder::off()
{
     this->relay->off();
     this->isRunning = false;
     this->startTime = 0;
}

bool BoilerFeeder::isRun()
{
  if ( this->isRunning == true || this->startStopping == true) return true;
  return false;
}


unsigned long BoilerFeeder::lastStartTime()
{
  if ( millis() < this->startTime ) this->startTime = millis(); 
  return this->startTime;
}

unsigned long BoilerFeeder::lastRunTime()
{
  if ( millis() < this->lastRun ) this->lastRun = millis(); 
  return this->lastRun;
}

bool BoilerFeeder::isError()
{
  return this->errorFeeder;
}

void BoilerFeeder::setRunInterval(unsigned long interval)
{
  this->interval = interval;
}
