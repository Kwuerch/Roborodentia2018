#include <avr32/io.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "flash.h"
#include "pm.h"
#include "scif.h"
#include "twi.h"
#include "usart.h"
#include "spi_master.h"
#include "console.h"
#include "delay.h"
#include "vl53l0x.h"
#include "drv8711.h"


#define STR_BUF_SIZE 256
char strBuf[STR_BUF_SIZE];


int main(void){
    /**
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    VL53L0X_RangingMeasurementData_t meas;
    **/

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
    init_spi();
    console_init();

    /**
    VL53L0X_Dev_t dev;
    dev.I2cDevAddr = 0x29;//VL53L0X_I2C_ADDR1;

    if (status == VL53L0X_ERROR_NONE){
        status = vl53l0x_init(&dev);
    }
    
	if (status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
        while(1){
            AVR32_GPIO.port[1].ovrt = 0x01;
            delay_ms(250);
        }
    }
    **/

    AVR32_GPIO.port[2].gpers = 0x01;
    AVR32_GPIO.port[2].oders = 0x01;
    AVR32_GPIO.port[2].ovrs = 0x01;

    delay_ms(1);

    AVR32_GPIO.port[2].ovrc = 0x01;

    drv8711_init();

    uint16_t data;
    //unsigned char* test = (unsigned char*)"Hello\r\n\0";
    while(1){
        data = drv8711_read_reg((avr32_spi_t*)AVR32_SPI0_ADDRESS, 0, 0x00);

        snprintf(strBuf, STR_BUF_SIZE, "Read 0x00: %X\r\n", data);
        console_print_str(strBuf);
        /**
        status = VL53L0X_PerformSingleRangingMeasurement(&dev, &meas);

        if(meas.RangeStatus){
            console_print_str("RangeStatus is not valid\r\n");
        }else{
            snprintf(strBuf, STR_BUF_SIZE, "Range(mm): %i\r\n", meas.RangeMilliMeter);
            console_print_str(strBuf);
        }
        **/

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
