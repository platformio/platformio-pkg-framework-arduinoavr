/*  BSD-License:

Copyright (c) 2016 by Nils Springob, nicai-systems, Germany

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
  * Neither the name nicai-systems nor the names of its contributors may be
    used to endorse or promote products derived from this software without
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

/*! @file    analog.c
 *  @brief   Zuordnung der physikalischen Pins zu symbolischen Namen
 *  @author  Nils Springob (nils.springob@nicai-systems.com)
 *  @date    2015-02-25
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "iodefs.h"
#include "analog.h"


#ifdef __cplusplus
extern "C" {
#endif

extern uint8_t _bob3_revision;

uint16_t analog_samples[2 * ANALOG_CNT];

uint16_t analog_random_seed;
uint8_t analog_pos;
uint8_t analog_flags;
volatile uint8_t analog_sample_id;
uint16_t analog_sum;

#define AREF_1V1(x) (_BV(REFS1)|_BV(REFS0)|((x)))
#define AREF_AVCC(x) (_BV(REFS0)|((x)))
#define AREF_EXT(x)  ((x))
#define AMUX_TEMP 0x08
#define AMUX_1V1  0x0e
#define AMUX_0V   0x0f

void analog_init() {
#if defined(DIDR0)
//  DIDR0 = 0xff;
#endif

  ADCSRA = _BV(ADPS2)  // prescale faktor = 64 ADC laeuft
         //| _BV(ADPS1)  // mit 8 MHz / 64 = 125 kHz
	 | _BV(ADPS0)  // mit 8 MHz / 32 = 250 kHz
         | _BV(ADEN)   // ADC an
//         | _BV(ADATE)  // auto trigger
         | _BV(ADIE)   // enable interrupt
         | _BV(ADSC);  // Beginne mit der Konvertierung

  DDRC |= _BV(2) | _BV(3) | _BV(4) | _BV(5);
  DDRB |= _BV(0);

  PORTC |= _BV(0) | _BV(1); // PullUp!
  
  activate_output_bit(IO_ARM_1R);
  activate_output_bit(IO_ARM_1C);
  activate_output_bit(IO_ARM_2R);
  activate_output_bit(IO_ARM_2C);
  activate_output_bit(IO_EN_IR);
  
  if (_bob3_revision==103) {
    deactivate_output_bit(IO_HEAD_S);
  }
  
  if (SREG&_BV(7)) {
    // measure all once!
    analog_wait_update();
  } else {
    // precharge Key and Voltage input
    analog_samples[ANALOG_VOLT] = analog_samples[ANALOG_VOLT+ANALOG_CNT] = (ADC_BANDGAP_CHANNEL_VOLTAGE*1024)/5;
  }
}

uint8_t analog_enable(uint8_t enable) {
  uint8_t result = (ADCSRA & _BV(ADIE))?0xff:0x00;
  if (enable) {
    ADCSRA |= _BV(ADEN);
    ADCSRA |= _BV(ADIE);  
  } else {
    ADCSRA &= ~_BV(ADIE);
    ADCSRA &= ~_BV(ADEN);
  }
  return result;
}  

uint16_t analog_getValue(uint8_t idx) {
  //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  uint16_t retval = 0;
  {
    uint8_t bak = SREG;
    cli();
    retval = analog_samples[idx];
    SREG = bak;
  }
  return retval;
}


uint16_t analog_getValueExt(uint8_t idx, uint8_t active) {
  if (active==0) {
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    uint16_t retval;
    {
      uint8_t bak = SREG;
      cli();
      retval = analog_samples[idx];
      SREG = bak;
    }
    return retval;

  } else if (active==1) {
    idx += ANALOG_CNT;
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    uint16_t retval;
    {
      uint8_t bak = SREG;
      cli();
      retval = analog_samples[idx];
      SREG = bak;
    }
    return retval;

  } else if (active==2) {
    uint8_t idx2 = idx+ANALOG_CNT;
    int16_t val = 0;
    //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    if (idx<8) {
      // inverted signal: 1023 == no touch
      uint8_t bak = SREG;
      cli();
      val = analog_samples[idx];
      val -= analog_samples[idx2];
      SREG = bak;
    } else {
      uint8_t bak = SREG;
      cli();
      val = analog_samples[idx2];
      val -= analog_samples[idx];
      SREG = bak;
    }
    return (val<0)?0:val;
  }
  return 0;
}

uint16_t analog_getRandomSeed() {
  //random_r(unsigned long *  __ctx);
  //ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  uint16_t retval;
  {
    uint8_t bak = SREG;
    cli();
    retval = analog_random_seed;
    SREG = bak;
  }
  return retval;
}


void analog_wait_update() {
  uint8_t id = analog_sample_id;
  while (id == analog_sample_id) {
  }
}


uint8_t analog_has_update() {
  static uint8_t id = 0;
  if (id == analog_sample_id) {
    return 0;
  }
  id = analog_sample_id;
  return 1;
}

#define KEY_THRESHOLD 50
#define KEY_COUNT 5

static uint8_t updateKey (uint8_t state, uint8_t value) {
  if (value>KEY_THRESHOLD) {
    if (state&0x01) {
      return 1;
      //return (state>1)?(state-2):1;
    } else {
      return (state>=(2*KEY_COUNT-2))?1:(state+2);
    }
  } else {
    if (state&0x01) {
      return (state>(2*KEY_COUNT-2))?0:(state+2);
    } else {
      return 0;
      //return (state>1)?(state-2):0;
    }    
  }
}



void analog_irq_hook() __attribute__((weak));
void analog_irq_hook() {
}

void _key_update_state() __attribute__((weak));
void _key_update_state() {
}

void _key_update_event() __attribute__((weak));
void _key_update_event() {
}

#if F_CPU==8000000
#define STD0 0
#define STD2 2
#define STD3 3
#define STD4 4
#define STD5 5
#define STD6 6

#elif F_CPU==15000000
#define STD0 0
#define STD2 2
#define STD3 3
#define STD4 4
#define STD5 5
#define STD6 6

#elif F_CPU==20000000
#define STD0 0
#define STD2 3
#define STD3 4
#define STD4 5
#define STD5 7
#define STD6 8

#else
#error "unsupported clock speed!"
#endif


#define CASE_ADC(CASE, ANALOG_CH, IO_EN_BIT, AREF, APORT) \
    case CASE+STD0: \
      ADMUX = AREF(APORT); \
      PORTC &= ~_BV(APORT);\
      DDRC |= _BV(APORT);\
      DDRC &= ~_BV(APORT);\
      PORTC |= _BV(APORT);\
      break; \
    case CASE+STD2: \
      analog_samples[ANALOG_CH]=value; \
      set_output_bit(IO_EN_BIT); \
      break; \
    case CASE+STD5: \
      analog_samples[ANALOG_CH+ANALOG_CNT]=value; \
      clear_output_bit(IO_EN_BIT); \
      break;

      
#define CASE_ADC_KEY(CASE, ANALOG_CH, IO_EN_BIT, AREF, APORT) \
    case CASE+STD0: \
      ADMUX = AREF(APORT); \
      PORTC &= ~_BV(APORT);\
      DDRC |= _BV(APORT);\
      DDRC &= ~_BV(APORT);\
      PORTC |= _BV(APORT);\
      break; \
    case CASE+STD2: \
      analog_sum = value; \
      set_output_bit(IO_EN_BIT); \
      break; \
    case CASE+STD5: \
      analog_sum = (analog_sum>value)?(analog_sum-value):0; \
      analog_samples[ANALOG_CH+ANALOG_CNT]=analog_sum; \
      clear_output_bit(IO_EN_BIT); \
      analog_samples[ANALOG_CH]=updateKey(analog_samples[ANALOG_CH], analog_sum/4); \
      break;

      
#define CASE_ADC2(CASE, ANALOG_CH, IO_EN_BIT, AREF, APORT) \
    case CASE+STD0: \
      ADMUX = AREF(APORT); \
      PORTC &= ~_BV(APORT);\
      DDRC |= _BV(APORT);\
      DDRC &= ~_BV(APORT);\
      PORTC |= _BV(APORT);\
      break; \
    case CASE+STD2: \
      analog_sum = value; \
      break; \
    case CASE+STD2+1: \
      analog_sum += value; \
      break; \
    case CASE+STD2+2: \
      analog_sum += value; \
      break; \
    case CASE+STD2+3: \
      analog_sum += value; \
      analog_samples[ANALOG_CH] = analog_sum/4; \
      activate_output_bit(IO_EN_BIT); \
      set_output_bit(IO_EN_BIT); \
      break; \
    case CASE+STD2+STD3+3: \
      analog_sum = value; \
      break; \
    case CASE+STD2+STD3+4: \
      analog_sum += value; \
      break; \
    case CASE+STD2+STD3+5: \
      analog_sum += value; \
      break; \
    case CASE+STD2+STD3+6: \
      analog_sum += value; \
      analog_samples[ANALOG_CH+ANALOG_CNT] = analog_sum/4; \
      clear_output_bit(IO_EN_BIT); \
      break;


ISR(ADC_vect) {
  uint16_t value = ADC;
  sei();
  
  if (_bob3_revision==102) {
    // BOB3 V1.2
    switch (analog_pos) {
      CASE_ADC_KEY(0*STD6, ANALOG_L0,  IO_ARM_1R, AREF_AVCC, IOG_ANALOG_BIT_ARM_U)
      CASE_ADC_KEY(1*STD6, ANALOG_L1,  IO_ARM_1C, AREF_AVCC, IOG_ANALOG_BIT_ARM_U)
      CASE_ADC_KEY(2*STD6, ANALOG_L2,  IO_ARM_1C, AREF_AVCC, IOG_ANALOG_BIT_ARM_L)
      CASE_ADC_KEY(3*STD6, ANALOG_L3,  IO_ARM_1R, AREF_AVCC, IOG_ANALOG_BIT_ARM_L)
      CASE_ADC_KEY(4*STD6, ANALOG_R0,  IO_ARM_2R, AREF_AVCC, IOG_ANALOG_BIT_ARM_U)
      CASE_ADC_KEY(5*STD6, ANALOG_R1,  IO_ARM_2C, AREF_AVCC, IOG_ANALOG_BIT_ARM_U)
      CASE_ADC_KEY(6*STD6, ANALOG_R2,  IO_ARM_2C, AREF_AVCC, IOG_ANALOG_BIT_ARM_L)
      CASE_ADC_KEY(7*STD6, ANALOG_R3,  IO_ARM_2R, AREF_AVCC, IOG_ANALOG_BIT_ARM_L)
      case 8*STD6:
        ADMUX = AREF_1V1(IOG_ANALOG_BIT_SENS_IR);
        break;
      CASE_ADC2(9*STD6, ANALOG_IR,  IO_EN_IR, AREF_1V1, IOG_ANALOG_BIT_SENS_IR)
      case 11*STD6:
        ADMUX = AREF_1V1(AMUX_TEMP);
        break;
      case 11*STD6+STD4:
        analog_sum = value;
        break;
      case 11*STD6+STD4+1:
        analog_samples[ANALOG_TEMP] = (analog_sum+value)>>1;
        break;
      case 12*STD6:
        ADMUX = AREF_AVCC(AMUX_1V1);
        break;      
      case 12*STD6+STD2: {
        analog_samples[ANALOG_VOLT] = value;
      } break;
    }
  } else if (_bob3_revision==103) {
    // BOB3 V1.3
    switch (analog_pos) {
      CASE_ADC_KEY(0*STD6, ANALOG_L0,  IO_ARM2_TA, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S1)
      CASE_ADC_KEY(1*STD6, ANALOG_L1,  IO_ARM2_TB, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S1)
      CASE_ADC_KEY(2*STD6, ANALOG_L2,  IO_ARM2_TB, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S1)
      CASE_ADC_KEY(3*STD6, ANALOG_L3,  IO_ARM2_TC, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S1)
      CASE_ADC_KEY(4*STD6, ANALOG_R0,  IO_ARM2_TA, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S2)
      CASE_ADC_KEY(5*STD6, ANALOG_R1,  IO_ARM2_TB, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S2)
      CASE_ADC_KEY(6*STD6, ANALOG_R2,  IO_ARM2_TB, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S2)
      CASE_ADC_KEY(7*STD6, ANALOG_R3,  IO_ARM2_TC, AREF_AVCC, IOG_ANALOG_BIT_ARM2_S2)
      case 8*STD6:
        ADMUX = AREF_1V1(IOG_ANALOG_BIT_SENS_IR);
        break;
      CASE_ADC2(9*STD6, ANALOG_IR,  IO_EN_IR, AREF_1V1, IOG_ANALOG_BIT_SENS_IR)
      case 11*STD6:
        ADMUX = AREF_1V1(AMUX_TEMP);
        break;
      case 11*STD6+STD4:
        analog_sum = value;
        break;
      case 11*STD6+STD4+1:
        analog_samples[ANALOG_TEMP] = (analog_sum+value)>>1;
        break;
      case 12*STD6:
        ADMUX = AREF_AVCC(AMUX_1V1);
        break;      
      case 12*STD6+STD2: {
        analog_samples[ANALOG_VOLT] = value;
      } break;
    }    
  }
  if (++analog_pos>(12*STD6+STD2)) {
    analog_pos=0;
    analog_irq_hook();
    analog_sample_id++;
    analog_random_seed<<=1;
  }
  analog_random_seed += value;
  ADCSRA |= _BV(ADSC);
}


#ifdef __cplusplus
} // extern "C"
#endif


