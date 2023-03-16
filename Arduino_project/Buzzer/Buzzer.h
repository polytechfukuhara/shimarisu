#ifndef Buzzer_h
#define Buzzer_h

class Buzzer
{
  private:
    int buzzer_pin;
    int BUZZER_TIME = 200;

  public:
    Buzzer(int buzzer_pin);
    void onBuzzer();
    void offBuzzer();
};
#endif//#prama once
