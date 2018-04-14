#include <avr32/io.h>
#include "gpio.h"
#include "board.h"
#include "pwm.h"

void PWM_INIT(){
    /** Set Enable PWM Channel 0, 1, 2, and 3 to divide clock by 4 and provide
     * a 1.1ms on time at 50 Hz **/

    AVR32_PWM.channel[0].cprd = PWM_CPRD_50HZ;
    AVR32_PWM.channel[0].cdty = PWM_CDTY_1_1MS;
    AVR32_PWM.channel[0].cmr |= AVR32_PWM_CMR0_CPRE_CCK_DIV_4 << AVR32_PWM_CMR0_CPRE_OFFSET |
                                AVR32_PWM_CMR0_CPOL_MASK;

    AVR32_PWM.channel[1].cprd = PWM_CPRD_FAN_25KHZ;
    AVR32_PWM.channel[1].cdty = 0;
    AVR32_PWM.channel[1].cmr |= AVR32_PWM_CMR0_CPOL_MASK;

    AVR32_PWM.channel[2].cprd = PWM_CPRD_QTRHZ;
    AVR32_PWM.channel[2].cdty = 3000;
    AVR32_PWM.channel[2].cmr |= AVR32_PWM_CMR0_CPRE_CCK_DIV_256 << AVR32_PWM_CMR0_CPRE_OFFSET |
                                AVR32_PWM_CMR0_CPOL_MASK;

    AVR32_PWM.channel[3].cprd = PWM_CPRD_50HZ;
    AVR32_PWM.channel[3].cdty = 0;
    AVR32_PWM.channel[3].cmr |= AVR32_PWM_CMR0_CPRE_CCK_DIV_4 << AVR32_PWM_CMR0_CPRE_OFFSET |
                                AVR32_PWM_CMR0_CPOL_MASK;

    AVR32_PWM.ena = AVR32_PWM_ENA_CHID0_MASK |
                    AVR32_PWM_ENA_CHID1_MASK |
                    AVR32_PWM_ENA_CHID2_MASK |
                    AVR32_PWM_ENA_CHID3_MASK;
}

volatile uint32_t startVal = 0;

void pwmStartCount(){
    startVal = AVR32_PWM.channel[2].ccnt;
}

uint32_t getCount(){
    uint32_t cnt = AVR32_PWM.channel[2].ccnt;
    if(cnt >= startVal){
        return cnt - startVal;
    }else{
        return ((uint32_t)PWM_CPRD_QTRHZ - startVal) + cnt;
    }
}
