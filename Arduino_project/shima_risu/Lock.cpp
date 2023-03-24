#include <Servo.h>
#include <Arduino.h>
#include "Lock.h"

Servo servo;

Lock::Lock(int servo_pin) {
  m_servo_pin = servo_pin;
  servo.attach(m_servo_pin);
  servo.write(0);

}

void Lock::openServo() {
  servo.attach(m_servo_pin);
  servo.write(0);
}

void Lock::lockServo() {
  servo.attach(m_servo_pin);
  servo.write(90);
}
