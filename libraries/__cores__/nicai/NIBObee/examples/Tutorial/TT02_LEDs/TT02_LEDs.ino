#include <NIBObee.h>

void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
}

void loop() {
  for (int led=0; led<4; led++) {
    NIBObee.setLed(led, ON);
    delay(350);
    NIBObee.setLed(led, OFF);
    delay(150);
  }
}
