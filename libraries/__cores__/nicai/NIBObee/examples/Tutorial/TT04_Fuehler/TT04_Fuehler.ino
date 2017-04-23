#include <NIBObee.h>

void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
}

void loop() {
  int8_t left = FeelerL.get();
  switch (left) {
    case -1:
      NIBObee.setLed(LED1, OFF);
      NIBObee.setLed(LED0, ON);
      break;
    case +1:
      NIBObee.setLed(LED1, ON);
      NIBObee.setLed(LED0, OFF);
      break;
    default:
      NIBObee.setLed(LED1, OFF);
      NIBObee.setLed(LED0, OFF);
      break;
  }
  
  int8_t right = FeelerR.get();
  switch (right) {
    case -1:
      NIBObee.setLed(LED2, OFF);
      NIBObee.setLed(LED3, ON);
      break;
    case +1:
      NIBObee.setLed(LED2, ON);
      NIBObee.setLed(LED3, OFF);
      break;
    default:
      NIBObee.setLed(LED2, OFF);
      NIBObee.setLed(LED3, OFF);
      break;
  }
}
