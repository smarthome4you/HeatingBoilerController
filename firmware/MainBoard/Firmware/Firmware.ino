#include <Wire.h>
#include <math.h>
#include <OneWire.h>
#include <DallasTemperature.h>


/*
 * Firmware dla kotła z podajnikiem szufladkowym
 * Podajnik zawsze musi wykonać pełny obrót!
 */

#define pinPompaCO     A2 
#define pinPompaCWU    A3
#define pinPompaPDL    A4
#define pinPodajnik    A5
#define pinNadmuch     A10
#define pinTempKotla_1 A12
#define pinTempKotla_2 A13
#define pinTempCWU     A14
#define pinHallotron   A16

#define czujnikTempKotla_1 0
#define czujnikTempKotla_2 1
#define czujnikTempCO      2
#define czujnikTempCWU     3

#define okresPomiaruTemp  1000 // co jaki czas maja byc odczytywane czujniki temperatury 1000 = 1 sec

#define ostatniPomiarTemp  0
#define ostatniaZmianaTemp 1
#define ostatniaTemp       2

#define deltaTemp          1 // 1 stopień celcjusza

float tempZadanaKociol = 55;
float tempZadanaCWU    = 55;
float tempHistereza    = 5;
int   przepalanie      = 40*60; // w sekundach 40 minut

int temperatura [][3]  = {
                           {0, 0, 0},   // Temp kotła punkt pomiaru 1 (czas ostatniego pomiaru, czas ostatniej zmiany temperatury, temp ostatniego pomiaru)
                           {0, 0, 0},   // Temp kotła punkt pomiaru 2 (czas ostatniego pomiaru, czas ostatniej zmiany temperatury, temp ostatniego pomiaru)
                           {0, 0, 0},   // Temp CWU (czas ostatniego pomiaru, czas ostatniej zmiany temperatury, temp ostatniego pomiaru)
                         };

int dmuchawa [][1]     = {
                           {1}     // czas ostaniego załaczenia
                         };


int przekaznik [][1]   = {
                           {1},   // czas ostaniego załączenia pompa CO
                           {1},   // czas ostaniego załączenia pompa CWU
                           {1},   // czas ostaniego załączenia pompa podłoga
                           {1}    // czas ostaniego załączenia podajnika
                         };


OneWire wireTempKotla_1(pinTempKotla_1); 
OneWire wireTempKotla_2(pinTempKotla_2); 
OneWire wireTempCWU(pinTempCWU);  
DallasTemperature sensorTempKotla_1(&wireTempKotla_1);
DallasTemperature sensorTempKotla_2(&wireTempKotla_2);
DallasTemperature sensorTempCWU(&wireTempCWU);

void WireInit() {
  Serial.begin(115200);
  Serial.print("START\n");
}


float AktualizujTempKotla_1() {
  /* 
   * Nie odczytujemy czujnika za czesto bo podnosi to jego temperature i zakłamuje wynik robimy to raz na 1 sek.  
   */
  
  bool newMeasurement = false;
  
  if (temperatura[czujnikTempKotla_1][ostatniPomiarTemp] == 0) {
    temperatura[czujnikTempKotla_1][ostatniPomiarTemp]  = millis(); 
    temperatura[czujnikTempKotla_1][ostatniaZmianaTemp] = millis();
    newMeasurement = true;
  }

  // Czy upłynął wystarczający czas od ostatniego pomiaru aby wykonać pomiar ponownie
  if (  abs(millis() - temperatura[czujnikTempKotla_1][ostatniPomiarTemp]) > okresPomiaruTemp) {
    newMeasurement = true;
  }
  
  if ( newMeasurement ) {
     sensorTempKotla_1.begin();
     sensorTempKotla_1.requestTemperatures();   
     float tempKotla = sensorTempKotla_1.getTempCByIndex(0);

     // Kiedy ostatnio zmieniła się temperatura o deltaTemp
     if ( abs(temperatura[czujnikTempKotla_1][ostatniaTemp] - tempKotla) > deltaTemp ) {
          temperatura[czujnikTempKotla_1][ostatniaZmianaTemp] = millis();
     }
     temperatura[czujnikTempKotla_1][ostatniaTemp] = tempKotla;
     temperatura[czujnikTempKotla_1][ostatniPomiarTemp] = millis(); 
  }
  
  return temperatura[czujnikTempKotla_1][ostatniaTemp];
}

float AktualizujTempKotla_2() {
  // Nie odczytujemy czujnika za czesto bo podnosi to jego temperature i zakłamuje wynik 
  // robimy to raz na 1 sek.
  return 0;
}

float AktualizujTempCWU() {
  // Nie odczytujemy czujnika za czesto bo podnosi to jego temperature i zakłamuje wynik 
  // robimy to raz na 1 sek.
  return 0;
}

bool CheckList() {
  return true;
}


void setup(){
  WireInit();
  

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
    AktualizujTempKotla_1();
    AktualizujTempKotla_2();
    AktualizujTempCWU();

}
