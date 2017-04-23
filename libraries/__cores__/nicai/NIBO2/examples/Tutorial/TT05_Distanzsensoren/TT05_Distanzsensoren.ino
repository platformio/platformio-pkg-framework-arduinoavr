#include <NIBO2.h>
#include <NIBO_GFX.h>

void setup() {
  NIBO2.begin();
  GFX.begin();
  DistSensor.beginMeasure();
  GFX.term.home();
  GFX.term.println("TT05: Distanzsensoren");
  GFX.term.println("R   FR  F   FL  L");
}

void loop() {
  delay(100);

  GFX.term.cursor(0, 2);  
  GFX.term.clearLine();
  GFX.term.print(DistSensor.get(0), HEX);
  GFX.term.cursor(16, 2);
  GFX.term.print(DistSensor.get(4), HEX);
    
  GFX.term.cursor(4, 3);
  GFX.term.clearLine();
  GFX.term.print(DistSensor.get(1), HEX);
  GFX.term.cursor(12, 3);
  GFX.term.print(DistSensor.get(3), HEX);

  GFX.term.cursor(8, 4);
  GFX.term.clearLine();
  GFX.term.print(DistSensor.get(2), HEX);
  
  GFX.term.cursor(0, 6);
  GFX.term.clearLine();
  GFX.term.print("supply: ");
  GFX.term.print(NIBO2.getVoltage()/1000.0);
  GFX.term.println(" V");
}
