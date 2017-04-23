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

/*! @file    iodefs_bob3.h
 *  @brief   Zuordnung der physikalischen Pins zu symbolischen Namen
 */


/*! white LED 1 (right) */
#define IO_LED_1        IO_LED_1
#define IO_LED_1_PID    B
#define IO_LED_1_BIT    7

/*! white LED 2 (left) */
#define IO_LED_2        IO_LED_2
#define IO_LED_2_PID    B
#define IO_LED_2_BIT    6

/*! Eye 1 (right) */
#define IO_EYE_1        IO_EYE_1
#define IO_EYE_1_PID    B
#define IO_EYE_1_BIT    1

/*! Eye 2 (left) */
#define IO_EYE_2        IO_EYE_2
#define IO_EYE_2_PID    B
#define IO_EYE_2_BIT    2

/*! Eye red */
#define IO_EYE_R        IO_EYE_R
#define IO_EYE_R_PID    D
#define IO_EYE_R_BIT    6

/*! Eye green */
#define IO_EYE_G        IO_EYE_G
#define IO_EYE_G_PID    D
#define IO_EYE_G_BIT    5

/*! Eye blue */
#define IO_EYE_B        IO_EYE_B
#define IO_EYE_B_PID    D
#define IO_EYE_B_BIT    3

/////////// VERSION 1.2

/*! Arm 1 (right) center touch signal */
#define IO_ARM_1C        IO_ARM_1C
#define IO_ARM_1C_PID    C
#define IO_ARM_1C_BIT    4

/*! Arm 1 (right) rim touch signal */
#define IO_ARM_1R        IO_ARM_1R
#define IO_ARM_1R_PID    C
#define IO_ARM_1R_BIT    5

/*! Arm 2 (left) center touch signal */
#define IO_ARM_2C        IO_ARM_2C
#define IO_ARM_2C_PID    C
#define IO_ARM_2C_BIT    2

/*! Arm 2 (left) rim touch signal */
#define IO_ARM_2R        IO_ARM_2R
#define IO_ARM_2R_PID    C
#define IO_ARM_2R_BIT    3

/*! Arm upper touch sensor */
#define IO_ARM_U         IO_ARM_U
#define IO_ARM_U_PID     C
#define IO_ARM_U_BIT     0

/*! Arm lower touch sensor */
#define IO_ARM_L         IO_ARM_L
#define IO_ARM_L_PID     C
#define IO_ARM_L_BIT     1

/////////// VERSION 1.3

#define IO_ARM2_S1 IO_ARM_U
#define IO_ARM2_S2 IO_ARM_L

#define IO_ARM2_TA IO_ARM_2C
#define IO_ARM2_TB IO_ARM_2R
#define IO_ARM2_TC IO_ARM_1C

#define IO_HEAD_S IO_ARM_1R

#define IOG_ANALOG_BIT_ARM2_S1 IOG_ANALOG_BIT_ARM_U
#define IOG_ANALOG_BIT_ARM2_S2 IOG_ANALOG_BIT_ARM_L

/////////// 

/*! IR sensor enable */
#define IO_EN_IR        IO_EN_IR
#define IO_EN_IR_PID    B
#define IO_EN_IR_BIT    0


/*! I2C SDA */
#define IO_I2C_SDA       IO_I2C_SDA
#define IO_I2C_SDA_PID   C
#define IO_I2C_SDA_BIT   4

/*! I2C SCL */
#define IO_I2C_SCL       IO_I2C_SCL
#define IO_I2C_SCL_PID   C
#define IO_I2C_SCL_BIT   5


/*! UART RXD */
#define IO_UART_RXD       IO_UART_RXD
#define IO_UART_RXD_PID   D
#define IO_UART_RXD_BIT   0

/*! UART TXD */
#define IO_UART_TXD       IO_UART_TXD
#define IO_UART_TXD_PID   D
#define IO_UART_TXD_BIT   1

/*! SPI MISO */
#define IO_SPI_MISO       IO_SPI_MISO
#define IO_SPI_MISO_PID   B
#define IO_SPI_MISO_BIT   4

/*! SPI MOSI */
#define IO_SPI_MOSI       IO_SPI_MOSI
#define IO_SPI_MOSI_PID   B
#define IO_SPI_MOSI_BIT   3

/*! SPI SCK */
#define IO_SPI_SCK        IO_SPI_SCK
#define IO_SPI_SCK_PID    B
#define IO_SPI_SCK_BIT    5

/*! ID_0 */
#define IO_ID_0       IO_ID_0
#define IO_ID_0_PID   D
#define IO_ID_0_BIT   2

/*! ID_1 */
#define IO_ID_1        IO_ID_1
#define IO_ID_1_PID   D
#define IO_ID_1_BIT   4

/*! ID_2 - Alias */
#define IO_ID_2        IO_SPI_MOSI

/*! ID_3 - Alias */
#define IO_ID_3        IO_SPI_MISO

/*! ID_4 - Alias */
#define IO_ID_4        IO_SPI_SCK



/*! Analog group */
#define IOG_ANALOG       IOG_ANALOG
#define IOG_ANALOG_PID   C

#define IOG_ANALOG_BIT_ARM_L   1
#define IOG_ANALOG_BIT_ARM_U   0
#define IOG_ANALOG_BIT_SENS_IR 6

#define IOG_ANALOG_MASK  (_BV(IOG_ANALOG_BIT_ARM_L)|_BV(IOG_ANALOG_BIT_ARM_U))
