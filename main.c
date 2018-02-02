#include <avr32/io.h>
#include <stdlib.h>

#include "flash.h"
#include "pm.h"
#include "scif.h"
#include "twi.h"
#include "usart.h"
#include "spi_master.h"
#include "console.h"
#include "config.h"
#include "vl53l0x.h"

#define TC0_A_PORT (AVR32_TC0_A0_0_0_PIN / 32)
#define TC0_A_PIN  (1 << (AVR32_TC0_A0_0_0_PIN % 32))

#define SPI0_PORT (AVR32_SPI0_MISO_PIN / 32)

#define SPI0_MISO_PIN (1 << (AVR32_SPI0_MISO_PIN % 32))
#define SPI0_MOSI_PIN (1 << (AVR32_SPI0_MOSI_PIN % 32))
#define SPI0_SCK_PIN (1 << (AVR32_SPI0_SCK_PIN % 32))
#define SPI0_NPCS_0_PIN (1 << (AVR32_SPI0_NPCS_0_PIN % 32))
#define SPI0_NPCS_1_PIN (1 << (AVR32_SPI0_NPCS_1_PIN % 32))
#define SPI0_NPCS_2_PIN (1 << (AVR32_SPI0_NPCS_2_PIN % 32))
#define SPI0_NPCS_3_PIN (1 << (AVR32_SPI0_NPCS_3_PIN % 32))

#define TWI0_PORT (AVR32_TWIMS0_TWD_0_0_PIN / 32)

#define TWID_PIN  (1 << (AVR32_TWIMS0_TWD_0_0_PIN % 32))
#define TWICK_PIN (1 << (AVR32_TWIMS0_TWCK_0_0_PIN % 32))

void delay(){
    int i;
    for(i=0; i < 100000; i++){
        
    }
}

void init_twi(){
    //Function A
    AVR32_GPIO.port[TWI0_PORT].pmr2c = (TWID_PIN | TWICK_PIN);
    AVR32_GPIO.port[TWI0_PORT].pmr1c = (TWID_PIN | TWICK_PIN);
    AVR32_GPIO.port[TWI0_PORT].pmr0c = (TWID_PIN | TWICK_PIN);
    AVR32_GPIO.port[TWI0_PORT].gperc = (TWID_PIN | TWICK_PIN);

    twi_enable((avr32_twim_t*)AVR32_TWIM0_ADDRESS, 100000, PBA_HZ);
}
void init_spi(){
    //Function B
    AVR32_GPIO.port[SPI0_PORT].pmr2c = (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN | SPI0_NPCS_0_PIN | SPI0_NPCS_1_PIN | SPI0_NPCS_2_PIN | SPI0_NPCS_3_PIN);
    AVR32_GPIO.port[SPI0_PORT].pmr1c = (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN | SPI0_NPCS_0_PIN | SPI0_NPCS_1_PIN | SPI0_NPCS_2_PIN | SPI0_NPCS_3_PIN);
    AVR32_GPIO.port[SPI0_PORT].pmr0s = (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN | SPI0_NPCS_0_PIN | SPI0_NPCS_1_PIN | SPI0_NPCS_2_PIN | SPI0_NPCS_3_PIN);
    AVR32_GPIO.port[SPI0_PORT].gperc = (SPI0_MISO_PIN | SPI0_MOSI_PIN | SPI0_SCK_PIN | SPI0_NPCS_0_PIN | SPI0_NPCS_1_PIN | SPI0_NPCS_2_PIN | SPI0_NPCS_3_PIN);

    spi_master_init((avr32_spi_t*)AVR32_SPI0_ADDRESS);
}

int main(void){
    AVR32_GPIO.port[1].gper = 0x0F;
    AVR32_GPIO.port[1].oder = 0x0F;
    AVR32_GPIO.port[1].ovr = 0x00;

    scif_enable_RC120MCR();
    flash_set_wait_state(1);
    pm_divide_clk(CPU_CLK, CLK_DIV_2);
    pm_master_clk_sel(RC120M);

    pm_divide_clk(PBA_CLK, CLK_DIV_2);
    pm_divide_clk(PBC_CLK, CLK_DIV_2);

    init_twi();
    //init_spi();
    console_init();

    VL53L0X_Dev_t dev;
    dev.I2cDevAddr = 0x29;

    vl53l0x_init(&dev);
    vl53l0x_init_longrange(&dev);

    unsigned char* test = (unsigned char*)"Hello\r\n\0";
    while(1){
        twi_write(0x29, test, 2);
        //console_print_str(test);
        //AVR32_GPIO.port[1].ovrt = 0x01;
        //spi_write_packet((avr32_spi_t*)AVR32_SPI0_ADDRESS, test, 5);
        delay();

    }
}
