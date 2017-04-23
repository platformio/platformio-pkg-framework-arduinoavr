#include <NIBO2.h>
#include <NIBO_GFX.h>

void setup() {
  NIBO2.begin();
  GFX.begin();
  GFX.term.home();
  GFX.term.println("TT06: Bewegung");
}

int counter = 0;

void loop() {
  delay(100);
  
  GFX.term.cursor(0, 1);
  GFX.term.clearLine();
  GFX.term.print("supply: ");
  GFX.term.print(NIBO2.getVoltage()/1000.0);
  GFX.term.println(" V");  
   
  switch (++counter) {
    case 20:
      Engine.setSpeed(20, 20);
      break;
      
    case 40:
      Engine.stop();
      break;
      
    case 60:
      Engine.setSpeed(-20, -20);
      break;
      
    case 80:
      Engine.stop();
      counter = 0;
      break;
  }
}
