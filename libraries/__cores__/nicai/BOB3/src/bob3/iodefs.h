/*  BSD-License:

Copyright (c) 2015 by Nils Springob, nicai-systems, Germany

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

/*! @file    iodefs.h
 *  @brief   Zuordnung der physikalischen Pins zu symbolischen Namen
 */

#ifndef _IODEFS_H_
#define _IODEFS_H_



#include "iodefs_bob3.h"

#ifdef __AVR_ATmega644__
#define _NIBOBURGER_MEGAxx4_
#endif

#ifdef __AVR_ATmega324__
#define _NIBOBURGER_MEGAxx4_
#endif

#ifdef __AVR_ATmega164__
#define _NIBOBURGER_MEGAxx4_
#endif


#include <avr/io.h>
#include <avr/interrupt.h>


/******************************************************************************/

#define LOBYTE(x)        (uint8_t)((uint16_t)x)
#define HIBYTE(x)        (uint8_t)(((uint16_t)x)>>8)
#define MAKE_WORD(hi,lo) ((hi*0x100)+lo)

#define enable_interrupts() sei()
#define disable_interrupts() cli()

/* helper macro */
#define _PPCAT(a,b) a##b
#define __PPCAT(a,b) _PPCAT(a,b)
#define _PPCAT3(a,b,c) a##b##c
#define __PPCAT3(a,b,c) _PPCAT3(a,b,c)

/* NEW */
#define IO_PORT_REG(name, reg) _PPCAT(name,_PID)
#define IO_PORT_PID(name) _PPCAT(name,_PID)
#define IO_PORT_BIT(name) _PPCAT(name,_BIT)
#define IO_PORT_BITVAL(name) _BV(IO_PORT_BIT(name))

#define IO_PORT(name) __PPCAT(PORT,IO_PORT_PID(name))
#define IO_PIN(name)  __PPCAT(PIN,IO_PORT_PID(name))
#define IO_DDR(name)  __PPCAT(DDR,IO_PORT_PID(name))





/* Macro-functions for IO-bits */
#define activate_output_bit(name) IO_DDR(name)|=IO_PORT_BITVAL(name)
#define deactivate_output_bit(name) IO_DDR(name)&=~(unsigned char)IO_PORT_BITVAL(name)
#define set_output_bit(name) IO_PORT(name)|=IO_PORT_BITVAL(name)
#define clear_output_bit(name) IO_PORT(name)&=~(unsigned char)IO_PORT_BITVAL(name)
#define toggle_output_bit(name) IO_PORT(name)^=IO_PORT_BITVAL(name)
#define set_output_bitval(name, val) IO_PORT(name)=(IO_PORT(name)&~(unsigned char)IO_PORT_BITVAL(name))|((val)?IO_PORT_BITVAL(name):0)

#define get_input_bit(name) (IO_PIN(name)&IO_PORT_BITVAL(name))
#define get_output_bit(name) (IO_PORT(name)&IO_PORT_BITVAL(name))


/* Helper Macro for atomic Blocks in C90 compiler mode */
#define ATOMIC_C90_BEGIN() {uint8_t _sreg_bak__ = SREG; cli(); {
#define ATOMIC_C90_END() } SREG = _sreg_bak__;}


#endif
