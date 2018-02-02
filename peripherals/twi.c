#include "console.h"
#include "twi.h"

// twim_110
void twi_enable(volatile avr32_twim_t* twim, uint32_t speed, uint32_t pba_hz ){
    uint32_t f_prescaled;
    uint8_t cwgr_exp = 0;
    f_prescaled = (pba_hz / speed / 2);
    // f_prescaled must fit in 8 bits, cwgr_exp must fit in 3 bits
    while ((f_prescaled > 0xFF) && (cwgr_exp <= 0x7)) {
        // increase clock divider
        cwgr_exp++;
        // divide f_prescaled value
        f_prescaled /= 2;
    }
    /**
    if (cwgr_exp > 0x7) {
        return ERR_INVALID_ARG;
    }
    */
    // set clock waveform generator register
    twim->cwgr = ((f_prescaled/2) << AVR32_TWIM_CWGR_LOW_OFFSET)
            | ((f_prescaled - f_prescaled/2) << AVR32_TWIM_CWGR_HIGH_OFFSET)
            | (cwgr_exp << AVR32_TWIM_CWGR_EXP_OFFSET)
            | (1     << AVR32_TWIM_CWGR_DATA_OFFSET)
            | (f_prescaled << AVR32_TWIM_CWGR_STASTO_OFFSET);
}

void twi_read_reg(unsigned char saddr, unsigned char reg, unsigned char* data, int nbytes){
    twi_write(saddr, &reg, 1);
    twi_read(saddr, data, nbytes);
}

void twi_write_reg(unsigned char saddr, unsigned char reg, unsigned char* data, int nbytes){
    struct avr32_twim_t* twim = (avr32_twim_t*)AVR32_TWIM0_ADDRESS;
    int wrote_reg = 0;

    // Reset the TWIM module to clear the THR register
    twim->cr = AVR32_TWIM_CR_MEN_MASK;
    twim->cr = AVR32_TWIM_CR_SWRST_MASK;
    twim->cr = AVR32_TWIM_CR_MDIS_MASK;

    // set the command to start the transfer
    twim->cmdr = (saddr << AVR32_TWIM_CMDR_SADR_OFFSET)
            | (nbytes << AVR32_TWIM_CMDR_NBYTES_OFFSET)
            | (AVR32_TWIM_CMDR_VALID_MASK)
            | (AVR32_TWIM_CMDR_START_MASK)
            | ((nbytes == 0 ? 1 : 0) << (AVR32_TWIM_CMDR_STOP))
            | (0 << AVR32_TWIM_CMDR_READ_OFFSET);

    // Enable master transfer
    twim->cr = AVR32_TWIM_CR_MEN_MASK;

    while(1){
        if(twim->sr & (AVR32_TWIM_SR_NAK_MASK | AVR32_TWIM_SR_TXRDY_MASK | AVR32_TWIM_SR_RXRDY_MASK | AVR32_TWIM_SR_CCOMP_MASK)){
            uint32_t status = twim->sr;

            if (status & AVR32_TWIM_SR_NAK_MASK) {
                twim->scr |= AVR32_TWIM_SR_NAK_MASK;

                console_print_str("NAK Received\r\n"); 
                twim->CMDR.valid = 0;
                break;
            } else if (status & AVR32_TWIM_SR_CCOMP_MASK){
                console_print_str("CCOMP\r\n");
                break;
            } else if (status & AVR32_TWIM_SR_RXRDY_MASK) {
                console_print_str("RXRDY on write\r\n");
                twim->scr |= AVR32_TWIM_SR_RXRDY_MASK;
                break;
            } else if (status & AVR32_TWIM_SR_TXRDY_MASK) {
                // no more bytes to transmit
                if (nbytes == 0) {
                    // finish the receive operation
                    twim->cmdr |= AVR32_TWIM_SR_STOP_MASK;
                }else if(!wrote_reg){
                    twim->thr = reg;
                    nbytes--;
                    wrote_reg = 1;
                }else {
                    // put the byte in the Transmit Holding Register
                    twim->thr = *data++;
                    nbytes--;
                }
                twim->scr |= AVR32_TWIM_SR_TXRDY_MASK;
                console_print_str("TXRDY\r\n");
            }

        }
    }

    twim->cr = AVR32_TWIM_CR_MDIS_MASK;
}

