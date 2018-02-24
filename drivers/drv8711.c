#include "drv8711.h"
#include "spi_master.h"
#include "board.h"
#include "console.h"
#include "delay.h"
#include <avr32/io.h>

#define AVR32_SPI_CSR0_BITS_4_BPT 0x09

#define ReadMaskedData(data, mask, offset) ((data & mask) >> offset)
#define MaskOffsetData(data, mask, offset) ((data << offset) & mask)

 /*------------------------------------------Write Message---------------------------------------------------------\
 *     *      *     *      *       *       *      *      *      *      *      *      *      *      *      *      *
 *  0  *  A2  * A1  *  A0  *  D11  *  D10  *  D9  *  D8  *  D7  *  D6  *  D5  *  D4  *  D3  *  D2  *  D1  *  D0  *
 *     *      *     *      *       *       *      *      *      *      *      *      *      *      *      *      *
 \----------------------------------------------------------------------------------------------------------------*/

 /*-------------------------------------------Read Message---------------------------------------------------------\
 *     *      *     *      *       *       *      *      *      *      *      *      *      *      *      *      *
 *  1  *  A2  * A1  *  A0  *  D11  *  D10  *  D9  *  D8  *  D7  *  D6  *  D5  *  D4  *  D3  *  D2  *  D1  *  D0  *
 *     *      *     *      *       *       *      *      *      *      *      *      *      *      *      *      *
 \----------------------------------------------------------------------------------------------------------------*/


uint16_t drv8711_read_reg(uint8_t csid, uint8_t reg){
    avr32_spi_t* spi = (avr32_spi_t*)AVR32_SPI0_ADDRESS;

    spi->mr &= ~AVR32_SPI_MR_PCS_MASK;
    spi->mr |= AVR32_SPI_MR_PCS_MASK & (csid << AVR32_SPI_MR_PCS_OFFSET);


    volatile unsigned long* csrReg;
    uint32_t pin;
    switch(csid){
        case 0:
            csrReg = &(spi->csr0);
            pin = SPI0_NPCS_0_PIN;
            break;
        case 1:
            csrReg = &(spi->csr1);
            pin = SPI0_NPCS_1_PIN;
            break;
        case 2:
            csrReg = &(spi->csr2);
            pin = SPI0_NPCS_2_PIN;
            break;
        case 3:
            csrReg = &(spi->csr3);
            pin = SPI0_NPCS_3_PIN;
            break;
        default:
            console_print_str("bad CSID\r\n");
            return 0;
    }
    
    *csrReg &= ~AVR32_SPI_CSR0_BITS_MASK; 
    *csrReg |= AVR32_SPI_CSR0_BITS_MASK & (AVR32_SPI_CSR0_BITS_16_BPT << AVR32_SPI_CSR0_BITS_OFFSET);

    /** Dummy Read - This will clear RDRF Flag in SR**/
    uint16_t data = spi->rdr & AVR32_SPI_RDR_RD_MASK;


    AVR32_GPIO.port[SPI0_PORT].ovrs = pin;

    unsigned int timeout;
    timeout = SPI_TIMEOUT;
    while (!spi_is_tx_ready(spi)) {
        if (!timeout--) {
            AVR32_GPIO.port[SPI0_PORT].ovrc = pin;
            return 0;
        }
    }

    spi->tdr = ((0x8) | reg) << 12 << AVR32_SPI_TDR_TD_OFFSET;

    while (!(spi->sr & AVR32_SPI_SR_TXEMPTY_MASK)) {
        if (!timeout--) {
            console_print_str("TIMEOUT\r\n");
            AVR32_GPIO.port[SPI0_PORT].ovrc = pin;
            return 0;
        }
    }
    
    data = spi->rdr & AVR32_SPI_RDR_RD_MASK;

    AVR32_GPIO.port[SPI0_PORT].ovrc = (pin);

    return data;
}


