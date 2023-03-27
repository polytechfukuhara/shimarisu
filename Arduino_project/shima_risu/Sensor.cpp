#include <Arduino.h>
#include "Sensor.h"

Sensor::Sensor(int echo_pin, int trig_pin) {
  m_echo_pin = echo_pin;
  m_trig_pin = trig_pin;
  pinMode(m_echo_pin, INPUT);
  pinMode(m_trig_pin, OUTPUT);
}

double Sensor::read_distance() {
  digitalWrite(m_trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(m_trig_pin, LOW);

  double time = pulseIn(m_echo_pin, HIGH, MAX_WAIT);
  double dist = (time * sonic * 100) / 1000000 / 2;
  return dist;
}

bool Sensor::checkIsInside() {
  double distance;
  distance = read_distance();
  Serial.println(distance);
  if ( 0 < distance && distance < boxLength) {
    return true;
  }
  return false;
}
