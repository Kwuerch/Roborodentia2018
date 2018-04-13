#include <avr32/io.h>
#include "pwm.h"
#include "shootControl.h"
#include "board.h"

#define SHOOT_LOADER_COUNT_MAX       85000
#define SHOOT_LOADER_DELAY_COUNT_MAX 65500
#define SHOOT_SOLENOID_COUNT_MAX     400000 

stateResponse_t loadBalls(){
    static int counting = 0;
    static int count = 0;
    static int delayCount = 0;

    if(!delayCount){
        AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrs = SHOOT_LOADER_PIN_FW;
        AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrc = SHOOT_LOADER_PIN_RV;
        delayCount = 1;
    }

    if(!counting){
        if(delayCount++ >= SHOOT_LOADER_DELAY_COUNT_MAX &&
           AVR32_GPIO.port[SHOOT_LOADER_PORT].pvr & SHOOT_LOADER_STALL_PIN){

            AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrc = SHOOT_LOADER_PIN_FW;
            AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrs = SHOOT_LOADER_PIN_RV;
            counting = 1;
        }
    }else{
        if(count++ == SHOOT_LOADER_COUNT_MAX){
            AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrc = SHOOT_LOADER_PIN_FW | SHOOT_LOADER_PIN_RV;
            count = 0;
            delayCount = 0;
            counting = 0;
            return DONE;
        }
    }

    return NOT_DONE;
}


stateResponse_t shootBalls(){
    static int count = 0;

    if(!count){
        // Set solenoid
        AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrs = SHOOT_LOADER_PIN_SOLENOID;
        count = 1;
    }else{
        if(count++ >= SHOOT_SOLENOID_COUNT_MAX){
            // Release solenoid
            AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrc = SHOOT_LOADER_PIN_SOLENOID;
            count = 0;
            return DONE;
        }
    }

    return NOT_DONE;
}

void setResetFan(uint8_t set){
    if(set){
        /** 60% Duty Cycle **/
        AVR32_PWM.channel[1].cdty = PWM_CDTY_FAN_60PD;
    }else{
        /** 0% Duty Cycle **/
        AVR32_PWM.channel[1].cdty = 0;
    }
}
