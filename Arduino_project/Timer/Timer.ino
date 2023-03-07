//タイマー割り込み
//処理までの秒数を引数ではなくグローバル変数で定義する
//割り込み処理の内容はonTimer内に直接書き込む
//違う処理をさせたい場合は別の割り込み関数を作成し、別にインスタンス化必要

#include"Timer.h"

//グローバル変数
//onTimer(割り込み関数)の処理をするまでの秒数
int second = 3;

//インスタンス化
Timer timer = Timer(onTimer);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.timerSet();
}

void onTimer() {
  timer.isrCounter++;
  //右辺に動かしたい秒数を入れる
  if (timer.isrCounter == second) {

    timerEnd(timer.timer);
    timer.timer = NULL;
    timer.isrCounter = 0;

    //割り込み処理
    Serial.println("hellohello");
    
    delay(1000);
  }
}
