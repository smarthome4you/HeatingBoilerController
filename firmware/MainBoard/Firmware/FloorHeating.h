#ifndef floor_heating_h
#define floor_heating_h

#include "Config.h"
#include "RelaySSR.h"

class FloorHeating
{
  public:
  FloorHeating(int ssrPin);
  void setHeatingPeriods(int onLength, int offLength);
  bool isOn();
  void process();
  void pumpOn();
  void pumpOff();
  void forcePumpOn();
  private:
  RelaySSR *relay;
  int ssrPin;
  int onLength;
  int offLength;
  bool lock = false;
  unsigned long startTime = 0;
  unsigned long lastRun = 0;
};

#endif
