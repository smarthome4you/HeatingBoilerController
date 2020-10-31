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
RelaySSR boilerBlower(pinBoilerBlower);
BoilerFeeder boilerFeeder(pinBoilerFeeder, pinBoilerFeederHall);

EasyNex myNex(Serial1);  
  

void setup(){
  Serial.begin(115200);
  myNex.begin(9600);
  boilerFeeder.on();
}


void loop(){
  boilerFeeder.process();
  myNex.NextionListen();
  int tempBoilerIn = tempSensorBoilerIn.getAsInt();
  if ( boilerFeeder.isRun()) myNex.writeNum("ledFeeder.val", 1); else myNex.writeNum("ledFeeder.val", 0);
  if ( boilerFeeder.hall->getHallState()) myNex.writeNum("ledHall.val", 1); else myNex.writeNum("ledHall.val", 0);
  if ( boilerFeeder.isError()) myNex.writeStr("errorFeeder.txt", "Error");
  
  
  myNex.writeNum("tempBoiler1.val", tempBoilerIn);
  myNex.writeNum("tempBoiler2.val", tempBoilerIn);
  //val = myNex.readNumber("setBoilerTemp.val");
  //myNex.writeStr("page 1");

    
}
