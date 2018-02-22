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
    init_spi();

    console_init();
    vl53l0x_init_all();

    drv8711_init(DRV8711_FL);
    drv8711_init(DRV8711_BR);
    drv8711_init(DRV8711_FR);
    drv8711_init(DRV8711_BL);

    while(1){
        console_printf("Range (mm): %i\r\n", vl53l0x_measure(VL53L0X_R));

        delay_ms(100);
    }
}
