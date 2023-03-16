#include "Button.h"
#define BUTTON_PIN 2

volatile byte state = HIGH;      // volatileをつけて宣言

Button button(BUTTON_PIN);

void setup() {
  // ピン番号から割り込み番号への変換には専用の関数を使用
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), blink, CHANGE);
}

void loop() {

}

/*割り込みサービスルーチン(ISR)
  void blink() {
  state = !state;
  }
*/
