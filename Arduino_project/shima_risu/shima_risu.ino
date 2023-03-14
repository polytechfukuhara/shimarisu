#include "shima_risu.h"
#include "Sensor.h"
#include "Lock.h"

//インスタンス化
Lock lock = Lock(SERVO_PIN);
Sensor sensor = Sensor(ECHO_PIN, TRIG_PIN);


void setup() {

}


void loop() {

}
