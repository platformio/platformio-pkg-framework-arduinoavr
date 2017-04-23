#include <NIBOburger.h>

bool waitForKey(unsigned int millisec) {
  for (unsigned int t=0; t<millisec; t++) {
    if (NIBOburger.getKeyChar()!=0) {
      return true;
    }
    delay(1);
  }
  return false;
}

void displayState(unsigned int state) {
  switch (state) {
    case 0:  NIBOburger.setLeds(OFF, OFF, OFF, OFF); break;
    case 1:  NIBOburger.setLeds( ON, OFF, OFF, OFF); break;
    case 2:  NIBOburger.setLeds(OFF,  ON, OFF, OFF); break;
    case 3:  NIBOburger.setLeds(OFF, OFF,  ON, OFF); break;
    case 4:  NIBOburger.setLeds(OFF, OFF, OFF,  ON); break;
    case 5:  NIBOburger.setLeds( ON, OFF, OFF,  ON); break;
    case 6:  NIBOburger.setLeds(OFF,  ON, OFF,  ON); break;
    case 7:  NIBOburger.setLeds(OFF, OFF,  ON,  ON); break;
    case 8:  NIBOburger.setLeds( ON, OFF,  ON,  ON); break;
    case 9:  NIBOburger.setLeds(OFF,  ON,  ON,  ON); break;
    case 10: NIBOburger.setLeds( ON,  ON,  ON,  ON); break;
  }
}

void setup() {
  NIBOburger.begin();
}

void loop() {
  NIBOburger.checkVoltage();
  delay(100);
  while (NIBOburger.getKeyChar()!='A') {
    // auf KEY-DOWN von Taster 1 warten....
  }

  displayState(0);
  while (NIBOburger.getKeyChar()!='a') {
    // auf KEY-UP von Taster 1 warten....
  }

  NIBOburger.randomize();
  unsigned int time = NIBOburger.getRandomInt(1000, 4000);

  if (waitForKey(time)) {
    displayState(10);
    return;    
  }

  for (unsigned int i=1; i<=10; i++) {
    displayState(i);
    if (waitForKey(50)) {
      return;
    }
  }
}
