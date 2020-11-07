#ifndef fan_h
#define fan_h

#include "Arduino.h"
#include "TimerOne.h"
#include "Config.h"
#include "RelaySSR.h"

int  fanPwmLength  = 10;
int  fanPwmCounter = 0;
int  fanPwmState   = LOW;
int  fanPwmSpeed   = 0;
bool fanIsOn = false;
RelaySSR boilerFan(pinBoilerFan);


void FanPwmCallback()
{
  if ( fanIsOn ) {
    fanPwmCounter--;
    if (fanPwmCounter == 0){
      if ( fanPwmState == HIGH) 
      {
        fanPwmCounter = fanPwmLength - fanPwmSpeed;
        if (fanPwmCounter > 0 ) 
        {
          fanPwmState = LOW; 
          boilerFan.off();
        }
      }
      else 
      {
        fanPwmState = HIGH;
        fanPwmCounter = fanPwmSpeed;
        boilerFan.on();
      }
      digitalWrite(LED_BUILTIN, fanPwmState);
    }
  }
}

void FanSetup()
{
   pinMode(LED_BUILTIN, OUTPUT);
   Timer1.initialize(10000);         // 1_000_000 - second
   Timer1.attachInterrupt(FanPwmCallback);
}

bool FanIsOn()
{
  return fanIsOn;
}

void FanSetSpeed(int fanSpeed)
{
  if ( fanSpeed < 2) fanSpeed = 2;
  if ( fanSpeed > 10) fanSpeed = 10;
  
  fanPwmSpeed = fanSpeed;
  fanPwmCounter = fanSpeed;
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
