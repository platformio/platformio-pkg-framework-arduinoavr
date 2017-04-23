#include <NIBO2.h>
#include <NIBO_GFX.h>

void setup() {
  NIBO2.begin();
  GFX.begin();
  GFX.term.home();
  GFX.term.println("TT04: Bodensensoren");
  GFX.term.println("R1   R0   L0   L1");
}

void loop() {
  delay(100);
  
  GFX.term.cursor(0, 2); 
  GFX.term.clearLine();
  GFX.term.print(FloorSensor.getR1());
  GFX.term.cursor(5, 2);
  GFX.term.print(FloorSensor.getR0());
  GFX.term.cursor(10, 2);
  GFX.term.print(FloorSensor.getL0());
  GFX.term.cursor(15, 2);
  GFX.term.print(FloorSensor.getL1());
  
  GFX.term.cursor(0, 3);
  GFX.term.clearLine();
  GFX.term.print(FloorSensor.getR1(1));
  GFX.term.cursor(5, 3);
  GFX.term.print(FloorSensor.getR0(1));
  GFX.term.cursor(10, 3);
  GFX.term.print(FloorSensor.getL0(1));
  GFX.term.cursor(15, 3);
  GFX.term.print(FloorSensor.getL1(1));
  
  GFX.term.cursor(0, 5);
  GFX.term.clearLine();
  GFX.term.print("supply: ");
  GFX.term.print(NIBO2.getVoltage()/1000.0);
  GFX.term.println(" V");
}
