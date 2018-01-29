/*****************************************************************************
 *
 * \file
 *
 * \brief USART driver for AVR32 UC3.
 *
 * This file contains basic functions for the AVR32 USART, with support for all
 * modes, settings and clock speeds.
 *
 * Copyright (c) 2009 Atmel Corporation. All rights reserved.
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


#ifndef _USART_H_
#define _USART_H_

#include <avr32/io.h>

#define USART_SUCCESS                 0 //!< Successful completion.
#define USART_FAILURE                -1 //!< Failure because of some unspecified reason.
#define USART_INVALID_INPUT           1 //!< Input value out of range.
#define USART_INVALID_ARGUMENT       -1 //!< Argument value out of range.
#define USART_TX_BUSY                 2 //!< Transmitter was busy.
#define USART_RX_EMPTY                3 //!< Nothing was received.
#define USART_RX_ERROR                4 //!< Transmission error occurred.
#define USART_MODE_FAULT              5 //!< USART not in the appropriate mode.


#define USART_DEFAULT_TIMEOUT         10000


#define USART_EVEN_PARITY             AVR32_USART_MR_PAR_EVEN   //!< Use even parity on character transmission.
#define USART_ODD_PARITY              AVR32_USART_MR_PAR_ODD    //!< Use odd parity on character transmission.
#define USART_SPACE_PARITY            AVR32_USART_MR_PAR_SPACE  //!< Use a space as parity bit.
#define USART_MARK_PARITY             AVR32_USART_MR_PAR_MARK   //!< Use a mark as parity bit.
#define USART_NO_PARITY               AVR32_USART_MR_PAR_NONE   //!< Don't use a parity bit.
#define USART_MULTIDROP_PARITY        AVR32_USART_MR_PAR_MULTI  //!< Parity bit is used to flag address characters.



#define USART_1_STOPBIT               AVR32_USART_MR_NBSTOP_1   //!< Use 1 stop bit.
#define USART_1_5_STOPBITS            AVR32_USART_MR_NBSTOP_1_5 //!< Use 1.5 stop bits.
#define USART_2_STOPBITS              AVR32_USART_MR_NBSTOP_2   //!< Use 2 stop bits (for more, just give the number of bits).



#define USART_NORMAL_CHMODE           AVR32_USART_MR_CHMODE_NORMAL      //!< Normal communication.
#define USART_AUTO_ECHO               AVR32_USART_MR_CHMODE_ECHO        //!< Echo data.
#define USART_LOCAL_LOOPBACK          AVR32_USART_MR_CHMODE_LOCAL_LOOP  //!< Local loopback.
#define USART_REMOTE_LOOPBACK         AVR32_USART_MR_CHMODE_REMOTE_LOOP //!< Remote loopback.


typedef struct
{
  unsigned long baudrate;

  unsigned char charlength;

  unsigned char paritytype;

  unsigned short stopbits;

  unsigned char channelmode;
} usart_options_t;

//------------------------------------------------------------------------------


extern void usart_reset(volatile avr32_usart_t *usart);

extern int usart_init_rs232(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz);

extern int usart_init_sync_master(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz);

extern int usart_init_sync_slave(volatile avr32_usart_t *usart, const usart_options_t *opt, long pba_hz);

#endif  // USART rev. >= 4.0.0



//------------------------------------------------------------------------------


#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline void usart_reset_status(volatile avr32_usart_t *usart)
{
  usart->cr = AVR32_USART_CR_RSTSTA_MASK;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_parity_error(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_PARE_MASK) != 0;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_framing_error(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_FRAME_MASK) != 0;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_overrun_error(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_OVRE_MASK) != 0;
}

#if defined(AVR32_USART_400_H_INCLUDED) || \
    defined(AVR32_USART_410_H_INCLUDED) || \
    defined(AVR32_USART_420_H_INCLUDED) || \
    defined(AVR32_USART_440_H_INCLUDED) || \
    defined(AVR32_USART_602_H_INCLUDED)

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_lin_get_error(volatile avr32_usart_t *usart)
{
  return (usart->csr & (AVR32_USART_CSR_LINSNRE_MASK |
                        AVR32_USART_CSR_LINCE_MASK |
                        AVR32_USART_CSR_LINIPE_MASK |
                        AVR32_USART_CSR_LINISFE_MASK |
                        AVR32_USART_CSR_LINBE_MASK)) >> AVR32_USART_CSR_LINBE_OFFSET;
}

//------------------------------------------------------------------------------


#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline void usart_iso7816_enable_receiver(volatile avr32_usart_t *usart)
{
  usart->cr = AVR32_USART_CR_TXDIS_MASK | AVR32_USART_CR_RXEN_MASK;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline void usart_iso7816_enable_transmitter(volatile avr32_usart_t *usart)
{
  usart->cr = AVR32_USART_CR_RXDIS_MASK | AVR32_USART_CR_TXEN_MASK;
}

extern int usart_send_address(volatile avr32_usart_t *usart, int address);

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_tx_ready(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_TXRDY_MASK) != 0;
}

extern int usart_write_char(volatile avr32_usart_t *usart, int c);

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline void usart_bw_write_char(volatile avr32_usart_t *usart, int c)
{
  while (usart_write_char(usart, c) != USART_SUCCESS);
}

extern int usart_putchar(volatile avr32_usart_t *usart, int c);

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_tx_empty(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_TXEMPTY_MASK) != 0;
}

#if (defined __GNUC__)
__attribute__((__always_inline__))
#endif
static inline int usart_test_hit(volatile avr32_usart_t *usart)
{
  return (usart->csr & AVR32_USART_CSR_RXRDY_MASK) != 0;
}

extern int usart_read_char(volatile avr32_usart_t *usart, int *c);

extern int usart_getchar(volatile avr32_usart_t *usart);

extern void usart_write_line(volatile avr32_usart_t *usart, const char *string);

extern int usart_get_echo_line(volatile avr32_usart_t *usart);

#endif  // _USART_H_
