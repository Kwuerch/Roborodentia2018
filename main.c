#include <avr32/io.h>
#include <stdlib.h>

#include "peripherals/flash.h"
#include "peripherals/pm.h"
#include "peripherals/scif.h"
#include "peripherals/twi.h"

#define TC0_A_PORT (AVR32_TC0_A0_0_0_PIN / 32)
#define TC0_A_PIN  (1 << (AVR32_TC0_A0_0_0_PIN % 32))

void delay(){
    int i;
    for(i=0; i < 10000; i++){
        
    }
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

    /**
    AVR32_GPIO.port[TC0_A_PORT].gperc = TC0_A_PIN;

    AVR32_GPIO.port[TC0_A_PORT].pmr2c = TC0_A_PIN;
    AVR32_GPIO.port[TC0_A_PORT].pmr1c = TC0_A_PIN;
    AVR32_GPIO.port[TC0_A_PORT].pmr0c = TC0_A_PIN;

    AVR32_TC0.channel[0].ccr = AVR32_TC_CCR0_CLKEN_MASK;
    AVR32_TC0.channel[0].cmr = AVR32_TC_CMR0_WAVE_MASK | AVR32_TC_CMR0_ACPA_TOGGLE | AVR32_TC_CMR0_WAVSEL_UP_AUTO;
    AVR32_TC0.channel[0].ra = 1000;
    **/



    twi_enable((avr32_twim_t*)AVR32_TWIM0_ADDRESS, 400000, 60000000);

    unsigned char* test = (unsigned char*)"abc";
    while(1){
        twi_write(0x29, test, 1);
        delay();
    }
}
