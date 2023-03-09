//タイマー割り込み
//onTimerの中に割り込み処理をする関数を書く
//onTimerはクラス化できないのでinoファイルから動かせない
//onTimerの中に秒数(int型)、関数のポインタのグローバル変数を入れて
//setAndDoTimerでそれらに値をセット、実行する

#include"Timer.h"

//グローバル変数
int second = 3;           //onTimer(割り込み関数)の処理をするまでの秒数
void (*pfunc)();          //割り込み処理で実行する関数のポインタ

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

void setAndDoTimer(int time_s, void (*func)()) {
  second = time_s;
  pfunc = func;
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
    pfunc();
    delay(1000);
  }
}
