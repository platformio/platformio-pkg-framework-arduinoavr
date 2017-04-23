#include <NIBOburger.h>

void setup() {
  NIBOburger.begin();
  NIBOburger.checkVoltage();
}

void loop() {
  char key = NIBOburger.getKeyChar();
  
  switch (key) {
    case 'A':
      NIBOburger.setLed(LED1, ON);
      break;
    case 'B':
      NIBOburger.setLed(LED2, ON);
      break;
    case 'C':
      NIBOburger.setLed(LED3, ON);
      break;
    case 'a':
      NIBOburger.setLed(LED1, OFF);
      break;
    case 'b':
      NIBOburger.setLed(LED2, OFF);
      break;
    case 'c':
      NIBOburger.setLed(LED3, OFF);
      break;
  }
}