void drv8711_write_reg(uint8_t csid, uint8_t reg, uint16_t data){
    avr32_spi_t* spi = (avr32_spi_t*)AVR32_SPI0_ADDRESS;

    spi->mr &= ~AVR32_SPI_MR_PCS_MASK;
    spi->mr |= AVR32_SPI_MR_PCS_MASK & (csid << AVR32_SPI_MR_PCS_OFFSET);

    volatile unsigned long* csrReg;
    uint32_t pin;
    switch(csid){
        case 0:
            csrReg = &(spi->csr0);
            pin = SPI0_NPCS_0_PIN;
            break;
        case 1:
            csrReg = &(spi->csr1);
            pin = SPI0_NPCS_1_PIN;
            break;
        case 2:
            csrReg = &(spi->csr2);
            pin = SPI0_NPCS_2_PIN;
            break;
        case 3:
            csrReg = &(spi->csr3);
            pin = SPI0_NPCS_3_PIN;
            break;
        default:
            console_print_str("bad CSID\r\n");
            return;
    }

    /** Set to 16 bit mode **/
    *csrReg &= ~AVR32_SPI_CSR0_BITS_MASK; 
    *csrReg |= AVR32_SPI_CSR0_BITS_MASK & (AVR32_SPI_CSR0_BITS_16_BPT << AVR32_SPI_CSR0_BITS_OFFSET);

    AVR32_GPIO.port[SPI0_PORT].ovrs = (SPI0_NPCS_0_PIN);

    unsigned int timeout;
    timeout = SPI_TIMEOUT;
    while (!spi_is_tx_ready(spi)) {
        if (!timeout--) {
            console_print_str("TIMEOUT\r\n");
            AVR32_GPIO.port[SPI0_PORT].ovrc = pin;
            return;
        }
    }

    spi->tdr = ((reg << DRV8711_ADDR_OFFSET) | (data & DRV8711_WRITE_DATA_MASK));

    while (!(spi->sr & AVR32_SPI_SR_TXEMPTY_MASK)) {
        if (!timeout--) {
            console_print_str("TIMEOUT\r\n");
            AVR32_GPIO.port[SPI0_PORT].ovrc = pin;
            return;
        }
    }

    AVR32_GPIO.port[SPI0_PORT].ovrc = pin;

    console_print_str("DATA sent\r\n");
    
}

void drv8711_write_ctrl(uint8_t csid, DRV8711_ENBL enbl, DRV8711_RDIR rdir, DRV8711_RSTEP rstep,
        DRV8711_MODE mode, DRV8711_EXSTALL stall, DRV8711_ISGAIN gain, DRV8711_DTIME dtime){

    uint16_t data;

    data =  MaskOffsetData(enbl, DRV8711_CTRL_ENBL_MASK, DRV8711_CTRL_ENBL_OFFSET);
    data |=  MaskOffsetData(rdir, DRV8711_CTRL_RDIR_MASK, DRV8711_CTRL_RDIR_OFFSET);
    data |=  MaskOffsetData(rstep, DRV8711_CTRL_RSTEP_MASK, DRV8711_CTRL_RSTEP_OFFSET);

    data |=  DRV8711_CTRL_MODE_MASK & (mode << DRV8711_CTRL_MODE_OFFSET);
    data |=  DRV8711_CTRL_EXSTALL_MASK & (stall << DRV8711_CTRL_EXSTALL_OFFSET);
    data |=  DRV8711_CTRL_ISGAIN_MASK & (gain << DRV8711_CTRL_ISGAIN_OFFSET);
    data |=  DRV8711_CTRL_DTIME_MASK & (dtime << DRV8711_CTRL_DTIME_OFFSET);
    drv8711_write_reg(csid, DRV8711_CTRL_ADDRESS, data);
}

void drv8711_write_torque(uint8_t csid, uint8_t torque, DRV8711_SMPLTH smplt){
    uint16_t data;

    data =  DRV8711_TORQUE_TORQUE_MASK & (torque << DRV8711_TORQUE_TORQUE_OFFSET);
    data |=  DRV8711_TORQUE_SMPLTH_MASK & (smplt << DRV8711_TORQUE_SMPLTH_OFFSET);
    drv8711_write_reg(csid,  DRV8711_TORQUE_ADDRESS, data);
}

void drv8711_write_off(uint8_t csid, uint8_t toff, DRV8711_PWMMODE pwm){
    uint16_t data;

    data =  DRV8711_OFF_TOFF_MASK & (toff << DRV8711_OFF_TOFF_OFFSET);
    data |=  DRV8711_OFF_PWMMODE_MASK & (pwm << DRV8711_OFF_PWMMODE_OFFSET);
    drv8711_write_reg(csid,  DRV8711_OFF_ADDRESS, data);
}

