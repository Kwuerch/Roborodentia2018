#include <avr32/io.h>
#include <stdlib.h>
#include <stdio.h>

#include "flash.h"
#include "pm.h"
#include "scif.h"
#include "twi.h"
#include "usart.h"
#include "spi_master.h"
#include "console.h"
#include "config.h"
#include "delay.h"
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

#define STR_BUF_SIZE 256
char strBuf[STR_BUF_SIZE];

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
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    VL53L0X_RangingMeasurementData_t meas;

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

    delay_ms(1000);

    if (status == VL53L0X_ERROR_NONE){
        status = vl53l0x_init(&dev);
    }

	if (status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
        while(1){
            AVR32_GPIO.port[1].ovrt = 0x01;
            //console_print_str("Could Not Initialize VL53L0X Sensor\r\n");
            delay_ms(250);
        }
    }
        
        
    vl53l0x_init_longrange(&dev);

    //unsigned char* test = (unsigned char*)"Hello\r\n\0";
    while(1){
        status = VL53L0X_PerformSingleRangingMeasurement(&dev, &meas);

        if(meas.RangeStatus){
            console_print_str("RangeStatus is not valid\r\n");
        }else{
            snprintf(strBuf, STR_BUF_SIZE, "Range(mm): %i\r\n", meas.RangeMilliMeter);
            console_print_str(strBuf);
        }

        /**
        snprintf(strBuf, STR_BUF_SIZE, "Range(mm): %i\r\n", meas.RangeMilliMeter);
        console_print_str(strBuf);
        **/

        /**
        if (status != VL53L0X_ERROR_NONE)
            vl53l0x_print_error(status);
        **/

        //twi_read_reg(dev.I2cDevAddr, 0xCB, buf, 1);


        //console_print_str(test);
        //AVR32_GPIO.port[1].ovrt = 0x01;
        //spi_write_packet((avr32_spi_t*)AVR32_SPI0_ADDRESS, test, 5);
        delay_ms(10);
    }
}
