#include <NIBObee.h>

const unsigned int STATE_IDLE = 0;
const unsigned int STATE_PULL0 = 1;
const unsigned int STATE_PULL1 = 2;
const unsigned int STATE_KICK = 3;
const unsigned int STATE_PUSH = 4;

unsigned int ball_pos;
int direction;
unsigned int state_l;
unsigned int state_r;

unsigned int calculate_state(unsigned int state, int sensor) {
  switch (state) {
    case STATE_PUSH:
    case STATE_IDLE:
      if (sensor==-1) {
        return STATE_PULL0;
      } else if (sensor==+1) {
        return STATE_PUSH;
      }
      return STATE_IDLE;
    case STATE_PULL0:
      if (sensor==-1) {
        return STATE_PULL1;
      } else if (sensor==+1) {
        return STATE_PUSH;
      }
      return STATE_IDLE;
    case STATE_PULL1:
      if (sensor==-1) {
        return STATE_PULL1;
      }
      return STATE_KICK;
    case STATE_KICK:
      return STATE_IDLE;
  }
  return state;
}

void setup() {
  NIBObee.begin();
  NIBObee.checkVoltage();
  ball_pos = 3;
  direction = -1;
  state_l = STATE_IDLE;
  state_r = STATE_IDLE;
}

void loop() {
  delay(100);
  state_l = calculate_state(state_l, FeelerL.get());
  if (((state_l==STATE_PUSH) && (ball_pos==1)) ||
      ((state_l==STATE_KICK) && (ball_pos==0)) ) {
    direction = +1;
  }
  state_r = calculate_state(state_r, FeelerR.get());
  if (((state_r==STATE_PUSH) && (ball_pos==2)) ||
      ((state_r==STATE_KICK) && (ball_pos==3)) ) {
    direction = -1;
  }
  if (direction==+1) {
    if (ball_pos<3) {
      ball_pos++;
    } else {
      direction = 0;
    }
  }
  if (direction==-1) {
    if (ball_pos>0) {
      ball_pos--;
    } else {
      direction = 0;
    }
  }
  NIBObee.setLed(LED0, ball_pos==0);
  NIBObee.setLed(LED1, ball_pos==1);
  NIBObee.setLed(LED2, ball_pos==2);
  NIBObee.setLed(LED3, ball_pos==3);
}
