#include <OneWire.h>
#include <DallasTemperature.h>
#include "TemperatureSensor.h"


Temperature::Temperature(int pin)
{
  this->oneWire = new OneWire(pin);
  this->sensor.setOneWire(oneWire);
  this->sensor.begin();
}

float Temperature::get()
{
  this->sensor.requestTemperatures();
  return this->sensor.getTempCByIndex(0);
}

/*

float updateTempCWU() {
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


//Serial.print("Aktualna temperatura: ");
//sensors1.requestTemperatures(); //Pobranie temperatury czujnika
//Serial.print(sensors1.getTempCByIndex(0));  //Wyswietlenie informacji
//sensors2.requestTemperatures(); //Pobranie temperatury czujnika
//Serial.print("  ");
//Serial.println(sensors2.getTempCByIndex(0));  //Wyswietlenie informacji
//delay(1000);
