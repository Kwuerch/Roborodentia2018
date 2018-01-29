/*****************************************************************************
 *
 * \file
 *
 * \brief SPI Master driver for AVR UC3.
 *
 * This file defines a useful set of functions for the SPI interface on AVR UC3
 * devices.
 *
 * Copyright (c) 2009-2011 Atmel Corporation. All rights reserved.
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


#ifndef _SPI_MASTER_H_
#define _SPI_MASTER_H_

#include "compiler.h"
#include "status_codes.h"
#include "spi.h"

#define CONFIG_SPI_MASTER_DELAY_BCS                  0
#define CONFIG_SPI_MASTER_BITS_PER_TRANSFER          8
#define CONFIG_SPI_MASTER_DELAY_BCT                  0
#define CONFIG_SPI_MASTER_DELAY_BS                   0
#define CONFIG_SPI_MASTER_DUMMY                      0xFF

typedef uint8_t spi_flags_t;

typedef uint8_t board_spi_select_id_t;

struct spi_device {
    board_spi_select_id_t   id;
};

static inline void spi_master_init(volatile avr32_spi_t *spi)
{
   /**
    sysclk_enable_pba_module(SYSCLK_SPI);
    sysclk_enable_pbc_module(SYSCLK_SPI0);
    **/

    spi_reset(spi);
    spi_set_master_mode(spi);
    spi_disable_modfault(spi);
    spi_disable_loopback(spi);
    spi_set_chipselect(spi,(1 << AVR32_SPI_MR_PCS_SIZE) - 1);
    spi_disable_variable_chipselect(spi);
    spi_disable_chipselect_decoding(spi);
    spi_set_delay(spi,CONFIG_SPI_MASTER_DELAY_BCS);
}

extern void spi_master_setup_device(volatile avr32_spi_t *spi,
        struct spi_device *device, spi_flags_t flags, uint32_t baud_rate,
        board_spi_select_id_t sel_id);

extern void spi_enable(volatile avr32_spi_t *spi);

extern void spi_disable(volatile avr32_spi_t *spi);

extern int spi_is_enabled(volatile avr32_spi_t *spi);

static inline void spi_select_device(volatile avr32_spi_t *spi,
        struct spi_device *device)
{
    spi_selectChip(spi,device->id);
}

static inline void spi_deselect_device(volatile avr32_spi_t *spi,
        struct spi_device *device)
{
    spi_unselectChip(spi,device->id);
}

static inline void spi_write_single(volatile avr32_spi_t *spi, uint8_t data)
{
    spi_put(spi,(uint16_t)data);
}

extern status_code_t spi_write_packet(volatile avr32_spi_t *spi,
        const uint8_t *data, size_t len);

static inline void spi_read_single(volatile avr32_spi_t *spi, uint8_t *data)
{
    *data = (uint8_t)spi_get(spi);
}

extern status_code_t spi_read_packet(volatile avr32_spi_t *spi,
        uint8_t *data, size_t len);

extern bool spi_is_tx_empty(volatile avr32_spi_t *spi);

extern bool spi_is_tx_ready(volatile avr32_spi_t *spi);

extern bool spi_is_rx_full(volatile avr32_spi_t *spi);

extern bool spi_is_rx_ready(volatile avr32_spi_t *spi);

#endif  // _SPI_MASTER_H_
