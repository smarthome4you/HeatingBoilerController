#include <Wire.h>
#include <math.h>

#include "EasyNextionLibrary.h" //https://github.com/Seithan/EasyNextionLibrary
#include "config.h"
#include "TemperatureSensor.h"


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


//OneWire wireTempCO_1(pinTempCO_1); 
//OneWire wireTempCO_2(pinTempCO_2); 
//OneWire wireTempCWU(pinTempCWU);  
//DallasTemperature sensorTempCO_1(&wireTempCO_1);
//DallasTemperature sensorTempCO_2(&wireTempCO_2);
//DallasTemperature sensorTempCWU(&wireTempCWU);

EasyNex nxScreen(Serial);


/*
float AktualizujTempCO_1() {
  bool newMeasurement = false;
  
  if (temperatura[czujnikTempCO_1][ostatniPomiarTemp] == 0) {
    temperatura[czujnikTempCO_1][ostatniPomiarTemp]  = millis(); 
    temperatura[czujnikTempCO_1][ostatniaZmianaTemp] = millis();
    newMeasurement = true;
  }

  // Czy upłynął wystarczający czas od ostatniego pomiaru aby wykonać pomiar ponownie
  if (  abs(millis() - temperatura[czujnikTempCO_1][ostatniPomiarTemp]) > okresPomiaruTemp) {
    newMeasurement = true;
  }
  
  if ( newMeasurement ) {
     sensorTempCO_1.begin();
     sensorTempCO_1.requestTemperatures();   
     float tempCO = sensorTempCO_1.getTempCByIndex(0);

     // Kiedy ostatnio zmieniła się temperatura o deltaTemp
     if ( abs(temperatura[czujnikTempCO_1][ostatniaTemp] - tempCO) > deltaTemp ) {
          temperatura[czujnikTempCO_1][ostatniaZmianaTemp] = millis();
     }
     temperatura[czujnikTempCO_1][ostatniaTemp] = tempCO;
     temperatura[czujnikTempCO_1][ostatniPomiarTemp] = millis(); 
  }
  
  return temperatura[czujnikTempCO_1][ostatniaTemp];
}

float AktualizujTempCO_2() {
  bool newMeasurement = false;
  
  if (temperatura[czujnikTempCO_2][ostatniPomiarTemp] == 0) {
    temperatura[czujnikTempCO_2][ostatniPomiarTemp]  = millis(); 
    temperatura[czujnikTempCO_2][ostatniaZmianaTemp] = millis();
    newMeasurement = true;
  }

  // Czy upłynął wystarczający czas od ostatniego pomiaru aby wykonać pomiar ponownie
  if (  abs(millis() - temperatura[czujnikTempCO_2][ostatniPomiarTemp]) > okresPomiaruTemp) {
    newMeasurement = true;
  }
  
  if ( newMeasurement ) {
     sensorTempCO_2.begin();
     sensorTempCO_2.requestTemperatures();   
     float tempCO = sensorTempCO_2.getTempCByIndex(0);

     // Kiedy ostatnio zmieniła się temperatura o deltaTemp
     if ( abs(temperatura[czujnikTempCO_2][ostatniaTemp] - tempCO) > deltaTemp ) {
          temperatura[czujnikTempCO_2][ostatniaZmianaTemp] = millis();
     }
     temperatura[czujnikTempCO_2][ostatniaTemp] = tempCO;
     temperatura[czujnikTempCO_2][ostatniPomiarTemp] = millis(); 
  }
  
  return temperatura[czujnikTempCO_2][ostatniaTemp];
}

float AktualizujTempCWU() {
  bool newMeasurement = false;
  
  if (temperatura[czujnikTempCWU][ostatniPomiarTemp] == 0) {
    temperatura[czujnikTempCWU][ostatniPomiarTemp]  = millis(); 
    temperatura[czujnikTempCWU][ostatniaZmianaTemp] = millis();
    newMeasurement = true;
  }

  // Czy upłynął wystarczający czas od ostatniego pomiaru aby wykonać pomiar ponownie
  if (  abs(millis() - temperatura[czujnikTempCWU][ostatniPomiarTemp]) > okresPomiaruTemp) {
    newMeasurement = true;
  }
  
  if ( newMeasurement ) {
     sensorTempCWU.begin();
     sensorTempCWU.requestTemperatures();   
     float tempCWU = sensorTempCWU.getTempCByIndex(0);

     // Kiedy ostatnio zmieniła się temperatura o deltaTemp
     if ( abs(temperatura[czujnikTempCWU][ostatniaTemp] - tempCWU) > deltaTemp ) {
          temperatura[czujnikTempCWU][ostatniaZmianaTemp] = millis();
     }
     temperatura[czujnikTempCWU][ostatniaTemp] = tempCWU;
     temperatura[czujnikTempCWU][ostatniPomiarTemp] = millis(); 
  }
  
  return temperatura[czujnikTempCWU][ostatniaTemp];
}*/

bool CheckList() {
  return true;
}



  Temperature co(25);
  Temperature co2(25);
  Temperature co3(25);

void setup(){
  Serial.begin(9600);


/*
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  analogReference(DEFAULT);
  pinMode(A7, INPUT_PULLUP);
  */
}

/*
void test_relay() {
  delay(2000); 
  digitalWrite(relay1, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000); 
  digitalWrite(relay2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000); 
  digitalWrite(relay3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000); 
  digitalWrite(relay4, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(10000);
  digitalWrite(relay1, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(relay2, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(relay3, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(relay4, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(10000);
}
*/


int val;
void loop(){
  Serial.print("Aktualna temperatura: ");
  Serial.println(co.get());
  delay(500);
    /*float CO_1 = AktualizujTempCO_1();
    float CO_2 = AktualizujTempCO_2();
    float CWU = AktualizujTempCWU();
    wylaczPodajnik();
    
    if (CO_1 < tempZadanaCO) 
    {
      wlaczPodajnik();
      wlaczWentylator();
    }
    else
    {
      wylaczWentylator();
    }
    przepalanie();
    */
  
    
}
