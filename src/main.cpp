#include <Arduino.h>
#include <IRremote.hpp>
#include <Keypad.h>

#define KEYPAD_ROWS_COUNT 2
#define KEYPAD_COLS_COUNT 2

#define LED_COUNT 4
#define MAX_BRIGHTNESS 7
#define MAX_LED_VALUE 63

#define IR_RECEIVER_PIN 23
#define IR_CODE_LEFT 0x8
#define IR_CODE_RIGHT 0x5A
#define IR_CODE_UP 0x18
#define IR_CODE_DOWN 0x52

#ifndef DEV
#define DEV 1
#endif

char KEYS_MATRIX[KEYPAD_ROWS_COUNT][KEYPAD_COLS_COUNT] = {
  {'1', '2'},
  {'3', '4'},
};

byte rowPins[KEYPAD_ROWS_COUNT] = {48, 46};
byte colPins[KEYPAD_COLS_COUNT] = {49, 47};
byte ledPins[LED_COUNT] = {5, 4, 3, 2};

byte ledIrCodes[LED_COUNT] = {0x45, 0x46, 0x47, 0x44};

byte currentBrightness = MAX_BRIGHTNESS / 2;
byte enabledLeds = 0x1;

Keypad customKeypad = Keypad(makeKeymap(KEYS_MATRIX), rowPins, colPins, KEYPAD_ROWS_COUNT, KEYPAD_COLS_COUNT);

int getLedPinVoltage() {
  return (int)(sq((float)currentBrightness / MAX_BRIGHTNESS) * MAX_LED_VALUE);
}

void setBrightness() {
  for (byte ledIndex = 0; ledIndex < LED_COUNT; ledIndex++) {
    if (enabledLeds & 1 << ledIndex) {
      analogWrite(ledPins[ledIndex], getLedPinVoltage());
    }
  }
}

void setEnabledLeds(byte newEnabledLeds) {
  int voltage = getLedPinVoltage();

  if (voltage != 0) {
    for (byte ledIndex = 0; ledIndex < LED_COUNT; ledIndex++) {
      bool wasEnabled = enabledLeds & 1 << ledIndex;
      bool isEnabled = newEnabledLeds & 1 << ledIndex;

      if (wasEnabled && !isEnabled) {
        analogWrite(ledPins[ledIndex], 0);
      } else if (!wasEnabled && isEnabled) {
        analogWrite(ledPins[ledIndex], getLedPinVoltage());
      }
    }
  }

  enabledLeds = newEnabledLeds;
}

void toggleLedIndex(byte ledIndex) {
  setEnabledLeds(enabledLeds ^ (1 << ledIndex));
}

void shiftLedIndexes(bool toLeft) {
  setEnabledLeds(
    toLeft
      ? (enabledLeds >> 1) | ((enabledLeds & 1) << (LED_COUNT - 1))
      : (enabledLeds << 1) - (enabledLeds & (1 << (LED_COUNT - 1)) ? 1 : 0) * ((1 << LED_COUNT) - 1)
  );
}

void changeBrightness(bool less) {
  if ((less && currentBrightness == 0) || (!less && currentBrightness == MAX_BRIGHTNESS)) {
    return;
  }

  if (less) {
    currentBrightness--;
  } else {
    currentBrightness++;
  }

#if DEV
  Serial.print(F("Current brightness: "));
  Serial.print(currentBrightness);
  Serial.print(F("/"));
  Serial.println(MAX_BRIGHTNESS);
#endif

  setBrightness();
}

void setup() {
#if DEV
  Serial.begin(9600);
#endif

  IrReceiver.begin(IR_RECEIVER_PIN);

  for (auto ledPinIndex : ledPins) {
    pinMode(ledPins[ledPinIndex], OUTPUT);
  }

  setBrightness();
}

void loop() {
  char key = customKeypad.getKey();

  if (IrReceiver.decode()) {
    IrReceiver.resume();

    // TODO: add repeat delay
    if (IrReceiver.decodedIRData.protocol == NEC && (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT)) {
      long command = IrReceiver.decodedIRData.command;

      if (command == IR_CODE_DOWN) {
        changeBrightness(true);
      } else if (command == IR_CODE_UP) {
        changeBrightness(false);
      } else if (command == IR_CODE_LEFT) {
        shiftLedIndexes(true);
      } else if (command == IR_CODE_RIGHT) {
        shiftLedIndexes(false);
      } else {
        byte index = 0;

        for (auto code : ledIrCodes) {
          if (code == command) {
            toggleLedIndex(index);

            break;
          }

          index++;
        }
      }
    }

#if DEV
    IrReceiver.printIRResultShort(&Serial);
#endif
  }

#if DEV
  if (key) {
    Serial.print(F("Pressed key "));
    Serial.println(key);
  }
#endif

  if (key == '1') {
    changeBrightness(true);
  } else if (key == '2') {
    changeBrightness(false);
  } else if (key == '3') {
    shiftLedIndexes(true);
  } else if (key == '4') {
    shiftLedIndexes(false);
  }
}