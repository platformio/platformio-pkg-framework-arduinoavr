#include <NIBOburger.h>

void setup() {
  NIBOburger.begin();
}

void loop() {
  NIBOburger.waitAnalogUpdate();
  NIBOburger.checkVoltage();

  ColorRGB col = SurfaceSensor.getColorRGB();
  int diff_black = col.diff(ColorRGB::CALIBRATE_BLACK);
  int diff_white = col.diff(ColorRGB::CALIBRATE_WHITE);
  
  NIBOburger.setLed(LED1, diff_black < 20);
  NIBOburger.setLed(LED2, diff_white < 20);
  
  char key = NIBOburger.getKeyChar();
  if (key=='a') {
    SurfaceSensor.calibrateBlack();
    SurfaceSensor.storeCalibration();
  } else if (key=='b') {
    SurfaceSensor.calibrateWhite();
    SurfaceSensor.storeCalibration();
  }
}
