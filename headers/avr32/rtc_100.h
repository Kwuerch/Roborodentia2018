/*****************************************************************************
 *
 * Copyright (C) 2008-2010 Atmel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 *
 * * Neither the name of the copyright holders nor the names of
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * Model        : AP7000
 * Revision     : $Revision$
 * Checkin Date : $Date$ 
 *
 ****************************************************************************/
#ifndef AVR32_RTC_100_H_INCLUDED
#define AVR32_RTC_100_H_INCLUDED

#define AVR32_RTC_H_VERSION 100

#include "avr32/abi.h"


/*
 Note to user:

 The following defines are always generated:
 - Register offset: AVR32_RTC_<register>
 - Bitfield mask:   AVR32_RTC_<register>_<bitfield>
 - Bitfield offset: AVR32_RTC_<register>_<bitfield>_OFFSET
 - Bitfield size:   AVR32_RTC_<register>_<bitfield>_SIZE
 - Bitfield values: AVR32_RTC_<register>_<bitfield>_<value name>

 The following defines are generated if they don't cause ambiguities,
 i.e. the name is unique, or all values with that name are the same.
 - Bitfield mask:   AVR32_RTC_<bitfield>
 - Bitfield offset: AVR32_RTC_<bitfield>_OFFSET
 - Bitfield size:   AVR32_RTC_<bitfield>_SIZE
 - Bitfield values: AVR32_RTC_<bitfield>_<value name>
 - Bitfield values: AVR32_RTC_<value name>

 All defines are sorted alphabetically.
*/


#define AVR32_RTC_CTRL                                     0x00000000
#define AVR32_RTC_CTRL_EN                                           0
#define AVR32_RTC_CTRL_EN_MASK                             0x00000001
#define AVR32_RTC_CTRL_EN_OFFSET                                    0
#define AVR32_RTC_CTRL_EN_SIZE                                      1
#define AVR32_RTC_CTRL_PCLR                                         1
#define AVR32_RTC_CTRL_PCLR_MASK                           0x00000002
#define AVR32_RTC_CTRL_PCLR_OFFSET                                  1
#define AVR32_RTC_CTRL_PCLR_SIZE                                    1
#define AVR32_RTC_CTRL_PSEL                                         8
#define AVR32_RTC_CTRL_PSEL_MASK                           0x00ffff00
#define AVR32_RTC_CTRL_PSEL_OFFSET                                  8
#define AVR32_RTC_CTRL_PSEL_SIZE                                   16
#define AVR32_RTC_CTRL_TOPEN                                        2
#define AVR32_RTC_CTRL_TOPEN_MASK                          0x00000004
#define AVR32_RTC_CTRL_TOPEN_OFFSET                                 2
#define AVR32_RTC_CTRL_TOPEN_SIZE                                   1
#define AVR32_RTC_EN                                                0
#define AVR32_RTC_EN_MASK                                  0x00000001
#define AVR32_RTC_EN_OFFSET                                         0
#define AVR32_RTC_EN_SIZE                                           1
#define AVR32_RTC_ICR                                      0x00000020
#define AVR32_RTC_ICR_TOPI                                          0
#define AVR32_RTC_ICR_TOPI_MASK                            0x00000001
#define AVR32_RTC_ICR_TOPI_OFFSET                                   0
#define AVR32_RTC_ICR_TOPI_SIZE                                     1
#define AVR32_RTC_IDR                                      0x00000014
#define AVR32_RTC_IDR_TOPI                                          0
#define AVR32_RTC_IDR_TOPI_MASK                            0x00000001
#define AVR32_RTC_IDR_TOPI_OFFSET                                   0
#define AVR32_RTC_IDR_TOPI_SIZE                                     1
#define AVR32_RTC_IER                                      0x00000010
#define AVR32_RTC_IER_TOPI                                          0
#define AVR32_RTC_IER_TOPI_MASK                            0x00000001
#define AVR32_RTC_IER_TOPI_OFFSET                                   0
#define AVR32_RTC_IER_TOPI_SIZE                                     1
#define AVR32_RTC_IMR                                      0x00000018
#define AVR32_RTC_IMR_TOPI                                          0
#define AVR32_RTC_IMR_TOPI_MASK                            0x00000001
#define AVR32_RTC_IMR_TOPI_OFFSET                                   0
#define AVR32_RTC_IMR_TOPI_SIZE                                     1
#define AVR32_RTC_ISR                                      0x0000001c
#define AVR32_RTC_ISR_TOPI                                          0
#define AVR32_RTC_ISR_TOPI_MASK                            0x00000001
#define AVR32_RTC_ISR_TOPI_OFFSET                                   0
#define AVR32_RTC_ISR_TOPI_SIZE                                     1
#define AVR32_RTC_PCLR                                              1
#define AVR32_RTC_PCLR_MASK                                0x00000002
#define AVR32_RTC_PCLR_OFFSET                                       1
#define AVR32_RTC_PCLR_SIZE                                         1
#define AVR32_RTC_PSEL                                              8
#define AVR32_RTC_PSEL_MASK                                0x00ffff00
#define AVR32_RTC_PSEL_OFFSET                                       8
#define AVR32_RTC_PSEL_SIZE                                        16
#define AVR32_RTC_TOP                                      0x00000008
#define AVR32_RTC_TOPEN                                             2
#define AVR32_RTC_TOPEN_MASK                               0x00000004
#define AVR32_RTC_TOPEN_OFFSET                                      2
#define AVR32_RTC_TOPEN_SIZE                                        1
#define AVR32_RTC_TOPI                                              0
#define AVR32_RTC_TOPI_MASK                                0x00000001
#define AVR32_RTC_TOPI_OFFSET                                       0
#define AVR32_RTC_TOPI_SIZE                                         1
#define AVR32_RTC_TOP_VAL                                           0
#define AVR32_RTC_TOP_VAL_MASK                             0xffffffff
#define AVR32_RTC_TOP_VAL_OFFSET                                    0
#define AVR32_RTC_TOP_VAL_SIZE                                     32
#define AVR32_RTC_VAL                                      0x00000004
#define AVR32_RTC_VAL_MASK                                 0xffffffff
#define AVR32_RTC_VAL_OFFSET                                        0
#define AVR32_RTC_VAL_SIZE                                         32
#define AVR32_RTC_VAL_VAL                                           0
#define AVR32_RTC_VAL_VAL_MASK                             0xffffffff
#define AVR32_RTC_VAL_VAL_OFFSET                                    0
#define AVR32_RTC_VAL_VAL_SIZE                                     32




