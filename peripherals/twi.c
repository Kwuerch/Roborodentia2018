#include <stdlib.h>
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

    // set clock waveform generator register
    twim->cwgr = ((f_prescaled/2) << AVR32_TWIM_CWGR_LOW_OFFSET)
            | ((f_prescaled - f_prescaled/2) << AVR32_TWIM_CWGR_HIGH_OFFSET)
            | (cwgr_exp << AVR32_TWIM_CWGR_EXP_OFFSET)
            | (2     << AVR32_TWIM_CWGR_DATA_OFFSET)
            | ((1 + f_prescaled) << AVR32_TWIM_CWGR_STASTO_OFFSET);
}


#include <stdio.h>
char num[100];
TWI_STATUS twi_run(volatile avr32_twim_t* twim, TWI_MODE mode, uint8_t* data, int nbytes){
    TWI_STATUS ts = TWI_OK;
    // Enable master transfer
    twim->cr = AVR32_TWIM_CR_MEN_MASK;

    while(1){
        if(twim->sr & (AVR32_TWIM_SR_NAK_MASK | AVR32_TWIM_SR_TXRDY_MASK | AVR32_TWIM_SR_RXRDY_MASK | AVR32_TWIM_SR_CCOMP_MASK)){
            uint32_t status = twim->sr;

            snprintf(num, 100, "NBYTES: %i\r\n", nbytes);
            console_print_str(num);
            int rbts = (twim->cmdr & AVR32_TWIM_CMDR_NBYTES_MASK) >> AVR32_TWIM_CMDR_NBYTES_OFFSET;

            if(rbts < nbytes){
                ts = TWI_TIMEOUT;
                break;
            }else if(rbts > nbytes){
                continue;
            }

            if (status & AVR32_TWIM_SR_NAK_MASK) {
                twim->CMDR.valid = 0;
                ts = TWI_NAK;
                break;

            } else if (status & AVR32_TWIM_SR_CCOMP_MASK){
                ts = nbytes == 0 ? TWI_OK : TWI_CUTOFF;
                break;

            } else if (status & AVR32_TWIM_SR_RXRDY_MASK) {
                if(mode == TWI_WRITE){
                    twim->CMDR.valid = 0;
                    ts = TWI_RXRDY_TX;
                    break;
                }

                console_print_str("RXRDY\r\n");
                *data = twim->rhr;
                data++;
                nbytes--;

            } else if (status & AVR32_TWIM_SR_TXRDY_MASK) {
                twim->scr |= AVR32_TWIM_SR_TXRDY_MASK;
                if(mode == TWI_READ){
                    twim->CMDR.valid = 0;
                    ts = TWI_TXRDY_RX;
                    continue; 
                }

                console_print_str("TXRDY\r\n");
                twim->thr = *data++;
                nbytes--;
            }
        }

        console_print_str("Waiting\r\n");
    }

    if(ts != TWI_OK){
        twi_print_status(ts);
    }

    twim->cr = AVR32_TWIM_CR_MDIS_MASK;
    return ts;
}

TWI_STATUS twi_write_reg(unsigned char saddr, unsigned char reg, unsigned char* data, int nbytes){
    struct avr32_twim_t* twim = (avr32_twim_t*)AVR32_TWIM0_ADDRESS;

    // Reset the TWIM module to clear the THR register
    twim->cr = AVR32_TWIM_CR_MEN_MASK;
    twim->cr = AVR32_TWIM_CR_SWRST_MASK;
    twim->cr = AVR32_TWIM_CR_MDIS_MASK;

    // set the command to start the transfer
    // Writing reg adds 1 to number of bytes to write
    twim->cmdr = (saddr << AVR32_TWIM_CMDR_SADR_OFFSET)
            | ((nbytes + 1) << AVR32_TWIM_CMDR_NBYTES_OFFSET)
            | (AVR32_TWIM_CMDR_VALID_MASK)
            | (AVR32_TWIM_CMDR_START_MASK)
            | (AVR32_TWIM_CMDR_STOP_MASK)
            | (0 << AVR32_TWIM_CMDR_READ_OFFSET);

    // TODO - Need to write reg number first - This is not a good way to do it
    unsigned char newDat[nbytes + 1];
    newDat[0] = reg;
    int i;
    for(i = 0; i < nbytes; i++){
        newDat[i+1] = data[i];
    }
    return twi_run(twim, TWI_WRITE, newDat, nbytes + 1); 
}

TWI_STATUS twi_write(unsigned char saddr, unsigned char* data, int nbytes){
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
            | (AVR32_TWIM_CMDR_STOP_MASK)
            | (0 << AVR32_TWIM_CMDR_READ_OFFSET);

    return twi_run(twim, TWI_WRITE, data, nbytes); 
}

TWI_STATUS twi_read_reg(unsigned char saddr, unsigned char reg, unsigned char* data, int nbytes){
    twi_write(saddr, &reg, 1);
    return twi_read(saddr, data, nbytes);
}

TWI_STATUS twi_read(unsigned char saddr, unsigned char* buf, int nbytes){
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
            | (AVR32_TWIM_CMDR_STOP_MASK)
            | (1 << AVR32_TWIM_CMDR_READ_OFFSET);


    return twi_run(twim, TWI_READ, buf, nbytes); 
}

void twi_print_status(TWI_STATUS status){
    switch(status){
        case TWI_OK:
            console_print_str("Twi transaction completed succesfully\r\n");
            break;
        case TWI_TIMEOUT:
            console_print_str("Twi transaction timed out\r\n");
            break;
        case TWI_CUTOFF:
            console_print_str("Twi CCOMP received, but not all data transmitted\r\n");
            break;
        case TWI_NAK:
            console_print_str("Twi NAK received\r\n");
            break;
        case TWI_RXRDY_TX:
            console_print_str("Twi RXRDY received when writing\r\n");
            break;
        case TWI_TXRDY_RX:
            console_print_str("Twi TXRDY received when reading\r\n");
            break;
    }
}
