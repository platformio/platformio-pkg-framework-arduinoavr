#ifndef _BOB3_H_
#define _BOB3_H_

#if (defined (ARDUINO)) && (!defined (ARDUINO_AVR_BOB3))
#error "BOB3 library should be used with a BOB3 robot as board!"
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <bob3/utils.h>

void setup();
void loop();

enum {
  ARM_NONE,
  ARM_TOP,
  ARM_MID_TOP,
  ARM_MID_BOT,
  ARM_BOT
};


enum {
  OFF            = 0x000,
  WHITE          = 0xfff,
  ON             = 0xfff,
  RED            = 0xf00,
  GREEN          = 0x0f0,
  BLUE           = 0x00f,
  YELLOW         = 0xff0,
  CYAN           = 0x0ff,
  FUCHSIA        = 0xf0f,
  
  ORANGE         = 0xf80,
  KABARED        = 0xd42,
  PURPLE         = 0xf08,
  VIOLET         = 0x63a,
  AQUAMARINE     = 0x7fd,
  BROWN          = 0xa33,
  CORAL          = 0xf75,
  CORNFLOWERBLUE = 0x69e,
  STEELBLUE      = 0x48a,
  ROYALBLUE      = 0x46e,
  FORESTGREEN    = 0x282,
  SEAGREEN       = 0x5f9
};


enum {
  ARM_1  = 1,
  ARM_2  = 2,
  EYE_1  = 1,
  EYE_2  = 2,
  LED_3 = 3,
  LED_4 = 4
};


class Bob3 {
  public:
    void init();
    void setLed(uint8_t id, uint16_t color);
    uint16_t getLed(uint8_t id);
    void setEyes(uint16_t eye1, uint16_t eye2) {setLed(1, eye1); setLed(2, eye2);};
    void setWhiteLeds(uint16_t wled1, uint16_t wled2) {setLed(3, wled1); setLed(4, wled2);};
    uint16_t getIRSensor();
    uint16_t getIRLight();
    void enableIRSensor(bool enable);
    uint8_t getArm(uint8_t id);
    void enableArms(bool enable);
    uint16_t getTemperature();
    uint16_t getMillivolt();
    uint8_t getID();
    int16_t receiveIRCode(uint8_t carrier, uint16_t timeout);
    void transmitIRCode(uint8_t carrier, uint8_t code);
    int16_t receiveIRCode(uint16_t timeout) {return receiveIRCode(0, timeout);}
    void transmitIRCode(uint8_t code) {transmitIRCode(0, code);}
};

extern Bob3 bob3;

#if !defined(ARDUINO)
inline static 
void delay(uint16_t ms) {while (ms--) _delay_ms(1);}
#endif

inline static uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) __attribute__((const));

inline static 
uint16_t rgb(uint8_t r, uint8_t g, uint8_t b) {return ((uint16_t)r<<8)|(g<<4)|b;}

uint16_t mixColor(uint16_t color1, uint16_t color2, uint8_t w1, uint8_t w2);

uint16_t hsv(int16_t h, uint8_t s, uint8_t v);

uint16_t hsvx(uint8_t h, uint8_t s, uint8_t v);

uint16_t randomNumber(uint16_t lo, uint16_t hi);

uint16_t randomBits(uint8_t zeros, uint8_t ones);

uint32_t random32();


static inline 
uint8_t random8() {
  return random32() & 0xff;
}


static inline 
uint16_t random16() {
  return random32() & 0xffff;
}


#endif
