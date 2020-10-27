#define pinPompaCO     A2 
#define pinPompaCWU    A3
#define pinPompaPDL    A4
#define pinPodajnik    A5
#define pinNadmuch     A10
#define pinTempCO1     A11
#define pinTempCO2     A13
#define pinTempCWU     A14
#define pinHallotron   A16


#ifndef temperature_sensor_h
#define temperature_sensor_h

#include <OneWire.h>
#include <DallasTemperature.h>

class Temperature
{
  public:
    Temperature(int pin);
    float get();
  private:
    DallasTemperature sensor;
    OneWire *oneWire;
};

#endif
