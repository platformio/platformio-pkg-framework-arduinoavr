#include <NIBOburger.h>

void setup() {
  NIBOburger.begin();
  NIBOburger.checkVoltage();
}

void loop() {
  NIBOburger.setLed(LED1, ON);
  delay(500);
  NIBOburger.setLed(LED1, OFF);
  delay(500);
}
