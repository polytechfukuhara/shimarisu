#include "Led.h"

#define RED_PIN 32
#define GREEN_PIN 33

Led ledRed = Led(RED_PIN);
Led ledGreen = Led(GREEN_PIN);

void setup() {

}

void loop() {
  ledRed.ledOn();
  ledGreen.ledOn();
}
