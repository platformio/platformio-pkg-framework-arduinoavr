#include <NIBOburger.h>

void setup() {
  NIBOburger.begin();
  NIBOburger.checkVoltage();
  delay(100);
  NIBOburger.randomize();
}

void loop() {
  int led = NIBOburger.getRandomInt(1,4);
  NIBOburger.setLed(led, ON);
  delay(100);
  led = NIBOburger.getRandomInt(1,4);
  NIBOburger.setLed(led, OFF);
  delay(100);
}

