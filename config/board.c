#include <avr32/io.h>
#include "board.h"
#include "spi_master.h"
#include "usart.h"
#include "twi.h"

void init_gpio_af(uint8_t port, uint32_t pin_mask, uint8_t func){
    switch(func){
        case 0x00: /** Function A **/
            AVR32_GPIO.port[port].pmr2c = pin_mask;
            AVR32_GPIO.port[port].pmr1c = pin_mask;
            AVR32_GPIO.port[port].pmr0c = pin_mask;
            break;
        case 0x01: /** Function B **/
            AVR32_GPIO.port[port].pmr2c = pin_mask;
            AVR32_GPIO.port[port].pmr1c = pin_mask;
            AVR32_GPIO.port[port].pmr0s = pin_mask;
            break;
        case 0x02: /** Function C **/
            AVR32_GPIO.port[port].pmr2c = pin_mask;
            AVR32_GPIO.port[port].pmr1s = pin_mask;
            AVR32_GPIO.port[port].pmr0c = pin_mask;
            break;
        case 0x03: /** Function D **/
            AVR32_GPIO.port[port].pmr2c = pin_mask;
            AVR32_GPIO.port[port].pmr1s = pin_mask;
            AVR32_GPIO.port[port].pmr0s = pin_mask;
            break;
        case 0x04: /** Function E **/
            AVR32_GPIO.port[port].pmr2s = pin_mask;
            AVR32_GPIO.port[port].pmr1c = pin_mask;
            AVR32_GPIO.port[port].pmr0c = pin_mask;
            break;
        case 0x05: /** Function F **/
            AVR32_GPIO.port[port].pmr2s = pin_mask;
            AVR32_GPIO.port[port].pmr1c = pin_mask;
            AVR32_GPIO.port[port].pmr0s = pin_mask;
            break;
    }
    
    AVR32_GPIO.port[port].gperc = pin_mask;
}

void init_twi(){
    init_gpio_af(TWI0_PORT, (TWID_PIN | TWICK_PIN), TWI_FUNC);

    twi_enable((avr32_twim_t*)AVR32_TWIM0_ADDRESS, TWI_SPEED, PBA_HZ);
}

void init_spi(){
    /** Initialize SPI pins - Function A**/
    init_gpio_af(SPI0_PORT, (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN ), SPI_FUNC);

    /** Initialize CS Pins **/
    AVR32_GPIO.port[SPI_CS_PORT].gpers = (SPI_CS0_PIN | SPI_CS1_PIN | SPI_CS2_PIN | SPI_CS3_PIN);
    AVR32_GPIO.port[SPI_CS_PORT].oders = (SPI_CS0_PIN | SPI_CS1_PIN | SPI_CS2_PIN | SPI_CS3_PIN);
    AVR32_GPIO.port[SPI_CS_PORT].ovrc = (SPI_CS0_PIN | SPI_CS1_PIN | SPI_CS2_PIN | SPI_CS3_PIN);

    struct spi_device spi_device_conf = {
        .id = 0
    };

    /** 60 is BAUD_DIV -> 60 MHz Clock -> 1 MHz SPI CLK **/
    spi_master_init(SPI);
    spi_master_setup_device(SPI, &spi_device_conf, SPI_MODE_0, 30, 0);
    AVR32_SPI0.cr  |= AVR32_SPI_CR_SPIEN_MASK;
}

void init_usart(){
    init_gpio_af(USART1_PORT, (RX_PIN | TX_PIN), USART1_FUNC); 

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
    AVR32_GPIO.port[VL53L0X_PORT].gpers = (VL53L0X_SD_PIN_R | VL53L0X_SD_PIN_L | VL53L0X_SD_PIN_F | VL53L0X_SD_PIN_B);
    AVR32_GPIO.port[VL53L0X_PORT].oders = (VL53L0X_SD_PIN_R | VL53L0X_SD_PIN_L | VL53L0X_SD_PIN_F | VL53L0X_SD_PIN_B);
    AVR32_GPIO.port[VL53L0X_PORT].ovrc = (VL53L0X_SD_PIN_R | VL53L0X_SD_PIN_L | VL53L0X_SD_PIN_F | VL53L0X_SD_PIN_B);
}

void init_drv8711_step(){
    init_gpio_af(DRV8711_STEPAB_PORT, (DRV8711_STEP_PIN_A | DRV8711_STEP_PIN_B), DRV8711_STEPAB_FUNC);
    init_gpio_af(DRV8711_STEPC_PORT, DRV8711_STEP_PIN_C, DRV8711_STEPC_FUNC);
    init_gpio_af(DRV8711_STEPD_PORT, DRV8711_STEP_PIN_D, DRV8711_STEPD_FUNC);
}

void init_drv8711_ctrl(){
    AVR32_GPIO.port[DRV8711_CTRL_PORT].gpers = (DRV8711_NSLP_PIN | DRV8711_RST_PIN);
    AVR32_GPIO.port[DRV8711_CTRL_PORT].oders = (DRV8711_NSLP_PIN | DRV8711_RST_PIN);

    AVR32_GPIO.port[DRV8711_CTRL_PORT].ovrc =  DRV8711_RST_PIN;
    AVR32_GPIO.port[DRV8711_CTRL_PORT].ovrs =  DRV8711_NSLP_PIN;
}

void init_drv8711_dir(){
    AVR32_GPIO.port[DRV8711_DIR_PORT].gpers = (DRV8711_DIR_PIN_A | DRV8711_DIR_PIN_B | DRV8711_DIR_PIN_C | DRV8711_DIR_PIN_D);
    AVR32_GPIO.port[DRV8711_DIR_PORT].oders = (DRV8711_DIR_PIN_A | DRV8711_DIR_PIN_B | DRV8711_DIR_PIN_C | DRV8711_DIR_PIN_D);
    AVR32_GPIO.port[DRV8711_DIR_PORT].ovrc =  (DRV8711_DIR_PIN_A | DRV8711_DIR_PIN_B | DRV8711_DIR_PIN_C | DRV8711_DIR_PIN_D);
}

void init_pwm_step(){
    init_gpio_af(PWM_PORT, (PWM_SHOOT_PIN | PWM_G1_PIN | PWM_G2_PIN | PWM_G3_PIN), PWM_FUNC);
}
