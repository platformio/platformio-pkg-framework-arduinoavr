#ifndef _BOB3_LEDS_H_

#define _BOB3_LEDS_H_
#ifdef __cplusplus
extern "C" {
#endif
  
void leds_init();

void leds_set_RGB(uint8_t id, uint8_t r, uint8_t g, uint8_t b);

void leds_set_RGBx(uint8_t id, uint16_t color);

uint16_t leds_get_RGBx(uint8_t id);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
