#ifndef relay_ssr_h
#define relay_ssr_h

class RelaySSR
{
  public:
    RelaySSR(int pin);
    void on();
    void off();
    bool isOn();
    unsigned long lastChange();
  private:
    int pin;
    bool relayState = false;
    unsigned long lastChangeState;
};

#endif
