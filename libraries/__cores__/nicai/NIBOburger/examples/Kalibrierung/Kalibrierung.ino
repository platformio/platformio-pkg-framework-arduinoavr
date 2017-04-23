#include <NIBObee.h>

void setup()  {
   NIBObee.begin();
}


void do_blink(uint8_t mask) {
  for (int i=0; i<5; ++i) {
    NIBObee.setLed(1, mask&0x01);
    NIBObee.setLed(2, mask&0x01);
    NIBObee.setLed(0, mask&0x02);
    NIBObee.setLed(3, mask&0x02);
    delay(200);
    NIBObee.setLed(1, 0);
    NIBObee.setLed(2, 0);
    NIBObee.setLed(0, 0);
    NIBObee.setLed(3, 0);
    delay(200);
  }
}


void loop() {
  delay(10);
  int16_t speed_l=0;
  int16_t speed_r=0;

  if (FeelerL.get()==-1) {
    while (FeelerL.get()==-1) {
      delay(1);
    }
    LineSensor.calibrateWhite();
    do_blink(2);
  }
     
  if (FeelerR.get()==-1) {
    while (FeelerR.get()==-1) {
      delay(1);
    }
    LineSensor.calibrateBlack();
    do_blink(1);
  }

  NIBObee.setLed(0, LineSensor.getL()>160);
  NIBObee.setLed(1, LineSensor.getL()>240);
  NIBObee.setLed(2, LineSensor.getR()>160);
  NIBObee.setLed(3, LineSensor.getR()>240);

  if ((FeelerL.get()==1) && (FeelerR.get()==1)) {
    while ((FeelerL.get()==1) && (FeelerR.get()==1)) {
      delay(1);
    }
    LineSensor.storeCalibration();
    do_blink(3);
  }
}
