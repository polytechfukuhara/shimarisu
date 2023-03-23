//タイマー割り込み
//onTimerの中に割り込み処理をする関数を書く
//onTimerはクラス化できないのでinoファイルから動かせない
//onTimerの中に秒数(int型)、関数のポインタのグローバル変数を入れて
//setAndDoTimerでそれらに値をセット、実行する

#include"Timer.h"

//グローバル変数
int secondSensor = 3;           //onTimer(割り込み関数)の処理をするまでの秒数
int secondLock = 4;
boolean flag = true;

//インスタンス化
Timer timerSensor = Timer(onTimerSensor);
Timer timerLock = Timer(onTimerLock);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (flag) {
    timerSensor.timerSet();
  } else {
    timerLock.timerSet();
  }

  flag = !flag;
}


void onTimerSensor() {
  timerSensor.isrCounter++;
  //右辺に動かしたい秒数を入れる
  if (timerSensor.isrCounter == secondSensor) {

    timerEnd(timerSensor.timer);
    timerSensor.timer = NULL;
    timerSensor.isrCounter = 0;

    //割り込み処理
    Serial.println("Sensor");
    delay(1000);
  }
}

void onTimerLock() {
  timerLock.isrCounter++;
  //右辺に動かしたい秒数を入れる
  if (timerLock.isrCounter == secondLock) {

    timerEnd(timerLock.timer);
    timerLock.timer = NULL;
    timerLock.isrCounter = 0;

    //割り込み処理
    Serial.println("Lock");
    delay(1000);
  }
}