void twi_write(unsigned char saddr, unsigned char* data, int nbytes){
    struct avr32_twim_t* twim = (avr32_twim_t*)AVR32_TWIM0_ADDRESS;

    // Reset the TWIM module to clear the THR register
    twim->cr = AVR32_TWIM_CR_MEN_MASK;
    twim->cr = AVR32_TWIM_CR_SWRST_MASK;
    twim->cr = AVR32_TWIM_CR_MDIS_MASK;

    // set the command to start the transfer
    twim->cmdr = (saddr << AVR32_TWIM_CMDR_SADR_OFFSET)
            | (nbytes << AVR32_TWIM_CMDR_NBYTES_OFFSET)
            | (AVR32_TWIM_CMDR_VALID_MASK)
            | (AVR32_TWIM_CMDR_START_MASK)
            | ((nbytes == 0 ? 1 : 0) << (AVR32_TWIM_CMDR_STOP))
            | (0 << AVR32_TWIM_CMDR_READ_OFFSET);

    // Enable master transfer
    twim->cr = AVR32_TWIM_CR_MEN_MASK;

    while(1){
        if(twim->sr & (AVR32_TWIM_SR_NAK_MASK | AVR32_TWIM_SR_TXRDY_MASK | AVR32_TWIM_SR_RXRDY_MASK | AVR32_TWIM_SR_CCOMP_MASK)){
            uint32_t status = twim->sr;

            if (status & AVR32_TWIM_SR_NAK_MASK) {
                twim->scr |= AVR32_TWIM_SR_NAK_MASK;

                console_print_str("NAK Received\r\n"); 
                twim->CMDR.valid = 0;
                break;
            } else if (status & AVR32_TWIM_SR_CCOMP_MASK){
                console_print_str("CCOMP\r\n");
                break;
            } else if (status & AVR32_TWIM_SR_RXRDY_MASK) {
                console_print_str("RXRDY on write\r\n");
                twim->scr |= AVR32_TWIM_SR_RXRDY_MASK;
                break;
            } else if (status & AVR32_TWIM_SR_TXRDY_MASK) {
                // no more bytes to transmit
                if (nbytes == 0) {
                    // finish the receive operation
                    twim->cmdr |= AVR32_TWIM_SR_STOP_MASK;
                } else {
                    // put the byte in the Transmit Holding Register
                    twim->thr = *data++;
                    nbytes--;
                }
                twim->scr |= AVR32_TWIM_SR_TXRDY_MASK;
                console_print_str("TXRDY\r\n");
            }

        }
    }

    twim->cr = AVR32_TWIM_CR_MDIS_MASK;
}

void twi_read(unsigned char saddr, unsigned char* buf, int nbytes){
    struct avr32_twim_t* twim = (avr32_twim_t*)AVR32_TWIM0_ADDRESS;

    // Reset the TWIM module to clear the RHR register
    twim->cr = AVR32_TWIM_CR_MEN_MASK;
    twim->cr = AVR32_TWIM_CR_SWRST_MASK;
    twim->cr = AVR32_TWIM_CR_MDIS_MASK;

    // set the command to start the transfer
    twim->cmdr = (saddr << AVR32_TWIM_CMDR_SADR_OFFSET)
            | (nbytes << AVR32_TWIM_CMDR_NBYTES_OFFSET)
            | (AVR32_TWIM_CMDR_VALID_MASK)
            | (AVR32_TWIM_CMDR_START_MASK)
            | ((nbytes == 0 ? 1 : 0) << (AVR32_TWIM_CMDR_STOP))
            | (1 << AVR32_TWIM_CMDR_READ_OFFSET);

    // Enable master transfer
    twim->cr = AVR32_TWIM_CR_MEN_MASK;

    while(1){
        if(twim->sr & (AVR32_TWIM_SR_NAK_MASK | AVR32_TWIM_SR_TXRDY_MASK | AVR32_TWIM_SR_RXRDY_MASK | AVR32_TWIM_SR_CCOMP_MASK)){
            uint32_t status = twim->sr;

            if (status & AVR32_TWIM_SR_NAK_MASK) {
                twim->scr |= AVR32_TWIM_SR_NAK_MASK;

                console_print_str("NAK Received\r\n");

                twim->CMDR.valid = 0;
                break;
            } else if (status & AVR32_TWIM_SR_CCOMP_MASK){
                console_print_str("CCOMP\r\n");
                break;
            } else if (status & AVR32_TWIM_SR_RXRDY_MASK) {

                /** TODO - SHOULD THIS BE AFTER ADDING 
                 * last bytes? - SAME FOR TXRDY in write !!!???
                 * */
                if(nbytes == 0){
                    twim->cmdr |= AVR32_TWIM_SR_STOP_MASK;
                }else{
                    *buf = twim->rhr;
                    buf++;
                    nbytes--;
                }

                console_print_str("RXRDY\r\n");
                twim->scr |= AVR32_TWIM_SR_RXRDY_MASK;
            } else if (status & AVR32_TWIM_SR_TXRDY_MASK) {
                console_print_str("TXRDY on read\r\n");
                twim->scr |= AVR32_TWIM_SR_TXRDY_MASK;
                break;
            }
        }
    }

    twim->cr = AVR32_TWIM_CR_MDIS_MASK;
}
