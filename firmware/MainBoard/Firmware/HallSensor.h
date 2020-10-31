#ifndef hall_sensor_h
#define hall_sensor_h

class Hallotron
{
  public:
    Hallotron(int pin);
    bool getHallState();
  private:
    int pin;   
};

#endif
