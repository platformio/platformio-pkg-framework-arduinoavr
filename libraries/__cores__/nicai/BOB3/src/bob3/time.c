#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/sleep.h>
//#include <avr/cpufunc.h>


#if defined(ARDUINO)

#ifdef __cplusplus
extern "C" {
#endif

volatile uint32_t clock_ticks;

static uint32_t get_clock_ticks() {
  uint32_t m;
  uint8_t oldSREG = SREG;
  cli();
  m = clock_ticks;
  SREG = oldSREG;
  return m;
};


unsigned long millis() {
  return get_clock_ticks() / 50;
}


unsigned long micros() {
  return get_clock_ticks() * 20;
}


static void delay_ticks(unsigned long ticks) {
  uint32_t end = get_clock_ticks()+ticks;
  while (1) {    
    yield();
    uint32_t dt = get_clock_ticks() - end;
    if (!(dt&0x80000000)) {
      return;
    }
  }
}

void delay(unsigned long ms) {
  while (ms) {
    delay_ticks(50);
    ms--;
  }
}

void _time_suspend() {
  TIMSK1 &= ~_BV(OCIE1A);
  TCCR1A = 0;
  TCCR1B = 0;
}

void _time_resume() {
  TCCR1A = 0;
  TCCR1B = _BV(WGM12) | _BV(CS11); // CTC, PS=8
  OCR1A = 20-1; // 20 us  // 50 kHz
  TIMSK1 |= _BV(OCIE1A);
}

#ifdef __cplusplus
}
#endif


ISR(TIMER1_COMPA_vect) {
  clock_ticks++;
}


#endif



