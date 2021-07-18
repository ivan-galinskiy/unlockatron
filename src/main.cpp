#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <PWMServo.h>

#define arr_size(arr) (sizeof(arr)/sizeof((arr)[0]))

// The digital output of the mike module will be connected to the below pin
const int SOUND_PIN_D = 2;

// The servo's control line will be connected to the below pin
const int SERVO_PIN = 23;

// If a timeout more than this is reached after a knock, it indicates that
// thenext "digit" is being input
const int COUNTING_TIMEOUT = 1000;

// Hardcoded password consisting of numbers of knocks
// Note: order is reversed
int PASSWORD[] = {3, 2, 1};
const int PWD_N = arr_size(PASSWORD);

PWMServo s;

// A helper function to lower the servo arm and let the keys drop
void servo_release(bool yes) {
  if (yes) {
    s.write(180);
  }
  else {
    s.write(50);
  }
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SOUND_PIN_D, INPUT);

    Serial.begin(115200);
    s.attach(SERVO_PIN);
    servo_release(false);
}

// Just a helper to see if the sound level is above threshold
inline bool sound() {
    return digitalRead(SOUND_PIN_D);
}

// Block and wait for a knock to happen
void wait_for_knock() {
    while (1) {
        // Wait for the first edge
        while (!sound())
            ;

        // Then confirm after a delay
        delayMicroseconds(100);
        if (sound()) {
            // Debounce
            delay(100);
            return;
        }
        else
            continue;
    }
}

// Shift the elements of an array to the left
void lshift_array(int *arr, int N) {
  for (int i = N - 1; i > 0; i--) {
    arr[i] = arr[i - 1];
  }
}

// Check that the arrays have the same elements
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
          // ACCESS GRANTED
          digitalWrite(LED_BUILTIN, 1);
          
          servo_release(true);
          delay(2000);
          servo_release(false);
          digitalWrite(LED_BUILTIN, 0);
        }

        digitalWrite(LED_BUILTIN, 1);
        delay(50);
        digitalWrite(LED_BUILTIN, 0);

        Serial.print(knocks_counter);
    }
}