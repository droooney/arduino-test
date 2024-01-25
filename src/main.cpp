#include <Arduino.h>
#include <Keypad.h>

#define KEYPAD_ROWS_COUNT 2
#define KEYPAD_COLS_COUNT 2

#define LED_COUNT 4
#define MAX_BRIGHTNESS 7
#define MAX_LED_VALUE 63

#ifndef DEBUG
#define DEBUG 1
#endif

char KEYS_MATRIX[KEYPAD_ROWS_COUNT][KEYPAD_COLS_COUNT] = {
  {'1', '2'},
  {'3', '4'},
};

byte rowPins[KEYPAD_ROWS_COUNT] = {4, 2};
byte colPins[KEYPAD_COLS_COUNT] = {7, 8};
byte ledPins[LED_COUNT] = {9, 6, 5, 3};

byte currentBrightness = MAX_BRIGHTNESS / 2;
byte currentLedIndex = 0;

Keypad customKeypad = Keypad(makeKeymap(KEYS_MATRIX), rowPins, colPins, KEYPAD_ROWS_COUNT, KEYPAD_COLS_COUNT);

void setBrightness() {
  analogWrite(ledPins[currentLedIndex], (int)((float)currentBrightness * MAX_LED_VALUE / MAX_BRIGHTNESS));
}

void changeLedIndex(byte newIndex) {
  if (currentBrightness != 0) {
    analogWrite(ledPins[currentLedIndex], 0);
  }

  currentLedIndex = newIndex;

#if DEBUG
  Serial.print(F("Current LED index: "));
  Serial.println(currentLedIndex);
#endif

  if (currentBrightness != 0) {
    setBrightness();
  }
}

void changeBrightness() {
#if DEBUG
  Serial.print(F("Current brightness: "));
  Serial.print(currentBrightness);
  Serial.print(F("/"));
  Serial.println(MAX_BRIGHTNESS);
#endif

  setBrightness();
}

void setup() {
#if DEBUG
  Serial.begin(9600);
#endif

  for (auto ledPinIndex : ledPins) {
    pinMode(ledPins[ledPinIndex], OUTPUT);
  }

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
  bool ledChanged = false;
  byte newLedIndex;

  if (key == '1' && currentBrightness > 0) {
    currentBrightness--;
    brightnessChanged = true;
  } else if (key == '2' && currentBrightness < MAX_BRIGHTNESS) {
    currentBrightness++;
    brightnessChanged = true;
  } else if (key == '3') {
    newLedIndex = (currentLedIndex + LED_COUNT - 1) % LED_COUNT;
    ledChanged = true;
  } else if (key == '4') {
    newLedIndex = (currentLedIndex + 1) % LED_COUNT;
    ledChanged = true;
  }

  if (brightnessChanged) {
    changeBrightness();
  } else if (ledChanged) {
    changeLedIndex(newLedIndex);
  }
}