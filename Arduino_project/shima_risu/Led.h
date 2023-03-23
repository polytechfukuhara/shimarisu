#ifndef Led_h
#define Led_h

class Led
{
  private:
    int led_pin;

  public:
    void ledOn();
    void ledOff();
    void blinkLed();
    Led(int pinNum);
};
#endif
