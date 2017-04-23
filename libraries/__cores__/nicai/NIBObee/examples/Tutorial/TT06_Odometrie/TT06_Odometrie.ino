#include <NIBObee.h>

void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
}

void loop() {
  if (FeelerL.get()) {
    Engine.left.resetTicks();
  }
  if (FeelerR.get()) {
    Engine.right.resetTicks();
  }
  
  NIBObee.setLed(LED0, Engine.left.getTicks()>10);
  NIBObee.setLed(LED1, Engine.left.getTicks()>20);
  NIBObee.setLed(LED2, Engine.right.getTicks()>20);
  NIBObee.setLed(LED3, Engine.right.getTicks()>10);
}
