#include <NIBOburger.h>

void setup() {
  NIBOburger.begin();
  NIBOburger.checkVoltage();
}

void loop() {
  for (int time=50; time<800; time*=2) {
    for (int led=1; led<5; led++) {
      NIBOburger.setLed(led, ON);
      delay(time);
      NIBOburger.setLed(led, OFF);
      delay(time);
    }
  }
}
