#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define arr_size(arr) (sizeof(arr)/sizeof((arr)[0]))

const int SOUND_PIN_D = 2;
const int COUNTING_TIMEOUT = 1000;

// Note: order is reversed
int PASSWORD[] = {3, 2, 1};
const int PWD_N = arr_size(PASSWORD);

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

void lshift_array(int *arr, int N) {
  for (int i = N - 1; i > 0; i--) {
    arr[i] = arr[i - 1];
  }
}

bool arrays_equal(int *a, int *b, int N) {
  for (int i = 0; i < N; i++) {
    if (a[i] != b[i])
      return false;
  }

  return true;
}

void loop() {
    int knocks_counter = 0;
    uint32_t t0 = millis();

    // Array that will contain the input password
    int pwd_input[PWD_N] = {0};

    while (1) {
        wait_for_knock();

        uint32_t t = millis();
        if ((t - t0) > COUNTING_TIMEOUT) {
          // Here we have timed out, so that indicates that a "digit" has been
          // input
          // Shift the input array to the left
          lshift_array(pwd_input, arr_size(pwd_input));
          pwd_input[0] = 1;
        }
        else
          pwd_input[0] = pwd_input[0] + 1;

        t0 = t;
        if (arrays_equal(pwd_input, PASSWORD, PWD_N))
        {
          digitalWrite(LED_BUILTIN, 1);
          delay(1000);
          digitalWrite(LED_BUILTIN, 0);
        }

        digitalWrite(LED_BUILTIN, 1);
        delay(50);
        digitalWrite(LED_BUILTIN, 0);

        Serial.print(knocks_counter);
    }
}