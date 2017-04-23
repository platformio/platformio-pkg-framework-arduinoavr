#include <NIBO2.h>
#include <NIBO_GFX.h>

void setup() {
  NIBO2.begin();
}

void loop() {
  NIBO2.setLed(0, LED_RED);
  delay(500);
  NIBO2.setLed(0, LED_GREEN);
  delay(500);
}