#ifdef __AVR32_ABI_COMPILER__


typedef struct avr32_rtc_ctrl_t {
    unsigned int                 : 8;
    unsigned int psel            :16;
    unsigned int                 : 5;
    unsigned int topen           : 1;
    unsigned int pclr            : 1;
    unsigned int en              : 1;
} avr32_rtc_ctrl_t;



typedef struct avr32_rtc_ier_t {
    unsigned int                 :31;
    unsigned int topi            : 1;
} avr32_rtc_ier_t;



typedef struct avr32_rtc_idr_t {
    unsigned int                 :31;
    unsigned int topi            : 1;
} avr32_rtc_idr_t;



typedef struct avr32_rtc_imr_t {
    unsigned int                 :31;
    unsigned int topi            : 1;
} avr32_rtc_imr_t;



typedef struct avr32_rtc_isr_t {
    unsigned int                 :31;
    unsigned int topi            : 1;
} avr32_rtc_isr_t;



typedef struct avr32_rtc_icr_t {
    unsigned int                 :31;
    unsigned int topi            : 1;
} avr32_rtc_icr_t;



typedef struct avr32_rtc_t {
  union {
          unsigned long                  ctrl      ;//0x0000
          avr32_rtc_ctrl_t               CTRL      ;
  };
          unsigned long                  val       ;//0x0004
          unsigned long                  top       ;//0x0008
          unsigned int                   :32       ;//0x000c
  union {
          unsigned long                  ier       ;//0x0010
          avr32_rtc_ier_t                IER       ;
  };
  union {
          unsigned long                  idr       ;//0x0014
          avr32_rtc_idr_t                IDR       ;
  };
  union {
    const unsigned long                  imr       ;//0x0018
    const avr32_rtc_imr_t                IMR       ;
  };
  union {
    const unsigned long                  isr       ;//0x001c
    const avr32_rtc_isr_t                ISR       ;
  };
  union {
          unsigned long                  icr       ;//0x0020
          avr32_rtc_icr_t                ICR       ;
  };
} avr32_rtc_t;



/*#ifdef __AVR32_ABI_COMPILER__*/
#endif

/*#ifdef AVR32_RTC_100_H_INCLUDED*/
#endif

