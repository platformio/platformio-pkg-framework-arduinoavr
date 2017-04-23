#include <NIBObee.h>

void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
}

void loop() {
  NIBObee.setLed(LED0, ON);
  delay(500);
  NIBObee.setLed(LED0, OFF);
  delay(500);
}
