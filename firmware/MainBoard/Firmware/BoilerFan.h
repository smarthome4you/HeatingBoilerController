#ifndef fan_h
#define fan_h

#include "Arduino.h"
#include "TimerOne.h"
#include "Config.h"
#include "RelaySSR.h"

int  fanPwmLength  = 10;
int  fanPwmCounter = 10;
int  fanPwmState   = LOW;
int  fanPwmSpeed   = 10;
int  fanPowerMax   = 10;
bool fanIsOn = false;
bool fanShutDown = false;
unsigned long fanLastOn = 0;
unsigned long fanLockOnMillis = 0;
RelaySSR boilerFan(pinBoilerFan);


bool FanIsOn()
{
  return fanIsOn;
}


void FanPwmCallback()
{
  if ( FanIsOn() && fanShutDown == false ) {
    fanPwmCounter--;
    if (fanPwmCounter <= 0){
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


unsigned long FanLastOn()
{
  if ( millis() < fanLastOn ) fanLastOn = millis();  
  return fanLastOn;
}


void FanSetSpeed(int fanSpeed)
{ 
  if ( (millis() - FanLastOn()) < 3000 ) return;
  
  if ( fanSpeed < 2) fanSpeed = 2;
  if ( fanSpeed > 10) fanSpeed = 10;
  
  fanPwmSpeed = fanSpeed;
}


void FanOn()
{
  if ( FanIsOn() == true || fanShutDown == true ) return;
  if ( millis() - FanLastOn() > 500UL )
  {
     fanPwmSpeed = fanPowerMax;
     fanPwmCounter = fanPowerMax;
  }
  fanLastOn = millis();
  fanIsOn = true;
}


void FanOff()
{
  if (fanLockOnMillis > 0 && millis() - FanLastOn() < fanLockOnMillis ) return;
  if ( fanIsOn == false ) return;
  fanIsOn = false;
  boilerFan.off();
  fanLockOnMillis = 0;;
}


bool FanIsLockOff()
{
  if (fanLockOnMillis > 0 && millis() - FanLastOn() < fanLockOnMillis ) return false;
  return true;
}


void FanShutDown()
{
  fanShutDown = true;
  fanIsOn = false;
  boilerFan.off();
  boilerFan.forceOff();
}


void FanLockOn(unsigned long lockOnMillis)
{
  fanLockOnMillis = lockOnMillis;
} 

#endif
