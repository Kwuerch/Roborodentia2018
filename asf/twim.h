/*****************************************************************************
 *
 * \file
 *
 * \brief TWIM driver for AVR32 UC3.
 *
 * This file defines a useful set of functions for TWIM on AVR32 devices.
 *
 *****************************************************************************/

#ifndef _TWIM_H_
#define _TWIM_H_

#include <avr32/io.h>
#include <stdint.h>
#include "compiler.h"
#include "status_codes.h"
#include "conf_twim.h"
#include "interrupt/interrupt_avr32.h"

#define TWI_STD_MODE_SPEED      (100000 /* kbit/s */)

#define TWI_FAST_MODE_SPEED     (400000 /* kbit/s */)


#define AVR32_TWIM_SCR_NAK_MASK (AVR32_TWIM_SCR_ANAK_MASK \
        | AVR32_TWIM_SCR_DNAK_MASK)

#define AVR32_TWIM_SR_NAK_MASK  (AVR32_TWIM_SR_ANAK_MASK  \
        | AVR32_TWIM_SR_DNAK_MASK)

#define AVR32_TWIM_IER_NAK_MASK (AVR32_TWIM_IER_ANAK_MASK \
        | AVR32_TWIM_IER_DNAK_MASK)

#define AVR32_TWIM_IER_STD_MASK (AVR32_TWIM_IER_NAK_MASK  \
        | AVR32_TWIM_IER_ARBLST_MASK)

#define AVR32_TWIM_SR_STD_MASK  (AVR32_TWIM_SR_NAK_MASK  \
        | AVR32_TWIM_SR_ARBLST_MASK)

#define avr32_twi_t avr32_twim_t
#define twi_options_t twim_options_t
#define twi_package_t twim_package_t
#define twi_master_init twim_master_init
#define twi_probe twim_probe


enum twim_transfer_status{
    TWI_SUCCESS = 0,            
    TWI_INVALID_ARGUMENT = -1,  
    TWI_ARBITRATION_LOST = -2,  
    TWI_NO_CHIP_FOUND = -3,     
    TWI_RECEIVE_NACK = -4,      
    TWI_SEND_NACK = -5,         
    TWI_INVALID_CLOCK_DIV = -6  
};

typedef enum twim_transfer_status twim_transfer_status_t;
/* @} */

typedef struct
{
    uint32_t pba_hz;
    uint32_t speed;
    uint32_t chip;
    bool smbus;
}
twim_options_t;

typedef struct
{
    uint32_t chip;
    uint8_t addr[3];
    uint8_t addr_length;
    void *buffer;
    uint32_t length;
}
twim_package_t;

typedef struct
{
    uint32_t chip;
    void *buffer;
    uint32_t length;
    bool read;
}
twim_transfer_t;

static inline void twim_master_enable (volatile avr32_twim_t *twim)
{
    twim->cr = AVR32_TWIM_CR_MEN_MASK;;
}

static inline void twim_master_disable (volatile avr32_twim_t *twim)
{
    twim->cr = AVR32_TWIM_CR_MDIS_MASK;;
}

extern status_code_t twim_master_init (volatile avr32_twim_t *twim,
        const twim_options_t *opt);

extern status_code_t twim_set_speed (volatile avr32_twim_t *twim,
        uint32_t speed, uint32_t pba_hz);

extern status_code_t twim_probe (volatile avr32_twim_t *twim,
        uint32_t chip_addr);

extern status_code_t twim_read_packet (volatile avr32_twim_t *twim,
        const twim_package_t *package);

extern status_code_t twim_write_packet (volatile avr32_twim_t *twim,
        const twim_package_t *package);

extern status_code_t twim_read (volatile avr32_twim_t *twim, uint8_t *buffer,
        uint32_t nbytes, uint32_t saddr, bool tenbit);

extern status_code_t twim_write (volatile avr32_twim_t *twim,
        const uint8_t *buffer, uint32_t nbytes, uint32_t saddr,
        bool tenbit);

extern status_code_t twim_chained_transfer (volatile avr32_twim_t *twim,
        volatile twim_transfer_t *first,
        volatile twim_transfer_t *second,
        bool tenbit);

static inline status_code_t twi_master_read (volatile avr32_twi_t *twim,
        const twi_package_t *package)
{
    return (twim_read_packet (twim, package));
}

static inline status_code_t twi_master_write (volatile avr32_twi_t *twim,
        const twi_package_t *package)
{
    return (twim_write_packet (twim, package));
}

extern void twim_disable_interrupt (volatile avr32_twim_t *twim);

extern uint8_t twim_status ( void );

#endif // _TWI_H_
