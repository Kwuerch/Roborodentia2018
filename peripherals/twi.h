#ifndef TWI_H
#define TIW_H

#include <avr32/io.h>
#include <stdint.h>

void twi_enable(volatile avr32_twim_t* twim, uint32_t speed, uint32_t pba_hz);
void twi_write(unsigned char saddr, unsigned char* data, int length);

#endif
