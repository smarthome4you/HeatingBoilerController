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
#include "FloorHeating.h"


bool globalError              = false;
int  tempHysteresis           = 3;        // Histereza 
int  currentTargetTemperature = 55;       // Temperatura docelowa na kotle
int  targetWaterTemperature   = 60;       // Temperatura wody użytkowej
bool startHeating             = false;    // Wlaczenie procesu dogrzewania
int fanSpeed                  = 5;        // Poczatkowa sila nadmuchu
unsigned long lastHeatingTime = 0;


//Temperature tempSensorBoilerIn(25);
//Temperature tempSensorBoilerOut(25);
//Temperature tempSensorWater(25);

Temperature tempSensorBoilerIn(pinTempBoilerIn);
Temperature tempSensorBoilerOut(pinTempBoilerOut);
Temperature tempSensorWater(pinTempWater);

RelaySSR boilerMainPump(pinBoilerCentralHeatingPump); 
RelaySSR boilerWaterPump(pinBoilerWaterPump);
//RelaySSR boilerFloorPump(pinBoilerFloorPump);
BoilerFeeder boilerFeeder(pinBoilerFeeder, pinBoilerFeederHall);
FloorHeating floorHeatingPump(pinBoilerFloorPump);


EasyNex myNex(Serial1);


void setup() {
  // Enable watchdog
  wdt_enable(WDTO_4S);
  
  Serial.begin(115200);
  myNex.begin(9600);
  myNex.writeStr("page main");

  tempSensorBoilerIn.init(0);
  tempSensorBoilerOut.init(500);
  tempSensorWater.init(500);
  
  FanSetup();
  
  tone(pinBuzzer, 3500);
  delay(500);
  noTone(pinBuzzer);
  // Starting main pump for prevents discrepancies read signal from temperature sensors.
  boilerMainPump.on();
  boilerWaterPump.off();
  floorHeatingPump.setHeatingPeriods(5, 240); // 5 minute every 4 hours if floor pump is not setting
  floorHeatingPump.pumpOff();
  FanOff();
}

unsigned long timeReadFanSpeed = millis();
int getFanSpeed()
{
  if ( ! (abs(millis() - timeReadFanSpeed) > timeToReadFanSpeed )) return fanSpeed;
  fanSpeed = myNex.readNumber("speedFan.val");
  timeReadFanSpeed = millis();
  return fanSpeed;
}

unsigned long timeMainScreen = millis();
void updateMainScreen(float tempBoilerIn, float tempBoilerOut, float tempBoilerWater, int countOfErrorIn, int countOfErrorOut, int countOfErrorWater, BoilerFeeder *boilerFeeder, RelaySSR *boilerMainPump, RelaySSR *boilerWaterPump, FloorHeating *floorHeatingPump)
{
  if ( ! (abs(millis() - timeMainScreen) > timeToUpdatescreen )) return;

  if ( boilerFeeder->hall->getHallState()) myNex.writeNum("ledHall.val",   1);                else myNex.writeNum("ledHall.val",      0);
  if ( boilerFeeder->isRun())              myNex.writeNum("ledFeeder.val", 1);                else myNex.writeNum("ledFeeder.val",    0);

  if ( boilerMainPump->isOn())  myNex.writeNum("ledBoilerPump.val", 1); else myNex.writeNum("ledBoilerPump.val", 0);
  if ( boilerWaterPump->isOn()) myNex.writeNum("ledWaterPump.val",  1); else myNex.writeNum("ledWaterPump.val",  0);
  if ( floorHeatingPump->isOn()) myNex.writeNum("ledFloorPump.val",  1); else myNex.writeNum("ledFloorPump.val",  0);

  if ( FanIsOn()) myNex.writeNum("ledBoilerFan.val", 1); else myNex.writeNum("ledBoilerFan.val", 0);

  myNex.writeNum("tempBoiler1.val", (int)(tempBoilerIn * 100));
  myNex.writeNum("tempBoiler2.val", (int)(tempBoilerOut * 100));
  myNex.writeNum("tempWater.val",   (int)(tempBoilerWater * 100));

  int onFloorHeatMin = myNex.readNumber("floorHeatMin.val");
  int onFloorHeatMid = myNex.readNumber("floorHeatMid.val");
  int onFloorHeatMax = myNex.readNumber("floorHeatMax.val");

  if      (onFloorHeatMin == 1) floorHeatingPump->setHeatingPeriods(20, 220);// 2h
  else if (onFloorHeatMid == 1) floorHeatingPump->setHeatingPeriods(20, 140);// 3h
  else if (onFloorHeatMax == 1) floorHeatingPump->setHeatingPeriods(20, 100);// 4h
  else floorHeatingPump->setHeatingPeriods(5, 240); //Prevent to overheat boiler
 
  //Temperature sensor error
  myNex.writeNum("errIn.val",    countOfErrorIn);
  myNex.writeNum("errOut.val",   countOfErrorOut);
  myNex.writeNum("errWater.val", countOfErrorWater);

  timeMainScreen = millis();
}

void updateTargetTemperature()
{
  if ( ! (abs(millis() - timeMainScreen) > timeToUpdatescreen )) return;
  int temp = myNex.readNumber("setBoilerTemp.val");
  if ( temp > 40 && temp < 75 ) currentTargetTemperature = temp;
}

