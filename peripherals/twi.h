#ifndef TWI_H
#define TIW_H

#include <avr32/io.h>
#include <stdint.h>

#define AVR32_TWIM_SCR_NAK_MASK (AVR32_TWIM_SCR_ANAK_MASK \
        | AVR32_TWIM_SCR_DNAK_MASK)

#define AVR32_TWIM_SR_NAK_MASK  (AVR32_TWIM_SR_ANAK_MASK  \
        | AVR32_TWIM_SR_DNAK_MASK)

#define AVR32_TWIM_SR_STD_MASK  (AVR32_TWIM_SR_NAK_MASK  \
        | AVR32_TWIM_SR_ARBLST_MASK)


void twi_enable(volatile avr32_twim_t* twim, uint32_t speed, uint32_t pba_hz);
void twi_write(unsigned char saddr, unsigned char* data, int length);
void twi_read(unsigned char saddr, unsigned char* buf, int length);

#endif
