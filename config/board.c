#include <avr32/io.h>
#include "board.h"
#include "spi_master.h"
#include "usart.h"
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
    AVR32_GPIO.port[SPI_CS_PORT].gpers = (SPI_CS0 | SPI_CS1 | SPI_CS2 | SPI_CS3);
    AVR32_GPIO.port[SPI_CS_PORT].oders = (SPI_CS0 | SPI_CS1 | SPI_CS2 | SPI_CS3);
    AVR32_GPIO.port[SPI_CS_PORT].ovrc = (SPI_CS0 | SPI_CS1 | SPI_CS2 | SPI_CS3);

    struct spi_device spi_device_conf = {
        .id = 0
    };

    /** 60 is BAUD_DIV -> 60 MHz Clock -> 1 MHz SPI CLK **/
    spi_master_init(SPI);
    spi_master_setup_device(SPI, &spi_device_conf, SPI_MODE_0, 30, 0);
    AVR32_SPI0.cr  |= AVR32_SPI_CR_SPIEN_MASK;
}

void init_usart(){
    AVR32_GPIO.port[USART1_PORT].pmr2c = (RX_PIN | TX_PIN);
    AVR32_GPIO.port[USART1_PORT].pmr1c = (RX_PIN | TX_PIN);
    AVR32_GPIO.port[USART1_PORT].pmr0c = (RX_PIN | TX_PIN);
    AVR32_GPIO.port[USART1_PORT].gperc = (RX_PIN | TX_PIN);

    usart_options_t opts;
    opts.baudrate = 38400;
    opts.charlength = 8;
    opts.paritytype = USART_NO_PARITY;
    opts.stopbits = USART_1_STOPBIT;
    opts.channelmode = USART_NORMAL_CHMODE;

    usart_init_rs232(USART, &opts, PBA_HZ);
}

/** Initialize Shutdown Pins for VL53L0X **/
void init_vl53l0x_sd(){
    AVR32_GPIO.port[VL53L0X_PORT].gpers = (VL53L0X_SD_R | VL53L0X_SD_L | VL53L0X_SD_F | VL53L0X_SD_B);
    AVR32_GPIO.port[VL53L0X_PORT].oders = (VL53L0X_SD_R | VL53L0X_SD_L | VL53L0X_SD_F | VL53L0X_SD_B);
    AVR32_GPIO.port[VL53L0X_PORT].ovrs = (VL53L0X_SD_R | VL53L0X_SD_L | VL53L0X_SD_F | VL53L0X_SD_B);
}
