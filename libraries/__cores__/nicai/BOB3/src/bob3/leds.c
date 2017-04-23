
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/sleep.h>
//#include <avr/cpufunc.h>

#include "iodefs.h"
#include "leds.h"

void leds_init() {
  // setup output bits
  PORTD  |= 0x68;
  DDRD  |= 0x68;
  DDRB  |= 0xc6;
  
  // setup timer 0
  TCCR0A = _BV(COM0A1) // Clear OC0A on Compare Match, set OC0A at BOTTOM
         | _BV(COM0B1) // Clear OC0B on Compare Match, set OC0B at BOTTOM
         | _BV(WGM01) | _BV(WGM00); // Fast PWM
  TCCR0B =  _BV(CS01); // CLK/8
  TIMSK0 |= _BV(TOIE0); // enable timer overflow IRQ

  // setup timer 2
  TCCR2A = _BV(COM2B1) // Clear OC2B on Compare Match, set OC2B at BOTTOM
         | _BV(WGM21) | _BV(WGM20); // Fast PWM
  TCCR2B =  _BV(CS21); // CLK/8
}


uint8_t led_r1;
uint8_t led_g1;
uint8_t led_b1;
uint8_t led_r2;
uint8_t led_g2;
uint8_t led_b2;

uint16_t eye1col;
uint16_t eye2col;
uint8_t eye;


ISR(TIMER0_OVF_vect) {
  if (eye) {
    PORTB|=_BV(1);
    PORTB&=~_BV(2);
    OCR0A = led_r2;
    OCR0B = led_g2;
    OCR2B = led_b2;
    eye=0;
  } else {
    PORTB|=_BV(2);
    PORTB&=~_BV(1);
    OCR0A = led_r1;
    OCR0B = led_g1;
    OCR2B = led_b1;
    eye=1;
  }
}


static uint8_t calcR(uint8_t v) {
  switch (v) {
    case 0: return 255; //1
    case 1: return 254; //2
    case 2: return 252; //2
    case 3: return 250; //3
    case 4: return 247; //4
    case 5: return 243; //5
    case 6: return 238; //6
    case 7: return 232; //8
    case 8: return 224; //10
    case 9: return 214; //11
    case 10: return 203; //12
    case 11: return 191; //13
    case 12: return 178; //15
    case 13: return 163; //18
    case 14: return 145; //20
    case 15: return 125; //
  }
  return 255;
}


static uint8_t calcGB(uint8_t v) {
  switch (v) {
    case 0: return 255;
    case 1: return 253;
    case 2: return 250;
    case 3: return 245;    
    case 4: return 240;
    case 5: return 232;
    case 6: return 222;
    case 7: return 211;
    case 8: return 195;
    case 9: return 176;
    case 10: return 155;
    case 11: return 132;
    case 12: return 107;
    case 13: return 78;
    case 14: return 43;
    case 15: return 5;
  }
  return 255;
}


void leds_set_RGB(uint8_t id, uint8_t r, uint8_t g, uint8_t b) {
  if (id==1) {
    eye1col=r;
    eye1col=(eye1col<<8)|(g<<4)|b;
    led_r1=calcR(r);
    led_g1=calcGB(g);
    led_b1=calcGB(b);
  } else if (id==2) {
    eye2col=r;
    eye2col=(eye2col<<8)|(g<<4)|b;
    led_r2=calcR(r);
    led_g2=calcGB(g);
    led_b2=calcGB(b);
  }
}


void leds_set_RGBx(uint8_t id, uint16_t color) {
  if (id==1) {
    eye1col=color;
    led_r1=calcR((color&0xf00)>>8);
    led_g1=calcGB((color&0x0f0)>>4);
    led_b1=calcGB((color&0x00f)>>0);
  } else if (id==2) {    
    eye2col=color;
    led_r2=calcR((color&0xf00)>>8);
    led_g2=calcGB((color&0x0f0)>>4);
    led_b2=calcGB((color&0x00f)>>0);
  }
}

uint16_t leds_get_RGBx(uint8_t id) {
  if (id==1) {
    return eye1col;
  } else if (id==2) {
    return eye2col;
  }
  return 0;
}

