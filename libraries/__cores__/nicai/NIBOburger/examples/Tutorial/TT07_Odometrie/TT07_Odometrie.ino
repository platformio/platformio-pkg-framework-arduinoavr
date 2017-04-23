#include <NIBOburger.h>

void setup() {
  NIBOburger.begin();
}

void loop() {
  NIBOburger.checkVoltage();
  
  char key = NIBOburger.getKeyChar();
  if (key=='a') {
    Engine.left.resetTicks();
  }
  if (key=='b') {
    Engine.right.resetTicks();
  }
  if (key=='c') {
    Engine.left.resetTicks();
    Engine.right.resetTicks();
  }
  
  NIBOburger.setLed(LED1, Engine.left.getTicks()>10);
  NIBOburger.setLed(LED2, Engine.left.getTicks()>20);
  NIBOburger.setLed(LED3, Engine.right.getTicks()>20);
  NIBOburger.setLed(LED4, Engine.right.getTicks()>10);
}
