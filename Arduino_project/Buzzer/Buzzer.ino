#include "Buzzer.h"
#define BUZZER_PIN 14

Buzzer buzzer(BUZZER_PIN);

void setup() {
ledcAttachPin(BUZZER_PIN,1);
}


void loop() {
buzzer.onBuzzer();

}
