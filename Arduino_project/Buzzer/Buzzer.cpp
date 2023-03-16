#include <Arduino.h>
#include"Buzzer.h"

Buzzer::Buzzer(int num) {
  buzzer_pin = num;
}

void Buzzer::onBuzzer() {
  tone(buzzer_pin, 262, BUZZER_TIME) ;   // ド
  delay(200) ;                           // 0.2秒待機
  tone(buzzer_pin, 294, BUZZER_TIME) ;  // レ
  delay(200) ;                           // 0.2秒待機
  tone(buzzer_pin, 330, BUZZER_TIME) ;   // ミ
  delay(200) ;                           // 0.2秒待機

}

void Buzzer::offBuzzer() {
  delay(BUZZER_TIME) ;                   // 5秒待機
  noTone(buzzer_pin);                      //ブザー停止
}
