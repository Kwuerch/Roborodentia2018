#include <avr32/io.h>
#include <stdint.h>

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

    while(!(twim->sr & (/**AVR32_TWIM_SR_ANAK_MASK | AVR32_TWIM_SR_DNAK_MASK |**/ AVR32_TWIM_TXRDY_MASK /**(| AVR32_TWIM_SR_CCOMP_MASK**/))){
        twim->thr = (unsigned int)*data++; 
    }

    while(!(twim->sr & (AVR32_TWIM_SR_ANAK_MASK | AVR32_TWIM_SR_DNAK_MASK /** AVR32_TWIM_TXRDY_MASK | AVR32_TWIM_SR_CCOMP_MASK**/))){
    }

    /**
    while(!(twim->sr)){
        if(twim->sr & AVR32_TWIM_SR_ANAK_MASK){
            twim->scr |=  AVR32_TWIM_SR_ANAK_MASK;
        }else if(twim->sr & AVR32_TWIM_SR_TXRDY_MASK){
            twim->thr = (unsigned int)*data++;
            nbytes--;
        }else if(twim->sr & AVR32_TWIM_SR_DNAK_MASK){

        }else if(twim->sr & AVR32_TWIM_SR_CCOMP_MASK){

        }
    

    while(nbytes > 0){
        while(!(twim->sr)){
            if(twim->sr & AVR32_TWIM_SR_ANAK_MASK){
                twim->scr |=  AVR32_TWIM_SR_ANAK_MASK;
            }else if(twim->sr & AVR32_TWIM_SR_TXRDY_MASK){
                twim->thr = (unsigned int)*data++;
                nbytes--;
            }
        }
    }
    **/
}

    /**
    AVR32_TWIM0.cr |= AVR32_TWIM_CR_MEN_MASK |
                      AVR32_TWIM_CR_SMDIS_MASK;
    AVR32_TWIM0.cr |= AVR32_TWIM_CR_MEN_MASK;
    **/
    /**

    while(!(AVR32_TWIM0.sr & AVR32_TWIM_SR_MENB));



    AVR32_TWIM0.scr |= AVR32_TWIM_SCR_CCOMP_MASK | AVR32_TWIM_SCR_ANAK_MASK | AVR32_TWIM_SCR_DNAK_MASK;

    while(!(AVR32_TWIM0.sr & AVR32_TWIM_SR_BUSFREE));

    AVR32_TWIM0.thr = (unsigned char)*data++;
    length--;

    AVR32_TWIM0.cmdr = (AVR32_TWIM_CMDR_NBYTES_MASK & (length << AVR32_TWIM_CMDR_NBYTES_OFFSET)) |
                       (AVR32_TWIM_CMDR_SADR_MASK & (addr << AVR32_TWIM_CMDR_SADR_OFFSET)) |
                       AVR32_TWIM_CMDR_VALID_MASK |
                       AVR32_TWIM_CMDR_START_MASK;

    while(length != 0){
        while(!(AVR32_TWIM0.sr & AVR32_TWIM_SR_TXRDY)){
            AVR32_GPIO.port[1].ovrt = 0x04;
            if(AVR32_TWIM0.sr & (AVR32_TWIM_SR_TOUT)){
                AVR32_GPIO.port[1].ovrs = 0x01;
                AVR32_TWIM0.scr |= AVR32_TWIM_SR_TOUT;
                AVR32_TWIM0.cr |= AVR32_TWIM_CR_MDIS_MASK;
                return;
            }
        }


        AVR32_TWIM0.thr = *(unsigned int*)data;
        data++;
        if(length-- == 1){
            AVR32_TWIM0.cr |= AVR32_TWIM_CMDR_STOP_MASK;
        }
    }
    **/

    //AVR32_TWIM0.cr |= AVR32_TWIM_CR_MDIS_MASK;
