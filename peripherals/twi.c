#include <avr32/io.h>
#include <stdint.h>
#include "../drivers/console.h"

// twim_110

#define AVR32_TWIM_SCR_NAK_MASK (AVR32_TWIM_SCR_ANAK_MASK \
        | AVR32_TWIM_SCR_DNAK_MASK)

#define AVR32_TWIM_SR_NAK_MASK  (AVR32_TWIM_SR_ANAK_MASK  \
        | AVR32_TWIM_SR_DNAK_MASK)

#define AVR32_TWIM_SR_STD_MASK  (AVR32_TWIM_SR_NAK_MASK  \
        | AVR32_TWIM_SR_ARBLST_MASK)

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
            | (0     << AVR32_TWIM_CWGR_DATA_OFFSET)
            | (f_prescaled << AVR32_TWIM_CWGR_STASTO_OFFSET);
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
            console_print_str("Enter If Loop\r\n");

            // this is a NACK
            if (status & AVR32_TWIM_SR_NAK_MASK) {
                twim->scr |= AVR32_TWIM_SR_NAK_MASK;
                //if we get a nak, clear the valid bit in cmdr,
                //otherwise the command will be resent.
                console_print_str("NAK Received\r\n");

                twim->CMDR.valid = 0;
                break;
            } // this is a RXRDY
            else if (status & AVR32_TWIM_SR_CCOMP_MASK){
                console_print_str("CCOMP\r\n");
                break;
            }
            else if (status & AVR32_TWIM_SR_RXRDY_MASK) {
                twim->scr |= AVR32_TWIM_SR_RXRDY_MASK;
                /**
                // get data from Receive Holding Register
                *twim_rx_data = twim_inst->rhr;
                twim_rx_data++;
                // decrease recieved bytes number
                twim_rx_nb_bytes--;
                // receive complete
                if (twim_rx_nb_bytes == 0) {
                    // finish the receive operation
                    twim_inst->idr = AVR32_TWIM_IDR_RXRDY_MASK;
                    // set busy to false
                    twim_next = false;
                }
                **/
                console_print_str("RXRDY\r\n");
            } // this is a TXRDY
            else if (status & AVR32_TWIM_SR_TXRDY_MASK) {
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
