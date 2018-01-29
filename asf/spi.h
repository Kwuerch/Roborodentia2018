/*****************************************************************************
 *
 * \file
 *
 * \brief SPI driver for AVR32 UC3.
 *
 * This file defines a useful set of functions for the SPI interface on AVR32
 * devices.
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


#ifndef _SPI_H_
#define _SPI_H_

#include "compiler.h"

#define SPI_TIMEOUT       15000

#define SPI_MODE_0       0

#define SPI_MODE_1       1


typedef enum
{
  SPI_ERROR = -1,
  SPI_OK = 0,
  SPI_ERROR_TIMEOUT = 1,
  SPI_ERROR_ARGUMENT,
  SPI_ERROR_OVERRUN,
  SPI_ERROR_MODE_FAULT,
  SPI_ERROR_OVERRUN_AND_MODE_FAULT
} spi_status_t;

typedef struct
{
  uint8_t reg;

  uint32_t baudrate;

  uint8_t bits;
  uint8_t spck_delay;

  uint8_t trans_delay;

  uint8_t stay_act;

  uint8_t spi_mode;

  uint8_t modfdis;
} spi_options_t;

static inline void spi_reset(volatile avr32_spi_t *spi)
{
    spi->cr = AVR32_SPI_CR_SWRST_MASK;
}

static inline void spi_set_master_mode(volatile avr32_spi_t *spi)
{
    spi->MR.mstr = 1;
}

static inline void spi_set_slave_mode(volatile avr32_spi_t *spi)
{
    spi->MR.mstr = 0;
}

static inline void spi_enable_modfault(volatile avr32_spi_t *spi)
{
    spi->MR.modfdis = 0;
}

static inline void spi_disable_modfault(volatile avr32_spi_t *spi)
{
    spi->MR.modfdis = 1;
}

static inline void spi_enable_loopback(volatile avr32_spi_t *spi)
{
    spi->MR.llb = 1;
}

static inline void spi_disable_loopback(volatile avr32_spi_t *spi)
{
    spi->MR.llb = 0;
}

static inline void spi_enable_chipselect_decoding(volatile avr32_spi_t *spi)
{
    spi->MR.pcsdec = 1;
}

static inline void spi_disable_chipselect_decoding(volatile avr32_spi_t *spi)
{
    spi->MR.pcsdec = 0;
}

static inline void spi_set_chipselect(volatile avr32_spi_t *spi,uint8_t chip_select)
{
    spi->MR.pcs = chip_select;
}

static inline void spi_enable_variable_chipselect(volatile avr32_spi_t *spi)
{
    spi->MR.ps = 1;
}

static inline void spi_disable_variable_chipselect(volatile avr32_spi_t *spi)
{
    spi->MR.ps = 0;
}

static inline void spi_set_delay(volatile avr32_spi_t *spi,uint8_t delay)
{
    spi->MR.dlybcs = delay;
}

static inline void spi_set_chipselect_delay_bct(volatile avr32_spi_t *spi,
                        uint8_t chip_select, uint8_t delay)
{
  Assert(chip_select <= 3);
  switch(chip_select) {
    case 0:
      spi->CSR0.dlybct = delay;
      break;
    case 1:
      spi->CSR1.dlybct  = delay;
      break;
    case 2:
      spi->CSR2.dlybct  = delay;
      break;
    case 3:
      spi->CSR3.dlybct  = delay;
      break;
  }
}

static inline void spi_set_chipselect_delay_bs(volatile avr32_spi_t *spi,
                        uint8_t chip_select, uint8_t delay)
{
  Assert(chip_select <= 3);
  switch(chip_select) {
    case 0:
      spi->CSR0.dlybs = delay;
      break;
    case 1:
      spi->CSR1.dlybs  = delay;
      break;
    case 2:
      spi->CSR2.dlybs  = delay;
      break;
    case 3:
      spi->CSR3.dlybs  = delay;
      break;
  }
}

static inline void spi_set_bits_per_transfer(volatile avr32_spi_t *spi, uint8_t chip_select,
                        uint8_t len)
{
  Assert((len >= 8) && (len <= 16));
  switch(chip_select) {
    case 0:
      spi->CSR0.bits = len - 8;
      break;
    case 1:
      spi->CSR1.bits  = len - 8;
      break;
    case 2:
      spi->CSR2.bits  = len - 8;
      break;
    case 3:
      spi->CSR3.bits  = len - 8;
      break;
  }
}

static inline void spi_set_baudrate_register(volatile avr32_spi_t *spi,uint8_t chip_select,
                        uint8_t scbr)
{
  switch(chip_select) {
    case 0:
      spi->CSR0.scbr = scbr;
      break;
    case 1:
      spi->CSR1.scbr  = scbr;
      break;
    case 2:
      spi->CSR2.scbr  = scbr;
      break;
    case 3:
      spi->CSR3.scbr  = scbr;
      break;
  }
}

static inline void spi_enable_active_mode(volatile avr32_spi_t *spi,uint8_t chip_select)
{
  switch(chip_select) {
    case 0:
      spi->CSR0.csaat = 1;
      break;
    case 1:
      spi->CSR1.csaat  = 1;
      break;
    case 2:
      spi->CSR2.csaat  = 1;
      break;
    case 3:
      spi->CSR3.csaat  = 1;
      break;
  }
}

static inline void spi_set_mode(volatile avr32_spi_t *spi,uint8_t chip_select,
                        uint8_t flags)
{
  switch(chip_select) {
    case 0:
      spi->CSR0.cpol = flags >> 1;
      spi->CSR0.ncpha = (flags & 0x1) ^ 0x1;
      break;
    case 1:
      spi->CSR1.cpol  = flags >> 1;
      spi->CSR1.ncpha = (flags & 0x1) ^ 0x1;
      break;
    case 2:
      spi->CSR2.cpol  = flags >> 1;
      spi->CSR2.ncpha = (flags & 0x1) ^ 0x1;
      break;
    case 3:
      spi->CSR3.cpol  = flags >> 1;
      spi->CSR3.ncpha = (flags & 0x1) ^ 0x1;
      break;
  }
}

static inline void spi_put(volatile avr32_spi_t *spi, uint16_t data)
{
    spi->tdr = data << AVR32_SPI_TDR_TD_OFFSET;
}

static inline uint16_t spi_get(volatile avr32_spi_t *spi)
{
    return (spi->rdr >> AVR32_SPI_RDR_RD_OFFSET);
}

static inline bool spi_is_tx_empty(volatile avr32_spi_t *spi)
{
  return (spi->sr & AVR32_SPI_SR_TXEMPTY_MASK) != 0;
}

static inline bool spi_is_tx_ready(volatile avr32_spi_t *spi)
{
  return (spi->sr & AVR32_SPI_SR_TDRE_MASK) != 0;
}

static inline bool spi_is_rx_full(volatile avr32_spi_t *spi)
{
  return (spi->sr & AVR32_SPI_SR_RDRF_MASK) != 0;
}

static inline bool spi_is_rx_ready(volatile avr32_spi_t *spi)
{
  return (spi->sr & (AVR32_SPI_SR_RDRF_MASK | AVR32_SPI_SR_TXEMPTY_MASK)) ==
         (AVR32_SPI_SR_RDRF_MASK | AVR32_SPI_SR_TXEMPTY_MASK);
}

extern void spi_reset(volatile avr32_spi_t *spi);

extern spi_status_t spi_initSlave(volatile avr32_spi_t *spi,
                                  uint8_t bits,
                                  uint8_t spi_mode);

extern spi_status_t spi_initTest(volatile avr32_spi_t *spi);

extern spi_status_t spi_initMaster(volatile avr32_spi_t *spi, const spi_options_t *options);

extern int16_t getBaudDiv(const unsigned int baudrate, uint32_t pb_hz);

extern spi_status_t spi_selectionMode(volatile avr32_spi_t *spi,
                                      uint8_t variable_ps,
                                      uint8_t pcs_decode,
                                      uint8_t delay);
extern spi_status_t spi_selectChip(volatile avr32_spi_t *spi, unsigned char chip);

extern spi_status_t spi_unselectChip(volatile avr32_spi_t *spi, unsigned char chip);

extern spi_status_t spi_setupChipReg(volatile avr32_spi_t *spi,
                                     const spi_options_t *options,
                                     uint32_t pb_hz);
extern void spi_enable(volatile avr32_spi_t *spi);

extern void spi_disable(volatile avr32_spi_t *spi);

extern int spi_is_enabled(volatile avr32_spi_t *spi);

extern unsigned char spi_writeRegisterEmptyCheck(volatile avr32_spi_t *spi);

extern spi_status_t spi_write(volatile avr32_spi_t *spi, uint16_t data);

extern spi_status_t spi_variableSlaveWrite(volatile avr32_spi_t *spi,
                                           uint16_t data,
                                           uint8_t pcs,
                                           uint8_t lastxfer);

extern unsigned char spi_writeEndCheck(volatile avr32_spi_t *spi);

extern unsigned char spi_readRegisterFullCheck(volatile avr32_spi_t *spi);

extern spi_status_t spi_read(volatile avr32_spi_t *spi, uint16_t *data);

extern unsigned char spi_getStatus(volatile avr32_spi_t *spi);

#endif  // _SPI_H_
