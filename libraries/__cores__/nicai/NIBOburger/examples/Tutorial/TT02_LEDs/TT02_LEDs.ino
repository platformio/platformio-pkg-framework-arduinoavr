#include <NIBOburger.h>

void setup() {
  NIBOburger.begin();
  NIBOburger.checkVoltage();
}

void loop() {
  for (int led=1; led<5; led++) {
    NIBOburger.setLed(led, ON);
    delay(350);
    NIBOburger.setLed(led, OFF);
    delay(150);
  }
}
