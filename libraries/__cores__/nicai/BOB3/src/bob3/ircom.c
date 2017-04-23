
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "iodefs.h"
#include "ircom.h"


#ifdef __cplusplus
extern "C" {
#endif

#define RX_DELTA 24
  
#define AREF_1V1(x) (_BV(REFS1)|_BV(REFS0)|((x)))
#define AREF_AVCC(x) (_BV(REFS0)|((x)))
#define AREF_EXT(x)  ((x))
#define AMUX_TEMP 0x08
#define AMUX_1V1  0x0e
#define AMUX_0V   0x0f

  
#if defined(ARDUINO)
void _time_suspend();
void _time_resume();
#else
inline static void _time_suspend() {}
inline static void _time_resume() {}
#endif

  
static void start_timer(uint8_t carrier) {
  TCCR1A = 0;
  TCCR1B = _BV(WGM12) | _BV(CS11); // CTC[OCR1A], PS=8
  OCR1A = 250-1; // 250 us  
}

static void stop_timer() {
  TCCR1B = 0;
  _time_resume();
}

static void wait_timer() {
  TIFR1 |= _BV(OCF1A);
  while (!(TIFR1&_BV(OCF1A))) {
  }
}

static uint16_t get_ir_value() {
  ADMUX = AREF_1V1(IOG_ANALOG_BIT_SENS_IR);
  ADCSRA |= _BV(ADSC);
  while (ADCSRA & _BV(ADSC)) {}
  return ADC;
}


static uint8_t nibble_sync;
static uint8_t nibble_bpos;
static uint8_t nibble_val;

static void receive_nibble_reset() {
  nibble_sync = nibble_bpos = nibble_val = 0;
  
}

static void receive_nibble_val(uint8_t bit) {
  nibble_val <<= 1;
  nibble_val |= bit;
  nibble_bpos++;
}


// 0x00 - nothing
// 0x10 - 0x1f: rxdata
// 0xff - error
static uint8_t receive_nibble(uint8_t subbit) {
  
  switch (nibble_sync) {
    case 0:  if (subbit) nibble_sync++; break;
    
    case 1:  nibble_sync++; break;
    case 2:  nibble_sync++; break;
    case 3:  nibble_sync++; break;
    case 4:  receive_nibble_val(1); if (subbit) nibble_sync=0xff; else nibble_sync++; break;
    
    case 5:  if (subbit) nibble_sync=0xff; else nibble_sync++; break;
    case 6:  if (subbit) nibble_sync=1; else nibble_sync++; break;
    case 7:  if (subbit) nibble_sync=1; else nibble_sync++; break;
    case 8:  if (subbit) nibble_sync=1; else nibble_sync++; break;
    
    case 9:  if (subbit) nibble_sync=1; else nibble_sync++; break;
    case 10: if (subbit) nibble_sync=1; else nibble_sync++; break;
    case 11: if (subbit) nibble_sync=0xff; else nibble_sync++; break;
    case 12: receive_nibble_val(0); nibble_sync = 5; break;
  }
  
  if (nibble_sync==0xff) {
    return 0xff;
  }
  
  if (nibble_bpos>6) {
    return 0xff;
  }
  
  if ((nibble_sync==6) && (nibble_bpos==6)) {
    // nibble complete!
    uint8_t res = nibble_val;
    nibble_bpos = 0;
    nibble_val = 0;
    if ((res&0x21)!=0x20) {
      return 0xff;
    }
    // success
    
    return (res>>1);
  }
  return 0;
}

int16_t ircom_receive(uint8_t carrier, uint16_t timeout) {
  receive_nibble_reset();
  ADCSRA |= _BV(ADEN);
  get_ir_value(); // dummy read 25 ADC...
  start_timer(carrier);
  
  timeout*=16;
  
  uint16_t rxval[16];
  uint8_t rxpos=0;
  uint8_t lev=0;
  uint8_t zero=0;
  uint8_t silence=64;
  int16_t result = -1;
  uint8_t nibble = 0;
  uint8_t hinib = 0;
  uint8_t lonib = 0;
  
  while (timeout--) {
    wait_timer();
    //clear_output_bit(IO_LED_2);
    int16_t val = get_ir_value();
    rxval[rxpos] = val;
    val -= rxval[(rxpos+8)%16];
    if (val>RX_DELTA) {
      zero = 0;
      if (lev<6) lev++;
    } else if (val<-RX_DELTA) {
      zero = 0;
      if (lev>0) lev--;
    } else {
      if (zero) {
        if (lev>0) lev--;
      } else {
        zero=1;
      }
    }

    if (silence) {
      silence--;
      if (lev>4) {
        silence=64;
      }    
    } else if (rxpos & 0x01) {
      uint8_t res = receive_nibble(lev>4);
      if ((res>=0x10)&&(res<=0x1f)) {
	res &= 0x0f;
        switch (nibble++) {
          case 0: if (res!=0x0f) res=0xff; break;
          case 1: hinib = res; break;
          case 2: lonib = res; break;
	  case 3:
	    if ((res^hinib^lonib)==0x0f) {
	      //set_output_bit(IO_LED_2);
	      result = (hinib<<4) | lonib;
	      timeout=0;
	    } else {
	      res=0xff;
	    }
        }
      }
      if (res==0xff) {
        receive_nibble_reset();
        silence = 64;
        nibble = 0;
      }
    
    }
    if (++rxpos>15) rxpos=0;
  }
  
  stop_timer();
  ADCSRA &= ~_BV(ADEN);
  return result;
}

static void transmit_bit(uint8_t val) {
  for (uint8_t i=0; i<8; ++i) {
    wait_timer();
  }
  if (val) set_output_bit(IO_EN_IR);
  for (uint8_t i=0; i<8; ++i) {
    wait_timer();
  }
  clear_output_bit(IO_EN_IR);
}

static void transmit_nibble(uint8_t val) {
  transmit_bit(1);
  for (uint8_t i=0; i<4; ++i) {
    transmit_bit(val&0x80);
    val <<= 1;
  }  
  transmit_bit(0);
}


void ircom_transmit(uint8_t carrier, uint8_t code) {
  uint8_t hinib = code;
  uint8_t lonib = code<<4;
  start_timer(carrier);
  transmit_nibble(0xf0);
  transmit_nibble(hinib);
  transmit_nibble(lonib);
  transmit_nibble(~(hinib^lonib));
  stop_timer();
}



#ifdef __cplusplus
} // extern "C"
#endif