void checkTemperatureRange(float tempIn, bool tempInError, float tempOut, bool tempOutError, float tempBoilerWater, bool tempWaterError)
{
  if ( (abs(abs(tempIn) - abs(tempOut)) > 20) || tempIn > 70 || tempOut > 70 || tempIn < 0 || tempOut < 0 || tempInError == true || tempOutError == true )
  {
    globalError = true;
    boilerFeeder.shutDown();
    FanShutDown();
    boilerWaterPump.on();
    boilerMainPump.on();
    floorHeatingPump.forcePumpOn();
    tone(pinBuzzer, 3500);
    if (myNex.currentPageId != 3) {
       myNex.writeStr("page error");
       myNex.writeStr("errmessage.txt", "Przekroczona temperatura na czujnikach!!!");
       myNex.writeNum("errorTempIn.val", (int) (tempIn * 100));
       myNex.writeNum("errorTempOut.val", (int) (tempOut * 100));
       myNex.writeNum("errorTempWater.val", (int) (tempBoilerWater * 100));
       if (tempInError==true)     myNex.writeStr("infoErrIn.txt", "Error");    else myNex.writeStr("infoErrIn.txt", "OK");
       if (tempOutError==true)    myNex.writeStr("infoErrOut.txt", "Error");   else myNex.writeStr("infoErrOut.txt", "OK");
       if (tempWaterError==true)  myNex.writeStr("infoErrWater.txt", "Error"); else myNex.writeStr("infoErrWater.txt", "OK");
    }
  }
  else if ( boilerFeeder.isError() )
  {
    globalError = true;
    boilerFeeder.shutDown();
    FanShutDown();
    boilerWaterPump.on();
    boilerMainPump.on();
    floorHeatingPump.forcePumpOn();
    tone(pinBuzzer, 3500);
    if (myNex.currentPageId != 3) {
       myNex.writeStr("page error");
       myNex.writeStr("errmessage.txt", "Problem z podajnikiem!!!");
       myNex.writeNum("errorTempIn.val", (int) (tempIn * 100));
       myNex.writeNum("errorTempOut.val", (int) (tempOut * 100));
       myNex.writeNum("errorTempWater.val", (int) (tempBoilerWater * 100));
       if (tempInError==true)     myNex.writeStr("infoErrIn.txt", "Error");    else myNex.writeStr("infoErrIn.txt", "OK");
       if (tempOutError==true)    myNex.writeStr("infoErrOut.txt", "Error");   else myNex.writeStr("infoErrOut.txt", "OK");
       if (tempWaterError==true)  myNex.writeStr("infoErrWater.txt", "Error"); else myNex.writeStr("infoErrWater.txt", "OK");
    }
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
void loop_test() {
  wdt_reset();
  FanOn();
  FanSetSpeed(5);
  Serial.println(fanPwmSpeed);
}

void loop() {
    // reset watchdog counter
    wdt_reset();
    
    // update temperature
    float tempBoilerIn    = tempSensorBoilerIn.get();
    float tempBoilerOut   = tempSensorBoilerOut.get();
    float tempBoilerWater = tempSensorWater.get();
    int countOfErrorIn    = tempSensorBoilerIn.countOfError();
    int countOfErrorOut   = tempSensorBoilerOut.countOfError();
    int countOfErrorWater = tempSensorWater.countOfError();

    // Process heating
    updateTargetTemperature();
    checkTemperatureRange(tempBoilerIn, tempSensorBoilerIn.isError(), tempBoilerOut, tempSensorBoilerOut.isError(), tempBoilerWater, tempSensorWater.isError());
    boilerFeeder.process();
    floorHeatingPump.process();

    // Ciepła woda użytkowa
    if ( tempBoilerWater < targetWaterTemperature && tempBoilerWater < tempBoilerIn) {
      boilerWaterPump.on();
      boilerWaterPump.lockOn(30UL * 60UL * 1000UL); // blokada wlączenia na 30 minut 
    } else {
      boilerWaterPump.off();
    }

    // Boiler
    if ( tempBoilerIn < (currentTargetTemperature - tempHysteresis) || startHeating)
    {
      boilerMainPump.on();
      boilerMainPump.forceOn();
      boilerFeeder.on();
      boilerFeeder.setRunInterval(180UL * 1000ULL);
      FanOn();
      FanSetSpeed(getFanSpeed());
      FanLockOn(15UL * 1000UL);
      startHeating = true;
      lastHeatingTime = millis();
    } else if (isTimeToKeepFire()) {
      // Przedmuch co 40 minut
      boilerFeeder.on();
      boilerFeeder.setRunInterval(180UL * 1000ULL);
      FanOn();
      FanLockOn(120UL * 1000UL);
      lastHeatingTime = millis();
    } else if ( FanIsLockOff() ){
      FanOff();
    } 

    if (tempBoilerIn >= currentTargetTemperature){
      FanOff();
      startHeating = false;
    }
    
    // Process screen
    myNex.NextionListen();
    if ( myNex.currentPageId == 0 ) updateMainScreen(tempBoilerIn, tempBoilerOut, tempBoilerWater, countOfErrorIn, countOfErrorOut, countOfErrorWater, &boilerFeeder, &boilerMainPump, &boilerWaterPump, &floorHeatingPump);
  }
