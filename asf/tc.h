/*****************************************************************************
 *
 * \file
 *
 * \brief Timer/Counter driver for AVR32 UC3.
 *
 * AVR32 Timer/Counter driver module.
 *
 * Copyright (c) 2009-2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 ******************************************************************************/


#ifndef _TC_H_
#define _TC_H_

#include <avr32/io.h>


#define TC_INVALID_ARGUMENT                     (-1)

#define TC_NUMBER_OF_CHANNELS                   (sizeof(((avr32_tc_t *)0)->channel) / sizeof(avr32_tc_channel_t))


#define TC_CH0_EXT_CLK0_SRC_TCLK0               AVR32_TC_TC0XC0S_TCLK0
#define TC_CH0_EXT_CLK0_SRC_NO_CLK              AVR32_TC_TC0XC0S_NO_CLK
#define TC_CH0_EXT_CLK0_SRC_TIOA1               AVR32_TC_TC0XC0S_TIOA1
#define TC_CH0_EXT_CLK0_SRC_TIOA2               AVR32_TC_TC0XC0S_TIOA2



#define TC_CH1_EXT_CLK1_SRC_TCLK1               AVR32_TC_TC1XC1S_TCLK1
#define TC_CH1_EXT_CLK1_SRC_NO_CLK              AVR32_TC_TC1XC1S_NO_CLK
#define TC_CH1_EXT_CLK1_SRC_TIOA0               AVR32_TC_TC1XC1S_TIOA0
#define TC_CH1_EXT_CLK1_SRC_TIOA2               AVR32_TC_TC1XC1S_TIOA2



#define TC_CH2_EXT_CLK2_SRC_TCLK2               AVR32_TC_TC2XC2S_TCLK2
#define TC_CH2_EXT_CLK2_SRC_NO_CLK              AVR32_TC_TC2XC2S_NO_CLK
#define TC_CH2_EXT_CLK2_SRC_TIOA0               AVR32_TC_TC2XC2S_TIOA0
#define TC_CH2_EXT_CLK2_SRC_TIOA1               AVR32_TC_TC2XC2S_TIOA1



#define TC_EVT_EFFECT_NOOP                      AVR32_TC_NONE
#define TC_EVT_EFFECT_SET                       AVR32_TC_SET
#define TC_EVT_EFFECT_CLEAR                     AVR32_TC_CLEAR
#define TC_EVT_EFFECT_TOGGLE                    AVR32_TC_TOGGLE



#define TC_NO_TRIGGER_COMPARE_RC                0
#define TC_TRIGGER_COMPARE_RC                   1



#define TC_WAVEFORM_SEL_UP_MODE                 AVR32_TC_WAVSEL_UP_NO_AUTO
#define TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER      AVR32_TC_WAVSEL_UP_AUTO
#define TC_WAVEFORM_SEL_UPDOWN_MODE             AVR32_TC_WAVSEL_UPDOWN_NO_AUTO
#define TC_WAVEFORM_SEL_UPDOWN_MODE_RC_TRIGGER  AVR32_TC_WAVSEL_UPDOWN_AUTO



#define TC_EXT_TRIG_SEL_TIOA                    1
#define TC_EXT_TRIG_SEL_TIOB                    0



#define TC_EXT_EVENT_SEL_TIOB_INPUT             AVR32_TC_EEVT_TIOB_INPUT
#define TC_EXT_EVENT_SEL_XC0_OUTPUT             AVR32_TC_EEVT_XC0_OUTPUT
#define TC_EXT_EVENT_SEL_XC1_OUTPUT             AVR32_TC_EEVT_XC1_OUTPUT
#define TC_EXT_EVENT_SEL_XC2_OUTPUT             AVR32_TC_EEVT_XC2_OUTPUT



#define TC_SEL_NO_EDGE                          AVR32_TC_EEVTEDG_NO_EDGE
#define TC_SEL_RISING_EDGE                      AVR32_TC_EEVTEDG_POS_EDGE
#define TC_SEL_FALLING_EDGE                     AVR32_TC_EEVTEDG_NEG_EDGE
#define TC_SEL_EACH_EDGE                        AVR32_TC_EEVTEDG_BOTH_EDGES



#define TC_BURST_NOT_GATED                      AVR32_TC_BURST_NOT_GATED
#define TC_BURST_CLK_AND_XC0                    AVR32_TC_BURST_CLK_AND_XC0
#define TC_BURST_CLK_AND_XC1                    AVR32_TC_BURST_CLK_AND_XC1
#define TC_BURST_CLK_AND_XC2                    AVR32_TC_BURST_CLK_AND_XC2



