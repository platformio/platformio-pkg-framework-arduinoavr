#include <NIBOburger.h>

enum {
  OBST_CLEAR = 0,
  OBST_LEFT  = 1,
  OBST_RIGHT = 2,
  OBST_BOTH  = 3
};

uint8_t environment = OBST_CLEAR;

uint8_t calculateEnvironment() {
  uint16_t left  = max(SensorFLL.get(), SensorFL.get());
  uint16_t right = max(SensorFRR.get(), SensorFR.get());
  
  if ((left>15)&&(right>15)) {
    return OBST_BOTH;
  }
  if (left>15) {
    return OBST_LEFT;
  }
  if (right>15) {
    return OBST_RIGHT;
  }
  return OBST_CLEAR;
}

void setup() {
  NIBOburger.begin();
}

void loop() {
  NIBOburger.waitAnalogUpdate();
  NIBOburger.checkVoltage();
  uint8_t new_env = calculateEnvironment();
  
  if (new_env!=environment) {
    environment = new_env;
    switch (new_env) {
      case OBST_CLEAR:
        NIBOburger.setLed(LED2, 0);
        NIBOburger.setLed(LED3, 0);
        Engine.setSpeed(+25, +25);
        break;

      case OBST_LEFT:
        NIBOburger.setLed(LED2, 1);
        NIBOburger.setLed(LED3, 0);
        Engine.setSpeed(+20, -20);
        break;

      case OBST_RIGHT:
        NIBOburger.setLed(LED2, 0);
        NIBOburger.setLed(LED3, 1);
        Engine.setSpeed(-20, +20);
        break;

      case OBST_BOTH:
        NIBOburger.setLed(LED2, 1);
        NIBOburger.setLed(LED3, 1);
        Engine.setSpeed(0, 0);
        break;
    } 
  }
}
