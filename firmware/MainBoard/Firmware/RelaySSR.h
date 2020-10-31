#ifndef relay_ssr_h
#define relay_ssr_h

class RelaySSR
{
  public:
    RelaySSR(int pin);
    void on();
    void off();
  private:
    int pin;   
};

#endif
