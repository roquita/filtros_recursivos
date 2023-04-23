#include "filter_recursive.h"


#define BUTTON_PIN 14
#define LED_PIN 23
#define LOD_PIN 22
FR_DebounceState_t current = FR_NOT_PRESSED;
FR_DebounceState_t prev = FR_NOT_PRESSED;

void isr_button() {




  //Serial.println(digitalRead(BUTTON_PIN));
}
void setup() {

  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LOD_PIN, OUTPUT);
  digitalWrite(LED_PIN, 1);
  digitalWrite(LOD_PIN, 1);
  //attachInterrupt(BUTTON_PIN, isr_button, CHANGE);
}

void loop() {
  FR_DebounceState_t input = (digitalRead(BUTTON_PIN) == 1 ? FR_NOT_PRESSED : FR_PRESSED);
  FR_DebounceState_t output;
  FR_DebounceSwitch_Run(input, &output);
  //prev = current;

  if (output == FR_PRESSED) {
    digitalWrite(LED_PIN, 0);
  } else {
    digitalWrite(LED_PIN, 1);
  }

  if (input == FR_PRESSED) {
    digitalWrite(LOD_PIN, 0);
  } else {
    digitalWrite(LOD_PIN, 1);
  }

  delay(1);
}
