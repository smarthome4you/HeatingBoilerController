#include <Wire.h>
#include <math.h>
#include <avr/wdt.h>
#include "Config.h"
#include "EasyNextionLibrary.h" //https://github.com/Seithan/EasyNextionLibrary
#include "TimerOne.h"
#include "TemperatureSensor.h"
#include "BoilerFeeder.h"
#include "RelaySSR.h"
#include "BoilerFan.h"


bool globalError              = false;
int  tempHysteresis           = 5;        // Histereza 
int  currentTargetTemperature = 55;       // Temperatura docelowa na kotle
int  targetWaterTemperature   = 50;       // Temperatura wody użytkowej
bool startHeating             = false;    // Wlaczenie procesu dogrzewania
unsigned long lastHeatingTime = 0;

Temperature tempSensorBoilerIn(pinTempBoilerIn);
Temperature tempSensorBoilerOut(pinTempBoilerOut);
Temperature tempSensorWater(pinTempWater);

RelaySSR boilerMainPump(pinBoilerCentralHeatingPump);
RelaySSR boilerWaterPump(pinBoilerWaterPump);
RelaySSR boilerFloorPump(pinBoilerFloorPump);
BoilerFeeder boilerFeeder(pinBoilerFeeder, pinBoilerFeederHall);

EasyNex myNex(Serial1);


void setup() {
  // Enable watchdog
  wdt_enable(WDTO_4S);
  
  Serial.begin(115200);
  myNex.begin(9600);
  myNex.writeStr("page main");
  FanSetup();
  
  tone(pinBuzzer, 3500, 1000);
  delay(500);
  noTone(pinBuzzer);
  // Starting main pump for prevents discrepancies read signal from temperature sensors.
  boilerMainPump.on();
  boilerWaterPump.off();
  boilerFloorPump.off();
  
}

unsigned long timeMainScreen = millis();
void updateMainScreen(int tempBoilerIn, int tempBoilerOut, int tempBoilerWater, BoilerFeeder *boilerFeeder, RelaySSR *boilerMainPump, RelaySSR *boilerWaterPump, RelaySSR *boilerFloorPump)
{
  if ( ! (abs(millis() - timeMainScreen) > timeToUpdatescreen )) return;

  if ( boilerFeeder->hall->getHallState()) myNex.writeNum("ledHall.val",   1);                else myNex.writeNum("ledHall.val",      0);
  if ( boilerFeeder->isRun())              myNex.writeNum("ledFeeder.val", 1);                else myNex.writeNum("ledFeeder.val",    0);
  if ( boilerFeeder->isError())            myNex.writeStr("feederError.txt", "Feeder Error"); else myNex.writeStr("feederError.txt", "");

  if ( boilerMainPump->isOn())  myNex.writeNum("ledBoilerPump.val", 1); else myNex.writeNum("ledBoilerPump.val", 0);
  if ( boilerWaterPump->isOn()) myNex.writeNum("ledWaterPump.val",  1); else myNex.writeNum("ledWaterPump.val",  0);
  if ( boilerFloorPump->isOn()) myNex.writeNum("ledFloorPump.val",  1); else myNex.writeNum("ledFloorPump.val",  0);

  if ( FanIsOn()) myNex.writeNum("ledBoilerFan.val", 1); else myNex.writeNum("ledBoilerFan.val", 0);

  myNex.writeNum("tempBoiler1.val", tempBoilerIn);
  myNex.writeNum("tempBoiler2.val", tempBoilerOut);
  myNex.writeNum("tempWater.val",   tempBoilerWater);

  timeMainScreen = millis();
}