void drv8711_write_blank(uint8_t csid, uint8_t tblank, DRV8711_ABT abt){
    uint16_t data;

    data =  DRV8711_BLANK_TBLANK_MASK & (tblank << DRV8711_BLANK_TBLANK_OFFSET);
    data |=  DRV8711_BLANK_ABT_MASK & (abt << DRV8711_BLANK_ABT_OFFSET);
    drv8711_write_reg(csid,  DRV8711_BLANK_ADDRESS, data);
}

void drv8711_write_decay(uint8_t csid, uint8_t tdecay, DRV8711_DECMOD decmod){
    uint16_t data;

    data =  DRV8711_DECAY_TDECAY_MASK & (tdecay << DRV8711_DECAY_TDECAY_OFFSET);
    data |=  DRV8711_DECAY_DECMOD_MASK & (decmod << DRV8711_DECAY_DECMOD_OFFSET);
    drv8711_write_reg(csid,  DRV8711_DECAY_ADDRESS, data);
}

void drv8711_write_stall(uint8_t csid, uint8_t sdthr, DRV8711_SDCNT sdcnt, DRV8711_VDIV vdiv){
    uint16_t data;

    data =  DRV8711_STALL_SDTHR_MASK & (sdthr << DRV8711_STALL_SDTHR_OFFSET);
    data |=  DRV8711_STALL_SDCNT_MASK & (sdcnt << DRV8711_STALL_SDCNT_OFFSET);
    data |=  DRV8711_STALL_VDIV_MASK & (vdiv << DRV8711_STALL_VDIV_OFFSET);
    drv8711_write_reg(csid,  DRV8711_STALL_ADDRESS, data);
}

void drv8711_write_drive(uint8_t csid, DRV8711_OCPTH ocpth, DRV8711_OCPDEG ocpdeg, DRV8711_TDRIVEN tdriven,
        DRV8711_TDRIVEP tdrivep, DRV8711_IDRIVEN idriven, DRV8711_IDRIVEP idrivep){

    uint16_t data;

    data =  DRV8711_DRIVE_OCPTH_MASK & (ocpth << DRV8711_DRIVE_OCPTH_OFFSET);
    data |=  DRV8711_DRIVE_OCPDEG_MASK & (ocpdeg << DRV8711_DRIVE_OCPDEG_OFFSET);
    data |=  DRV8711_DRIVE_TDRIVEN_MASK & (tdriven << DRV8711_DRIVE_TDRIVEN_OFFSET);
    data |=  DRV8711_DRIVE_TDRIVEP_MASK & (tdrivep << DRV8711_DRIVE_TDRIVEP_OFFSET);
    data |=  DRV8711_DRIVE_IDRIVEN_MASK & (idriven << DRV8711_DRIVE_IDRIVEN_OFFSET);
    data |=  DRV8711_DRIVE_IDRIVEP_MASK & (idrivep << DRV8711_DRIVE_IDRIVEP_OFFSET);
    drv8711_write_reg(csid,  DRV8711_DRIVE_ADDRESS, data);
}

void drv8711_write_status(uint8_t csid, uint8_t data){
    drv8711_write_reg(csid, DRV8711_STATUS_ADDRESS, data);
}

uint16_t drv8711_read_status(uint8_t csid){
    return drv8711_read_reg(csid, DRV8711_STATUS_ADDRESS);
}

void drv8711_drive(DRV8711_ID id, uint8_t speed){

}

void drv8711_init(uint8_t csid){
    drv8711_write_ctrl(csid, ENBL_DISABLE, RDIR_DIR_PIN, RSTEP_NO_ACT, STEP_1_16, INTERNAL_DETECT, GAIN_20, DTIME_850_NS);
    drv8711_write_torque(csid, 0xAA, SMPLTH_100_US);
    drv8711_write_off(csid, 0x30, PWMMODE_INT_INDEXER); 
    drv8711_write_blank(csid, 0x80, ABT_DISABLE ); 
    drv8711_write_decay(csid, 0x10, DECMOD_1); 
    drv8711_write_stall(csid, 0x40, SDCNT_FIRST_STEP, VDIV_DIV_32);
    drv8711_write_drive(csid, OCPTH_500_MV, OCPDEG_4_US, TDRIVEN_500_NS, TDRIVEP_500_NS, IDRIVEN_300_MA, IDRIVEP_150_MA);
    drv8711_write_status(csid, 0x00);
}
