#ifndef temperature_sensor_h
#define temperature_sensor_h

#include <OneWire.h>
#include <DallasTemperature.h>

class Temperature
{
  public:
    Temperature(int pin);
    float get();
    int getAsInt();
  private:
    DallasTemperature sensor;
    OneWire *oneWire;
    float temp [5] {-1000,-1000,-1000,-1000,-1000};
    float currTemp = -1000;
    byte tempCursor =0;
    unsigned long time;
};

#endif
