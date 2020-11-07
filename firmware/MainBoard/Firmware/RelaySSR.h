#ifndef relay_ssr_h
#define relay_ssr_h

class RelaySSR
{
  public:
    RelaySSR(int pin);
    void on();
    void off();
    bool isOn();
  private:
    int pin;
    bool relayState = false;
};

#endif
