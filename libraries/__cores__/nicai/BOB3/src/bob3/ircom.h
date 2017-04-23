#ifndef _BOB3_IRCOM_H_

#define _BOB3_IRCOM_H_
#ifdef __cplusplus
extern "C" {
#endif
  


int16_t ircom_receive(uint8_t carrier, uint16_t timeout);  

void ircom_transmit(uint8_t carrier, uint8_t code);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
