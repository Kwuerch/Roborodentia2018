#include "drv8711.h"
#include "spi_master.h"
#include <avr32/io.h>

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


uint16_t drv8711_read_reg(volatile avr32_spi_t* spi, struct spi_device* dev, uint8_t reg){
    spi->mr &= ~AVR32_SPI_MR_PCS_MASK;
    spi->mr |= AVR32_SPI_MR_PCS_MASK & ((dev -> id) << AVR32_SPI_MR_PCS_OFFSET);

    volatile unsigned long* csrReg;
    switch(dev -> id){
        case 0:
            csrReg = &(spi->csr0);
            break;
        case 1:
            csrReg = &(spi->csr1);
            break;
        case 2:
            csrReg = &(spi->csr2);
            break;
        case 3:
            csrReg = &(spi->csr3);
            break;
        default:
            return 0;
    }

    /** USE independent GPIOs instead to match active HIGH of DRV7811 -- IS that even possible if want to write 4 bit words?? **/

    #define AVR32_SPI_CSR0_BITS_4_BPT 0x09

    /** Set to 4 bit mode **/
    *csrReg &= ~AVR32_SPI_CSR0_BITS_MASK; 
    *csrReg |= AVR32_SPI_CSR0_BITS_MASK & (AVR32_SPI_CSR0_BITS_4_BPT << AVR32_SPI_CSR1_BITS_OFFSET);

    /** Keep CS low after transfer **/
    *csrReg |= AVR32_SPI_CSR0_CSAAT_MASK;

    spi->tdr = ((DRV8711_READ_MASK >> DRV8711_ADDR_OFFSET) | reg) << AVR32_SPI_TDR_TD_OFFSET;

    /** Set to 12 bit mode **/
    *csrReg &= ~AVR32_SPI_CSR0_BITS_MASK; 
    *csrReg |= AVR32_SPI_CSR0_BITS_MASK & (AVR32_SPI_CSR0_BITS_12_BPT << AVR32_SPI_CSR1_BITS_OFFSET);

    uint16_t data;
    data = spi->rdr & AVR32_SPI_RDR_RD_MASK;

    /** Raise CS **/
    *csrReg &= ~AVR32_SPI_CSR0_CSAAT_MASK;
}

void drv8711_init_ctrl(DRV8711_MODE mode, DRV8711_EXSTALL stall, DRV8711_ISGAIN gain, DRV8711_DTIME dtime){
    uint16_t data;

    data =  DRV8711_CTRL_MODE_MASK & (mode << DRV8711_CTRL_MODE_OFFSET);
    data |=  DRV8711_CTRL_EXSTALL_MASK & (stall << DRV8711_CTRL_EXSTALL_OFFSET);
    data |=  DRV8711_CTRL_ISGAIN_MASK & (gain << DRV8711_CTRL_ISGAIN_OFFSET);
    data |=  DRV8711_CTRL_DTIME_MASK & (dtime << DRV8711_CTRL_DTIME_OFFSET);
    drv8711_write_reg(DRV8711_CTRL_ADDRESS, data);
}

void drv8711_init_torque(uint8_t torque, DRV8711_SMPLTH smplt){
    uint16_t data;

    data =  DRV8711_TORQUE_TORQUE_MASK & (torque << DRV8711_TORQUE_TORQUE_OFFSET);
    data |=  DRV8711_TORQUE_SMPLTH_MASK & (smplt << DRV8711_TORQUE_SMPLTH_OFFSET);
    drv8711_write_reg(DRV8711_TORQUE_ADDRESS, data);
}

void drv8711_init(){
    drv8711_init_ctrl(STEP_1_16, INTERNAL_DETECT, GAIN_20, DTIME_850_NS);
    drv8711_init_torque(0xAA, SMPLTH_100_US);
}
