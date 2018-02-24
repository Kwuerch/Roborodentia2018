#include <avr32/io.h>

void pwm_init(){
    AVR32_PWM.channel[0].cprd = 320664;
    AVR32_PWM.channel[0].cdty = 16033;
    /** Divide PWM CLK by 4 and start output high **/
    AVR32_PWM.channel[0].cmr |= AVR32_PWM_CMR0_CPRE_CCK_DIV_4 << AVR32_PWM_CMR0_CPRE_OFFSET | AVR32_PWM_CMR0_CPOL_MASK;

    AVR32_PWM.channel[1].cprd = 320664;
    AVR32_PWM.channel[1].cdty = 16033;
    /** Divide PWM CLK by 4 and start output high **/
    AVR32_PWM.channel[1].cmr |= AVR32_PWM_CMR0_CPRE_CCK_DIV_4 << AVR32_PWM_CMR0_CPRE_OFFSET | AVR32_PWM_CMR0_CPOL_MASK;

    AVR32_PWM.channel[2].cprd = 320664;
    AVR32_PWM.channel[2].cdty = 16033;
    /** Divide PWM CLK by 4 and start output high **/
    AVR32_PWM.channel[2].cmr |= AVR32_PWM_CMR0_CPRE_CCK_DIV_4 << AVR32_PWM_CMR0_CPRE_OFFSET | AVR32_PWM_CMR0_CPOL_MASK;

    AVR32_PWM.channel[3].cprd = 320664;
    AVR32_PWM.channel[3].cdty = 16033;
    /** Divide PWM CLK by 4 and start output high **/
    AVR32_PWM.channel[3].cmr |= AVR32_PWM_CMR0_CPRE_CCK_DIV_4 << AVR32_PWM_CMR0_CPRE_OFFSET | AVR32_PWM_CMR0_CPOL_MASK;


    AVR32_PWM.ena = AVR32_PWM_ENA_CHID0_MASK | AVR32_PWM_ENA_CHID1_MASK | AVR32_PWM_ENA_CHID2_MASK | AVR32_PWM_ENA_CHID3_MASK;
}
