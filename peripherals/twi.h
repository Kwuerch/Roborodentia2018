#ifndef TWI_H
#define TIW_H

#include <avr32/io.h>
#include <stdint.h>

#define TWI_TIMEOUT_CONST 200

#define AVR32_TWIM_SCR_NAK_MASK (AVR32_TWIM_SCR_ANAK_MASK \
        | AVR32_TWIM_SCR_DNAK_MASK)

#define AVR32_TWIM_SR_NAK_MASK  (AVR32_TWIM_SR_ANAK_MASK  \
        | AVR32_TWIM_SR_DNAK_MASK)

#define AVR32_TWIM_SR_STD_MASK  (AVR32_TWIM_SR_NAK_MASK  \
        | AVR32_TWIM_SR_ARBLST_MASK)

typedef enum{
   TWI_OK = 0,
   TWI_TIMEOUT,
   TWI_CUTOFF,
   TWI_NAK,
   TWI_RXRDY_TX, /** Receive RXRDY on transmit **/
   TWI_TXRDY_RX /** Receive TXRDY on transmit **/
   
}TWI_STATUS;

typedef enum{
   TWI_READ = 0,
   TWI_WRITE
}TWI_MODE;


void twi_enable(volatile avr32_twim_t* twim, uint32_t speed, uint32_t pba_hz);
TWI_STATUS twi_write_reg(unsigned char saddr, unsigned char reg, unsigned char* data, int nbytes);
TWI_STATUS twi_read_reg(unsigned char saddr, unsigned char reg, unsigned char* data, int nbytes);
TWI_STATUS twi_write(unsigned char saddr, unsigned char* data, int nbytes);
TWI_STATUS twi_read(unsigned char saddr, unsigned char* buf, int nbytes);

void  twi_print_status(TWI_STATUS status);

#endif
