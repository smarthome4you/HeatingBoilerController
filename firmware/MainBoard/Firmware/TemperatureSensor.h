#ifndef temperature_sensor_h
#define temperature_sensor_h

#include <OneWire.h>
#include <DallasTemperature.h>

class Temperature
{
  public:
    Temperature(int pin);
    void init(int wait);
    float get();
    unsigned long lastReadMillis();
    bool isError();
    int countOfError();
  private:
    DallasTemperature sensor;
    OneWire *oneWire;
    float temp [5] {-1000,-1000,-1000,-1000,-1000};
    float currTemp = -1000;
    byte tempCursor =0;
    int errorCounter = 3;
    int globalErrorCounter = 0;
    int pin;
    bool iserror = false;
    unsigned long lastRead;
};

#endif
