#include "twi.h"
#include "spi.h"
#include "usart.h"
#include "tc.h"
#include "gpio.h"
#include "pwm.h"
#include "vl53l0x.h"
#include "drv8711.h"
#include "scif.h"
#include "pm.h"
#include "flash.h"

void init_board(){
    scif_enable_RC120MCR();
    flash_set_wait_state(1);

    pm_divide_clk(CPU_CLK, CLK_DIV_2);
    pm_master_clk_sel(RC120M);

    pm_divide_clk(PBA_CLK, CLK_DIV_2);
    pm_divide_clk(PBB_CLK, CLK_DIV_2);
    pm_divide_clk(PBC_CLK, CLK_DIV_2);

    TWIM0_INIT();
    SPI0_INIT();
    USART1_INIT();
    TC0_INIT();
    TC1_INIT();
    PWM_INIT();
    GPIO_INIT();
}

void init_drivers(){
    vl53l0x_init_all();

    drv8711_init(DRV8711_FL);
    drv8711_init(DRV8711_BR);
    drv8711_init(DRV8711_FR);
    drv8711_init(DRV8711_BL);
}
