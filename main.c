#include <avr32/io.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "flash.h"
#include "pm.h"
#include "pwm.h"
#include "scif.h"
#include "tc.h"
#include "twi.h"
#include "usart.h"
#include "spi_master.h"
#include "console.h"
#include "delay.h"
#include "vl53l0x.h"
#include "drv8711.h"
#include "motors.h"


int main(void){
    AVR32_GPIO.port[1].gper = 0x0F;
    AVR32_GPIO.port[1].oder = 0x0F;
    AVR32_GPIO.port[1].ovr = 0x00;

    scif_enable_RC120MCR();

    /** This clock is not used **/
    /** This function does not even work :( **/
    scif_enable_OSC32K();

    flash_set_wait_state(1);

    pm_divide_clk(CPU_CLK, CLK_DIV_2);
    pm_master_clk_sel(RC120M);

    pm_divide_clk(PBA_CLK, CLK_DIV_2);
    pm_divide_clk(PBB_CLK, CLK_DIV_2);
    pm_divide_clk(PBC_CLK, CLK_DIV_2);

    init_twi();
    init_spi();

    console_init();
    vl53l0x_init_all();

    console_printf("Hello Again\r\n");

    init_drv8711_step();
    init_drv8711_ctrl();
    init_drv8711_dir();

    drv8711_init(DRV8711_FL);
    drv8711_init(DRV8711_BR);
    drv8711_init(DRV8711_FR);
    drv8711_init(DRV8711_BL);

    init_pwm_step();
    tc_init();
    pwm_init();
    
    //180 corresponds to 2.1kHz
    //1 corresponds to 690 Hz
    drive_motor(DRV8711_FL, 0, 1);
    drive_motor(DRV8711_FR, 0, 180);
    drive_motor(DRV8711_BR, 0, 60);
    drive_motor(DRV8711_BL, 0, 70);

    while(1){
        /**
        while(count++ < 2000){
            AVR32_TC0.channel[0].ra -= 1;
            AVR32_TC0.channel[0].rc -= 2;
            delay_ms(1);
        }

        while(count-- > 0){
            AVR32_TC0.channel[0].ra += 1;
            AVR32_TC0.channel[0].rc += 2;
            delay_ms(1);
        }
        **/


        delay_ms(100);
    }
}
