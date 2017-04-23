#include <NIBObee.h>

void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
}

void loop() {
  NIBObee.setLed(LED0, LineSensor.getL()>160);
  NIBObee.setLed(LED1, LineSensor.getL()>240);
  NIBObee.setLed(LED3, LineSensor.getR()>160);
  NIBObee.setLed(LED2, LineSensor.getR()>240);
}
