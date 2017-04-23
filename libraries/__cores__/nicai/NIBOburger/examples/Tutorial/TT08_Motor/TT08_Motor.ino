#include <NIBOburger.h>
 
void setup() {
  NIBOburger.begin();
}

void loop() {
  NIBOburger.checkVoltage();
  int speed = 0;
  char c = NIBOburger.getKeyChar();
    
  if (c!=0) {  
    switch (c) {
      case 'a': speed = 800; break;
      case 'B': speed = 0; break;
      case 'c': speed = -800; break;
    }  
    Engine.setPWM(speed, speed);
  }
  
  delay(10);
}
