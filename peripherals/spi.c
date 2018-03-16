#include <avr32/io.h>
#include "spi_master.h"
#include "board.h"
#include "gpio.h"

void SPI0_INIT(){
    gpioInit gi;
    gi.pins = SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN ;
    gi.type = GPIO_AF;
    gi.port = SPI0_PORT;
    gi.af = FUNC_A;
    gpio_init(&gi);

    gi.pins = SPI_CS0_PIN | SPI_CS1_PIN | SPI_CS2_PIN | SPI_CS3_PIN;
    gi.type = GPIO_OUTPUT;
    gi.port = SPI_CS_PORT;
    gpio_init(&gi);

    struct spi_device spi_device_conf = {
        .id = 0
    };

    /** 60 is BAUD_DIV -> 60 MHz Clock -> 1 MHz SPI CLK **/
    spi_master_init(SPI0);
    spi_master_setup_device(SPI0, &spi_device_conf, SPI_MODE_0, 30, 0);
    AVR32_SPI0.cr  |= AVR32_SPI_CR_SPIEN_MASK;

    SPI0->mr &= ~AVR32_SPI_MR_PCS_MASK;
    SPI0->mr |= AVR32_SPI_MR_PCS_MASK & (1 << AVR32_SPI_MR_PCS_OFFSET);
}
