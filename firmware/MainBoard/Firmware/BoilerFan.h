#ifndef fan_h
#define fan_h

#include "Arduino.h"
#include "TimerOne.h"
#include "Config.h"
#include "RelaySSR.h"

int  fanPwmLength  = 10;
int  fanPwmCounter = 0;
int  fanPwmState   = LOW;
int  fanPwmPower   = 0;
int  fanPin = 0;
bool fanIsOn = false;
RelaySSR boilerFan(pinBoilerFan);


void FanPwmCallback()
{
  if ( fanIsOn ) {
    fanPwmCounter--;
    if (fanPwmCounter == 0){
      if ( fanPwmState == HIGH) 
      {
        fanPwmState = LOW; 
        fanPwmCounter = fanPwmLength - fanPwmPower;
        boilerFan.on();
      }
      else 
      {
        fanPwmState = HIGH;
        fanPwmCounter = fanPwmPower;
        boilerFan.off();
      }
      digitalWrite(LED_BUILTIN, fanPwmState);
    }
  }
}

void FanSetup()
{
   pinMode(LED_BUILTIN, OUTPUT);
   Timer1.initialize(10000);         // 1000000 - second
   Timer1.attachInterrupt(FanPwmCallback);
}

bool FanIsOn()
{
  return fanIsOn;
}

void FanSetPower(int power)
{
  if ( power < 1) power = 1;
  if ( power > 10) power = 10;
  
  fanPwmPower = power;
  fanPwmCounter = power;
}

void FanOn()
{
  if ( fanIsOn ) return;
  fanIsOn = true;
  digitalWrite(LED_BUILTIN, HIGH);
}

void FanOff()
{
  if ( ! fanIsOn ) return;
  fanIsOn = false;
  boilerFan.off();
  digitalWrite(LED_BUILTIN, LOW);
}

#endif