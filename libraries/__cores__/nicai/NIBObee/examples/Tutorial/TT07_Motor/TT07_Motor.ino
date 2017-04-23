#include <NIBObee.h>

void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
}

void loop() {
  int speed_l = 0;
  int speed_r = 0;
  switch (FeelerL.get()) {
    case 1: speed_l = 800; break;
    case 0: speed_l = 0; break;
    case -1: speed_l = -800; break;
  }
  switch (FeelerR.get()) {
    case 1: speed_r = 800; break;
    case 0: speed_r = 0; break;
    case -1: speed_r = -800; break;
  }
  Engine.setPWM(speed_l, speed_r);
}