#define TC_CLOCK_RISING_EDGE                    0
#define TC_CLOCK_FALLING_EDGE                   1



#define TC_CLOCK_SOURCE_TC1                     AVR32_TC_TCCLKS_TIMER_CLOCK1
#define TC_CLOCK_SOURCE_TC2                     AVR32_TC_TCCLKS_TIMER_CLOCK2
#define TC_CLOCK_SOURCE_TC3                     AVR32_TC_TCCLKS_TIMER_CLOCK3
#define TC_CLOCK_SOURCE_TC4                     AVR32_TC_TCCLKS_TIMER_CLOCK4
#define TC_CLOCK_SOURCE_TC5                     AVR32_TC_TCCLKS_TIMER_CLOCK5
#define TC_CLOCK_SOURCE_XC0                     AVR32_TC_TCCLKS_XC0
#define TC_CLOCK_SOURCE_XC1                     AVR32_TC_TCCLKS_XC1
#define TC_CLOCK_SOURCE_XC2                     AVR32_TC_TCCLKS_XC2



typedef struct
{
  unsigned int                 :24;

  unsigned int etrgs           : 1;

  unsigned int ldrbs           : 1;

  unsigned int ldras           : 1;

  unsigned int cpcs            : 1;

  unsigned int cpbs            : 1;

  unsigned int cpas            : 1;

  unsigned int lovrs           : 1;

  unsigned int covfs           : 1;
} tc_interrupt_t;

typedef struct
{
  unsigned int channel            ;

  unsigned int                 :12;

  unsigned int ldrb            : 2;

  unsigned int ldra            : 2;

  unsigned int                 : 1;

  unsigned int cpctrg          : 1;

  unsigned int                 : 3;

  unsigned int abetrg          : 1;

  unsigned int etrgedg         : 2;

  unsigned int ldbdis          : 1;

  unsigned int ldbstop         : 1;

  unsigned int burst           : 2;

  unsigned int clki            : 1;

  unsigned int tcclks          : 3;
} tc_capture_opt_t;

typedef struct
{
  unsigned int channel            ;

  unsigned int bswtrg          : 2;

  unsigned int beevt           : 2;

  unsigned int bcpc            : 2;

  unsigned int bcpb            : 2;

  unsigned int aswtrg          : 2;

  unsigned int aeevt           : 2;

  unsigned int acpc            : 2;

  unsigned int acpa            : 2;

  unsigned int                 : 1;

  unsigned int wavsel          : 2;

  unsigned int enetrg          : 1;

  unsigned int eevt            : 2;

  unsigned int eevtedg         : 2;

  unsigned int cpcdis          : 1;

  unsigned int cpcstop         : 1;

  unsigned int burst           : 2;

  unsigned int clki            : 1;

  unsigned int tcclks          : 3;
} tc_waveform_opt_t;


extern int tc_get_interrupt_settings(volatile avr32_tc_t *tc, unsigned int channel);

extern int tc_configure_interrupts(volatile avr32_tc_t *tc, unsigned int channel, const tc_interrupt_t *bitfield);

extern int tc_select_external_clock(volatile avr32_tc_t *tc, unsigned int channel, unsigned int ext_clk_sig_src);

extern int tc_init_capture(volatile avr32_tc_t *tc, const tc_capture_opt_t *opt);

extern int tc_init_waveform(volatile avr32_tc_t *tc, const tc_waveform_opt_t *opt);

extern int tc_start(volatile avr32_tc_t *tc, unsigned int channel);

extern int tc_stop(volatile avr32_tc_t *tc, unsigned int channel);

extern int tc_software_trigger(volatile avr32_tc_t *tc, unsigned int channel);

extern void tc_sync_trigger(volatile avr32_tc_t *tc);

extern void tc_sync_start(volatile avr32_tc_t *tc);

extern int tc_read_sr(volatile avr32_tc_t *tc, unsigned int channel);

extern int tc_read_tc(volatile avr32_tc_t *tc, unsigned int channel);

extern int tc_read_ra(volatile avr32_tc_t *tc, unsigned int channel);

extern int tc_read_rb(volatile avr32_tc_t *tc, unsigned int channel);

extern int tc_read_rc(volatile avr32_tc_t *tc, unsigned int channel);

extern int tc_write_ra(volatile avr32_tc_t *tc, unsigned int channel, unsigned short value);

extern int tc_write_rb(volatile avr32_tc_t *tc, unsigned int channel, unsigned short value);

extern int tc_write_rc(volatile avr32_tc_t *tc, unsigned int channel, unsigned short value);

#endif  // _TC_H_
