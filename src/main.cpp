#include <Arduino.h>

const int SOUND_PIN_D = 2;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SOUND_PIN_D, INPUT);
}

void loop() {
  while (1) {
    digitalWrite(LED_BUILTIN, digitalRead(SOUND_PIN_D));
  }
}