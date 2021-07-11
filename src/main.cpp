#include <Arduino.h>

const int SOUND_PIN_D = 2;
const int COUNTING_TIMEOUT = 1000;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SOUND_PIN_D, INPUT);

    Serial.begin(115200);
}

inline bool sound() {
    return digitalRead(SOUND_PIN_D);
}

void wait_for_knock() {
    while (1) {
        // Wait for the first edge
        while (!sound())
            ;

        // Then confirm after a delay
        delayMicroseconds(100);
        if (sound())
            return;
        else
            continue;
    }
}

void loop() {
    int knocks_counter = 0;
    uint32_t t0 = millis();

    while (1) {
        wait_for_knock();

        uint32_t t = millis();
        if ((t - t0) > COUNTING_TIMEOUT) {
          knocks_counter = 1;
        }
        else
          knocks_counter++;

        t0 = t;

        digitalWrite(LED_BUILTIN, 1);
        delay(50);
        digitalWrite(LED_BUILTIN, 0);

        Serial.print(knocks_counter);
    }
}