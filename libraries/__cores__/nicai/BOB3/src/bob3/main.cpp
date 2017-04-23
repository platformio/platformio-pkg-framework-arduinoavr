#if !defined(ARDUINO)

#include "bob3.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void init() __attribute__((weak));
void init() {
}


void setup() __attribute__((weak));
void setup() {
}


void loop() __attribute__((weak));
void loop() {
}


int main () {
  init();
  sei();
  bob3.init();
  setup();
  while (1) {
    loop();
  }
}

#else

#include "BOB3.h"

extern "C" void _time_resume();

extern "C" void bob3_init() {
  bob3.init();
  _time_resume();
}


#endif
