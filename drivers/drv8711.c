#include "drv8711.h"
#include "spi_master.h"
#include "board.h"
#include "console.h"
#include "delay.h"
#include <avr32/io.h>

#define AVR32_SPI_CSR0_BITS_4_BPT 0x09

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


uint16_t drv8711_read_reg(volatile avr32_spi_t* spi, uint8_t csid, uint8_t reg){
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


void drv8711_write_reg(volatile avr32_spi_t* spi, uint8_t csid, uint8_t reg, uint16_t data){
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
void drv8711_init_ctrl(DRV8711_MODE mode, DRV8711_EXSTALL stall, DRV8711_ISGAIN gain, DRV8711_DTIME dtime){
    uint16_t data;

    data =  DRV8711_CTRL_ENBL_MASK & (mode << DRV8711_CTRL_ENBL_OFFSET);
    data |=  DRV8711_CTRL_MODE_MASK & (mode << DRV8711_CTRL_MODE_OFFSET);
    data |=  DRV8711_CTRL_EXSTALL_MASK & (stall << DRV8711_CTRL_EXSTALL_OFFSET);
    data |=  DRV8711_CTRL_ISGAIN_MASK & (gain << DRV8711_CTRL_ISGAIN_OFFSET);
    data |=  DRV8711_CTRL_DTIME_MASK & (dtime << DRV8711_CTRL_DTIME_OFFSET);
    drv8711_write_reg((avr32_spi_t*)AVR32_SPI0_ADDRESS, 0, DRV8711_CTRL_ADDRESS, data);
}

void drv8711_init_torque(uint8_t torque, DRV8711_SMPLTH smplt){
    uint16_t data;

    data =  DRV8711_TORQUE_TORQUE_MASK & (torque << DRV8711_TORQUE_TORQUE_OFFSET);
    data |=  DRV8711_TORQUE_SMPLTH_MASK & (smplt << DRV8711_TORQUE_SMPLTH_OFFSET);
    drv8711_write_reg((avr32_spi_t*)AVR32_SPI0_ADDRESS, 0,  DRV8711_TORQUE_ADDRESS, data);
}

void drv8711_init(){
    drv8711_init_ctrl(STEP_1_16, INTERNAL_DETECT, GAIN_20, DTIME_850_NS);
    drv8711_init_torque(0xAA, SMPLTH_100_US);
    drv8711_write_reg((avr32_spi_t*)AVR32_SPI0_ADDRESS, 0, DRV8711_STATUS_ADDRESS, 0);
}
