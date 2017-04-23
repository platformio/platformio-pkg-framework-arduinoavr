#include <NIBObee.h>

void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
}

void loop() {
  for (int time=50; time<800; time*=2) {
    for (int led=0; led<4; led++) {
      NIBObee.setLed(led, ON);
      delay(time);
      NIBObee.setLed(led, OFF);
      delay(time);
    }
  }
}
