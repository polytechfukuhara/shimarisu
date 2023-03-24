#include "Button.h"
#define BUTTON_PIN 15

volatile byte state = HIGH;      // volatileをつけて宣言

Button button(BUTTON_PIN);

void setup() {
  // ピン番号から割り込み番号への変換には専用の関数を使用
  //attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), blink, CHANGE);
  Serial.begin(115200);
}

void loop() {
  if (button.buttonRead()) {
    Serial.println("true");
  } else {
    Serial.println("false");
  }
  delay(100);
}

/*割り込みサービスルーチン(ISR)
  void blink() {
  state = !state;
  }
*/
