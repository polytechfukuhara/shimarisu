#include <Arduino.h>
#include"Buzzer.h"

Buzzer::Buzzer(int num) {
  buzzer_pin = num;
}

void Buzzer::onBuzzer() {
  ledcWriteTone(1,262) ;   // ド
  delay(200) ;                           // 0.2秒待機
  ledcWriteTone(1,294) ;  // レ
  delay(200) ;                           // 0.2秒待機
  ledcWriteTone(1,330) ;   // ミ
  delay(200) ;                           // 0.2秒待機
  ledcWriteTone(1,0) ;   // ミ
  delay(1000) ;                           // 0.2秒待機

}

/*void Buzzer::offBuzzer() {
  delay(BUZZER_TIME) ;                   // 5秒待機
  //noTone(buzzer_pin);                      //ブザー停止
}*/
