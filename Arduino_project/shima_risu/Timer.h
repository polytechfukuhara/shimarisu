#pragma once

class Timer {
  private :
    int LOCK_TIME;
    int SENSOR_TIME;


  public :
    //    void initTimer();
    hw_timer_t *  timer;
    volatile uint32_t isrCounter;
    Timer(void (*F)());
    void timerSet();
    void timerStop();

};