void updateManualScreen(BoilerFeeder *boilerFeeder, RelaySSR *boilerMainPump, RelaySSR *boilerWaterPump, RelaySSR *boilerFloorPump)
{
  if ( ! (abs(millis() - timeMainScreen) > timeToUpdatescreen )) return;

  int onBoilerFan    = myNex.readNumber("onBoilerFan.val");
  int speedBoilerFan = myNex.readNumber("fanSpeed.val");
  if (onBoilerFan == 1) {
    FanOn();
    FanSetSpeed(speedBoilerFan);
  } else FanOff();

  int onBoilerFeeder = myNex.readNumber("onBoilerFeeder.val");
  if (onBoilerFeeder == 1) boilerFeeder->on(); else boilerFeeder->off();

  int onBoilerPump = myNex.readNumber("onBoilerPump.val");
  if (onBoilerPump == 1) boilerMainPump->on(); else boilerMainPump->off();

  int onWaterPump = myNex.readNumber("onWaterPump.val");
  if (onWaterPump == 1) boilerWaterPump->on(); else boilerWaterPump->off();

  int onFloorPump = myNex.readNumber("onFloorPump.val");
  if (onFloorPump == 1) boilerFloorPump->on(); else boilerFloorPump->off();

  if ( boilerFeeder->hall->getHallState()) myNex.writeNum("ledBoilerHall.val", 1); else myNex.writeNum("ledBoilerHall.val", 0);

  timeMainScreen = millis();
}


void updateTargetTemperature()
{
  int temp = myNex.readNumber("setBoilerTemp.val");
  if ( temp > 40 && temp < 75 ) currentTargetTemperature = temp;
}

void checkTemperatureRange(int tempIn, int tempOut)
{
  if ( (abs(abs(tempIn) - abs(tempOut)) > 10) || tempIn > 70 || tempOut > 70)
  {
    globalError = true;
    boilerFeeder.shutDown();
    FanShutDown();
    boilerFloorPump.on();
    tone(pinBuzzer, 3500);
  }
}

bool isTimeToKeepFire()
{
  if ( millis() < lastHeatingTime ) {
    lastHeatingTime = millis();
    return true;
  }
  if ( millis() - lastHeatingTime > 40UL * 60UL * 1000UL) return true;
  return false;
}

//Dodac zabepieczenie przed zbyt dlugim rozpalaniem pieca bez efektu wzrostu temperatury

void loop() {
    // reset watchdog counter
    wdt_reset();

    // update temperature
    int tempBoilerIn    = tempSensorBoilerIn.getAsInt();
    int tempBoilerOut   = tempSensorBoilerOut.getAsInt();
    int tempBoilerWater = tempSensorWater.getAsInt();

    // Process heating
    updateTargetTemperature();
    checkTemperatureRange(tempBoilerIn, tempBoilerOut);
    boilerFeeder.process();

    // Ciepła woda użytkowa
    if ( tempBoilerWater < targetWaterTemperature && tempBoilerWater < tempBoilerIn) {
      boilerWaterPump.on();
      boilerWaterPump.lockOn(30UL * 60UL * 1000UL); // blokada wlączenia na 30 minut 
    } else {
      boilerWaterPump.off();
    }

    // Wylaczenie wentylatora
    FanOff();

    // Boiler
    if ( tempBoilerIn < (currentTargetTemperature - tempHysteresis) || startHeating)
    {
      boilerMainPump.on();
      boilerMainPump.forceOn();
      boilerFeeder.on();
      boilerFeeder.setRunInterval(180UL * 1000ULL);
      FanOn();
      FanLockOn(30UL * 1000UL);
      FanSetSpeed(5);
      startHeating = true;
      lastHeatingTime = millis();
    } 
    if (tempBoilerIn >= currentTargetTemperature){
      FanOff();
      startHeating = false;
    }

    // Przedmuch co 40 minut
    if (isTimeToKeepFire()) {
      boilerFeeder.on();
      boilerFeeder.setRunInterval(180UL * 1000ULL);
      FanOn();
      FanLockOn(120UL * 1000UL);
      lastHeatingTime = millis();
    }

    // Process screen
    myNex.NextionListen();
    if      ( myNex.currentPageId == 0 ) updateMainScreen(tempBoilerIn, tempBoilerOut, tempBoilerWater, &boilerFeeder, &boilerMainPump, &boilerWaterPump, &boilerFloorPump);
    else if ( myNex.currentPageId == 1 ) updateManualScreen(&boilerFeeder, &boilerMainPump, &boilerWaterPump, &boilerFloorPump);
  }
