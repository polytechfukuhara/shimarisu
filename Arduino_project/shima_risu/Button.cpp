#include <Arduino.h>
#include"Button.h"

Button::Button(int num) {
  button_pin = num;
  pinMode(button_pin, INPUT_PULLUP);//INPUT_PULLUPを使うと内部のプルアップ抵抗が有効になります。
}

bool Button::buttonRead() {
  bool flg = false;
  if (digitalRead(button_pin) == HIGH) {
    flg = true;

  }
  return flg;
}
