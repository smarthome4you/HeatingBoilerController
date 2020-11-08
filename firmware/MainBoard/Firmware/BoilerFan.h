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
int  fanPowerMax   = 10;
bool fanIsOn = false;
bool fanShutDown = false;
unsigned long fanLastChangeState = 0;
unsigned long fanLockOnMillis = 0;
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
   digitalWrite(LED_BUILTIN, LOW);
}

bool FanIsOn()
{
  return fanIsOn;
}

unsigned long FanLastChangeState()
{
  if ( millis() < fanLastChangeState ) fanLastChangeState = millis();  
  return fanLastChangeState;
}

void FanSetSpeed(int fanSpeed)
{ 
  if ( (millis() - FanLastChangeState()) < 4 ) return;
  
  if ( fanSpeed < 2) fanSpeed = 2;
  if ( fanSpeed > 10) fanSpeed = 10;
  
  fanPwmSpeed = fanSpeed;
  fanPwmCounter = fanSpeed;
}

void FanOn()
{
  if ( fanIsOn == true || fanShutDown == true ) return;
  fanPwmSpeed = fanPowerMax;
  fanPwmCounter = fanPowerMax;
  fanLastChangeState = millis();
  fanIsOn = true;
  digitalWrite(LED_BUILTIN, HIGH);
}

void FanOff()
{
  if (fanLockOnMillis > 0 && millis() - FanLastChangeState() < fanLockOnMillis ) return;
  if ( ! fanIsOn ) return;
  fanLastChangeState = millis();
  fanIsOn = false;
  boilerFan.off();
  fanPwmSpeed = fanPowerMax;
  fanPwmCounter = fanPowerMax;
  fanLockOnMillis = 0;
  digitalWrite(LED_BUILTIN, LOW);
}

void FanShutDown()
{
  fanShutDown = true;
  digitalWrite(LED_BUILTIN, LOW);
}

void FanLockOn(unsigned long lockOnMillis)
{
  fanLockOnMillis = lockOnMillis;
}

#endif
