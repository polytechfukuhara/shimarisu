#pragma once

class Lock
{
  private :
    int m_servo_pin;

  public :
    Lock(int servo_pin);
    void openServo();
    void lockServo();
};
