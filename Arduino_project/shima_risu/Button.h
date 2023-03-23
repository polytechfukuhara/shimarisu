#ifndef Button_h
#define Button_h

class Button
{
  private:
    int button_pin;

  public:
    Button(int num);
    bool buttonRead();
};
#endif
