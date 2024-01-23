#include <Arduino.h>
#include <Keypad.h>

#define KEYPAD_ROWS_COUNT 2
#define KEYPAD_COLS_COUNT 2

#define LED_PIN 10
#define MAX_BRIGHTNESS 7
#define MAX_LED_VALUE 63

char KEYS_MATRIX[KEYPAD_ROWS_COUNT][KEYPAD_COLS_COUNT] = {
  {'1', '2'},
  {'3', '4'},
};

byte rowPins[KEYPAD_ROWS_COUNT] = {7, 6};
byte colPins[KEYPAD_COLS_COUNT] = {8, 9};

byte currentBrightness = MAX_BRIGHTNESS / 2;

Keypad customKeypad = Keypad(makeKeymap(KEYS_MATRIX), rowPins, colPins, KEYPAD_ROWS_COUNT, KEYPAD_COLS_COUNT);

void changeBrightness() {
  Serial.print("Current brightness: ");
  Serial.print(currentBrightness);
  Serial.print('/');
  Serial.println(MAX_BRIGHTNESS);

  analogWrite(LED_PIN, (int)((float)currentBrightness * MAX_LED_VALUE / MAX_BRIGHTNESS));
}

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  changeBrightness();
}

void loop() {
  char key = customKeypad.getKey();

  if (key) {
    Serial.print("Pressed key ");
    Serial.println(key);
  }

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