#include <NIBOburger.h>

void setup() {
  NIBOburger.begin();
}

void loop() {
  NIBOburger.waitAnalogUpdate();
  NIBOburger.checkVoltage();

  ColorHSV col = SurfaceSensor.getColorHSV();
  int diff_blue   = col.diff(ColorHSV::CALIBRATE_BLUE, 20);
  int diff_red    = col.diff(ColorHSV::CALIBRATE_RED, 20);
  int diff_green  = col.diff(ColorHSV::CALIBRATE_GREEN, 20);
  int diff_yellow = col.diff(ColorHSV::CALIBRATE_YELLOW, 20);
  
  int diff_min = min(min(diff_blue, diff_red), min(diff_green, diff_yellow));
  
  if (diff_min<600) {
    if (diff_min==diff_blue) {
      NIBOburger.setLeds(0, 1, 1, 0);
    } else if (diff_min==diff_red) {
      NIBOburger.setLeds(1, 0, 0, 1);
    } else if (diff_min==diff_green) {
      NIBOburger.setLeds(1, 1, 0, 0);
    } else if (diff_min==diff_yellow) {
      NIBOburger.setLeds(0, 0, 1, 1);
    }
  } else {
    NIBOburger.setLeds(0, 0, 0, 0);
  }
}
