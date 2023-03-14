#pragma once

#define SERVO_PIN 13
#define ECHO_PIN 17
#define TRIG_PIN 16

/*
  #define BTN_PIN
  #define RED_PIN
  #define GREEN_PIN
*/

typedef enum {
  INIT_MODE,
  LOCK_MODE,
  OPEN_MODE,
} MODE;
