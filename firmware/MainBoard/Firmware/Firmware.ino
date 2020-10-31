#include <Wire.h>
#include <math.h>

#include "config.h"
#include "EasyNextionLibrary.h" //https://github.com/Seithan/EasyNextionLibrary
#include "TemperatureSensor.h"
#include "BoilerFeeder.h"
#include "RelaySSR.h"

/*
 * Firmware dla kotła z podajnikiem szufladkowym
 * Podajnik zawsze musi wykonać pełny obrót!
 */


int temperatura [][3]  = {
                           {0, 0, 0},   // Temp CO punkt pomiaru 1 (czas ostatniego pomiaru, czas ostatniej zmiany temperatury, temp ostatniego pomiaru)
                           {0, 0, 0},   // Temp CO punkt pomiaru 2 (czas ostatniego pomiaru, czas ostatniej zmiany temperatury, temp ostatniego pomiaru)
                           {0, 0, 0},   // Temp CWU (czas ostatniego pomiaru, czas ostatniej zmiany temperatury, temp ostatniego pomiaru)
                         };

int dmuchawa [][1]     = {
                           {1}     // czas ostaniego załaczenia
                         };


int przekaznik [][1]   = {
                           {1},   // czas ostaniego załączenia pompa CO
                           {1},   // czas ostaniego załączenia pompa CWU
                           {1},   // czas ostaniego załączenia pompa ogrzewania podłogowego
                           {1}    // czas ostaniego załączenia podajnika
                         };



Temperature tempSensorBoilerIn(pinTempBoilerIn);
Temperature tempSensorBoilerOut(pinTempBoilerOut);
Temperature tempSensorWater(pinTempWater);

RelaySSR boilerMainPump(pinBoilerCentralHeatingPump);
RelaySSR boilerWaterPump(pinBoilerWaterPump);
RelaySSR boilerFloorPump(pinBoilerFloorPump);
RelaySSR boilerFan(pinBoilerFan);
BoilerFeeder boilerFeeder(pinBoilerFeeder, pinBoilerFeederHall);

EasyNex myNex(Serial1);  
  

void setup(){
  Serial.begin(115200);
  myNex.begin(9600);
  myNex.writeStr("page main");
  boilerFeeder.on();
}

unsigned long timeMainScreen = millis();
void updateMainScreen(int tempBoilerIn, int tempBoilerOut, int tempBoilerWater, BoilerFeeder *boilerFeeder, RelaySSR *boilerMainPump, RelaySSR *boilerWaterPump, RelaySSR *boilerFloorPump, RelaySSR *boilerFan)
{
  if ( ! (abs(millis() - timeMainScreen) > timeToUpdatescreen )) return;
  
  if ( boilerFeeder->isRun()) myNex.writeNum("ledFeeder.val", 1); else myNex.writeNum("ledFeeder.val", 0);
  if ( boilerFeeder->hall->getHallState()) myNex.writeNum("ledHall.val", 1); else myNex.writeNum("ledHall.val", 0);
  if ( boilerFeeder->isError()) myNex.writeStr("feederError.txt", "Feeder Error"); else myNex.writeStr("feederError.txt", "");

  if ( boilerMainPump->isOn()) myNex.writeNum("ledBoilerPump.val", 1); else myNex.writeNum("ledBoilerPump.val", 0);
  if ( boilerWaterPump->isOn()) myNex.writeNum("ledWaterPump.val", 1); else myNex.writeNum("ledWaterPump.val", 0);
  if ( boilerFloorPump->isOn()) myNex.writeNum("ledFloorPump.val", 1); else myNex.writeNum("ledFloorPump.val", 0);

  if ( boilerFan->isOn()) myNex.writeNum("ledBoilerFan.val", 1); else myNex.writeNum("ledBoilerFan.val", 0);

  myNex.writeNum("tempBoiler1.val", tempBoilerIn);
  myNex.writeNum("tempBoiler2.val", tempBoilerOut);
  myNex.writeNum("tempWater.val", tempBoilerWater);
  
  timeMainScreen = millis();
}

void loop(){
  myNex.NextionListen();
  boilerFeeder.process();
  
  int tempBoilerIn = tempSensorBoilerIn.getAsInt();
    
  updateMainScreen(tempBoilerIn, tempBoilerIn, tempBoilerIn, &boilerFeeder, &boilerMainPump, &boilerWaterPump, &boilerFloorPump, &boilerFan);
  //val = myNex.readNumber("setBoilerTemp.val");
  //myNex.writeStr("page 1");

    
}
