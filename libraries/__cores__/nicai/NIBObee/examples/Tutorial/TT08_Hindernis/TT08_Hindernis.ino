#include <NIBObee.h>

const unsigned int MODE_STOP = 0;
const unsigned int MODE_DRIVE = 1;
const unsigned int MODE_BACK = 2;
const unsigned int MODE_STEER_R = 3;
const unsigned int MODE_STEER_L = 4;
const unsigned int MODE_AVOID_R = 5;
const unsigned int MODE_AVOID_L = 6;

unsigned int mode;
int speed_l;
int speed_r;
unsigned int counter_ms;

unsigned int perform_check(unsigned int mode);
unsigned int do_stop();
unsigned int do_drive();
unsigned int do_back();
unsigned int do_steer_r();
unsigned int do_steer_l();
unsigned int do_avoid_r();
unsigned int do_avoid_l();


void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
  mode = MODE_STOP;
}

void loop() {
  delay(1);
  mode = perform_check(mode);
  switch (mode) {
    case MODE_STOP: 
      mode = do_stop(); 
      break;
    case MODE_DRIVE: 
      mode = do_drive(); 
      break;
    case MODE_BACK: 
      mode = do_back(); 
      break;
    case MODE_STEER_R: 
      mode = do_steer_r(); 
      break;
    case MODE_STEER_L: 
      mode = do_steer_l(); 
      break;
    case MODE_AVOID_R: 
      mode = do_avoid_r(); 
      break;
    case MODE_AVOID_L: 
      mode = do_avoid_l(); 
      break;
  }
  switch (mode) {
    case MODE_STOP: 
      speed_l = 0; 
      speed_r = 0;
      break;
    case MODE_DRIVE: 
      speed_l = 500; 
      speed_r = 500;
      break;
    case MODE_BACK: 
      speed_l = -500; 
      speed_r = -500;
      break;
    case MODE_STEER_R: 
      speed_l = 600; 
      speed_r = 400;
      break;
    case MODE_STEER_L: 
      speed_l = 400; 
      speed_r = 600;
      break;
    case MODE_AVOID_R: 
      speed_l = -400; 
      speed_r = -600;
      break;
    case MODE_AVOID_L: 
      speed_l = -600; 
      speed_r = -400;
      break;
  }
  Engine.setSpeed(speed_l, speed_r);
}

unsigned int perform_check(unsigned int mode) {
  if (FeelerL.get() && FeelerR.get()) {
    if ((FeelerL.get()==-1) && (FeelerR.get()==-1)) {
      mode = MODE_BACK;
    } 
    else {
      mode = MODE_STOP;
    }
  }
  return mode;
}

unsigned int do_stop() {
  if ((FeelerL.get()==0) && (FeelerR.get()==0)) {
    return MODE_DRIVE;
  }
  return MODE_STOP;
}

unsigned int do_back() {
  if (FeelerL.get()==0) {
    return MODE_AVOID_L;
  }
  if (FeelerR.get()==0) {
    return MODE_AVOID_R;
  }
  return MODE_BACK;
}

unsigned int do_drive() {
  if (FeelerR.get()==1) {
    return MODE_STEER_L;
  }
  if (FeelerL.get()==1) {
    return MODE_STEER_R;
  }
  if (FeelerR.get()==-1) {
    return MODE_AVOID_L;
  }
  if (FeelerL.get()==-1) {
    return MODE_AVOID_R;
  }
  return MODE_DRIVE;
}

unsigned int do_steer_r() {
  if (FeelerL.get()==0) {
    return MODE_DRIVE;
  }
  return MODE_STEER_R;
}

unsigned int do_steer_l() {
  if (FeelerR.get()==0) {
    return MODE_DRIVE;
  }
  return MODE_STEER_L;
}

unsigned int do_avoid_r() {
  if (counter_ms==0) {
    counter_ms=1000;
  } 
  else {
    counter_ms--;
  }
  if (counter_ms) {
    return MODE_AVOID_R;
  } 
  else {
    return MODE_DRIVE;
  }
}

unsigned int do_avoid_l() {
  if (counter_ms==0) {
    counter_ms=1000;
  } 
  else {
    counter_ms--;
  }
  if (counter_ms) {
    return MODE_AVOID_L;
  } 
  else {
    return MODE_DRIVE;
  }
}
