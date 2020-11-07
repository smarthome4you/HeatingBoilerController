#ifndef boiler_feeder_h
#define boiler_feeder_h

#include "HallSensor.h"
#include "RelaySSR.h"

class BoilerFeeder
/*
 * Obsługuje silnik podajnika z hallotronem.
 * Wykonuje zawsze jeden poprawny cykl pracy podajnika.
 * Podajnik po otrzymaniu od hallotron informacji o punkcie zatrzymania pracuje jeszcze przez zadany czas: <timeToStopFeeder>
 */
{
  public:
    BoilerFeeder(int ssrPin, int hallPin);
    void on();
    void off();
    bool isRun();
    bool isError();
    void process();
    Hallotron *hall;
    RelaySSR *relay;
  private:
    int ssrPin;
    int hallPin;
    bool isRunning = false;
    bool startStopping = false;
    bool errorFeeder = false;
    int lastHallState = -1;
    int counterHallState = 0;
    unsigned long startTime = 0; // czas pracy podajnika na jednym załaczniu używane w sytuacji gdyby hallotron przestał działać.
};

#endif
