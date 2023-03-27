#pragma once

class Sensor
{
  private :
    int m_echo_pin;
    int m_trig_pin;
    int MAX_WAIT = 20000;
    double sonic = 331.5 + (27.0 * 0.6);
    double boxLength = 19.5; //箱の内部が21.5cm
    double read_distance();

  public :
    Sensor(int echo_pin, int trig_pin);
    bool checkIsInside();
};
