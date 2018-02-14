#include <avr32/io.h>
#include "board.h"
#include "spi_master.h"
#include "twi.h"

void init_twi(){
    //Function A
    AVR32_GPIO.port[TWI0_PORT].pmr2c = (TWID_PIN | TWICK_PIN);
    AVR32_GPIO.port[TWI0_PORT].pmr1c = (TWID_PIN | TWICK_PIN);
    AVR32_GPIO.port[TWI0_PORT].pmr0c = (TWID_PIN | TWICK_PIN);
    AVR32_GPIO.port[TWI0_PORT].gperc = (TWID_PIN | TWICK_PIN);

    twi_enable((avr32_twim_t*)AVR32_TWIM0_ADDRESS, TWI_SPEED, PBA_HZ);
}
void init_spi(){
    /** Initialize SPI pins - Function A**/
    AVR32_GPIO.port[SPI0_PORT].pmr2c = (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN );
    AVR32_GPIO.port[SPI0_PORT].pmr1c = (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN );
    AVR32_GPIO.port[SPI0_PORT].pmr0c = (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN );
    AVR32_GPIO.port[SPI0_PORT].gperc = (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN );

    /** Initialize CS Pins **/
    AVR32_GPIO.port[SPI0_PORT].gpers = (SPI0_NPCS_0_PIN | SPI0_NPCS_1_PIN | SPI0_NPCS_2_PIN | SPI0_NPCS_3_PIN);
    AVR32_GPIO.port[SPI0_PORT].oders = (SPI0_NPCS_0_PIN | SPI0_NPCS_1_PIN | SPI0_NPCS_2_PIN | SPI0_NPCS_3_PIN);
    AVR32_GPIO.port[SPI0_PORT].ovrc = (SPI0_NPCS_0_PIN | SPI0_NPCS_1_PIN | SPI0_NPCS_2_PIN | SPI0_NPCS_3_PIN);

    struct spi_device spi_device_conf = {
        .id = 0
    };

    /** 60 is BAUD_DIV -> 60 MHz Clock -> 1 MHz SPI CLK **/
    spi_master_init((avr32_spi_t*)AVR32_SPI0_ADDRESS);
    spi_master_setup_device((avr32_spi_t*)AVR32_SPI0_ADDRESS, &spi_device_conf, SPI_MODE_0, 30, 0);
    AVR32_SPI0.cr  |= AVR32_SPI_CR_SPIEN_MASK;
}
