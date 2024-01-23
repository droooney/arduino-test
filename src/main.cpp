#include <Arduino.h>
#include <Keypad.h>

#define KEYPAD_ROWS_COUNT 2
#define KEYPAD_COLS_COUNT 2

#define LED_PIN 10
#define MAX_BRIGHTNESS 7
#define MAX_LED_VALUE 63

#ifndef DEBUG
#define DEBUG 1
#endif

char KEYS_MATRIX[KEYPAD_ROWS_COUNT][KEYPAD_COLS_COUNT] = {
  {'1', '2'},
  {'3', '4'},
};

byte rowPins[KEYPAD_ROWS_COUNT] = {7, 6};
byte colPins[KEYPAD_COLS_COUNT] = {8, 9};

byte currentBrightness = MAX_BRIGHTNESS / 2;

Keypad customKeypad = Keypad(makeKeymap(KEYS_MATRIX), rowPins, colPins, KEYPAD_ROWS_COUNT, KEYPAD_COLS_COUNT);

void changeBrightness() {
#if DEBUG
  Serial.print(F("Current brightness: "));
  Serial.print(currentBrightness);
  Serial.print(F("/"));
  Serial.println(MAX_BRIGHTNESS);
#endif

  analogWrite(LED_PIN, (int)((float)currentBrightness * MAX_LED_VALUE / MAX_BRIGHTNESS));
}

void setup() {
#if DEBUG
  Serial.begin(9600);
#endif

  pinMode(LED_PIN, OUTPUT);
  changeBrightness();
}

void loop() {
  char key = customKeypad.getKey();

#if DEBUG
  if (key) {
    Serial.print(F("Pressed key "));
    Serial.println(key);
  }
#endif

  bool brightnessChanged = false;

  if (key == '1' && currentBrightness > 0) {
    currentBrightness--;
    brightnessChanged = true;
  } else if (key == '2' && currentBrightness < MAX_BRIGHTNESS) {
    currentBrightness++;
    brightnessChanged = true;
  }

  if (brightnessChanged) {
    changeBrightness();
  }
}