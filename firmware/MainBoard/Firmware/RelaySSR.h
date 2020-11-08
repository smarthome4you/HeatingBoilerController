#ifndef relay_ssr_h
#define relay_ssr_h

class RelaySSR
{
  public:
    RelaySSR(int pin);
    void on();
    void forceOn();
    void forceOff();
    void off();
    bool isOn();
    void lockOn(unsigned long milliSeconds);
    unsigned long lastChange();
  private:
    int pin;
    bool relayStateOn = false;
    unsigned long lastChangeState;
    unsigned long lockOnMilliSeconds;
};

#endif
