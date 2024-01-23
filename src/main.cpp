#include <Arduino.h>
#include <Keypad.h>

const byte ROWS = 2;
const byte COLS = 2;

char KEYS_MATRIX[ROWS][COLS] = {
  {'1', '2'},
  {'3', '4'},
};

byte rowPins[ROWS] = {7, 6};
byte colPins[COLS] = {8, 9};

Keypad customKeypad = Keypad(makeKeymap(KEYS_MATRIX), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  char customKey = customKeypad.getKey();

  if (customKey) {
    Serial.println(customKey);
  }
}