#include <Arduino.h>
#include"Led.h"

Led::Led(int num) {
  led_pin = num;
  pinMode(led_pin, OUTPUT);
}

void Led::ledOn() {
  digitalWrite(led_pin, HIGH);
}

void Led::ledOff() {
  digitalWrite(led_pin, LOW);
}

void Led::blinkLed() {
  digitalWrite(led_pin, HIGH);
  delay(500);
  digitalWrite(led_pin, LOW);
  delay(500);
}
