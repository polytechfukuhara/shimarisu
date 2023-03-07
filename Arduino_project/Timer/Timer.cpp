#include<Arduino.h>
#include"Timer.h"

//void IRAM_ATTR onTimer();

//void Timer::Timer() {
//  hw_timer_t *  timer = NULL;
//  volatile uint32_t isrCounter = 0;
//}

Timer::Timer(void (*F)() ) {
  timer = NULL;
  isrCounter = 0;
  timer = timerBegin(0, getApbFrequency() / 1000000, true);//1μ秒で1クロック
  timerAttachInterrupt(timer, F, true);//onTimerが割り込み関数
  timerAlarmWrite(timer, 1000000, true);//1秒ごとにカウント　定期的に割り込みが発生
}

void Timer::timerSet() {
  timerAlarmEnable(timer);//timerの有効化
}


//使えない
//フラグで終了させるべき
void Timer::timerStop() {
  timerEnd(timer);
  timer = NULL;
}
