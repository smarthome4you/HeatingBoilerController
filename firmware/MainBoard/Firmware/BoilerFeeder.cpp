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
    if (  abs(millis() - this->startTime) > timeToEmergencyFeeder )
    {
      this->errorFeeder = true;
      this->isRunning = false;
      this->startStopping = false;
      this->off();
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
          this->relay->off();
        }
    }
}

void BoilerFeeder::on()
{
  if ( ! this->isRun() && ! this->errorFeeder) {
     this->startTime = millis();
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
}

bool BoilerFeeder::isRun()
{
  if ( this->isRunning == true || this->startStopping == true) return true;
  return false;
}

bool BoilerFeeder::isError()
{
  return this->errorFeeder;
}
