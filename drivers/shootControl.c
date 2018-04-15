#include <avr32/io.h>
#include "pwm.h"
#include "shootControl.h"
#include "board.h"

stateResponse_t loadBalls(){
    static int counting = 0;
    static int running = 0;
    static int reverse = 0;

    if(!running){
        if(!counting){
            AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrs = SHOOT_LOADER_PIN_FW;
            AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrc = SHOOT_LOADER_PIN_RV;
            pwmStartCount();
            counting = 1;
        }else{
            if(getCount() >= PWM_CNT_100MS){
                running = 1;
                counting = 0;
            }
        }
    }else{
        if(!reverse){
           if(AVR32_GPIO.port[SHOOT_LOADER_PORT].pvr & SHOOT_LOADER_STALL_PIN){
                /** Reverse Direction **/
                AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrc = SHOOT_LOADER_PIN_FW;
                AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrs = SHOOT_LOADER_PIN_RV;
                reverse = 1;
           }
        }else{
            if(!counting){
                pwmStartCount();
                counting = 1;
            }else{
                if(getCount() >= PWM_CNT_200MS){
                    /** Turn Off Motor **/
                    AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrc = SHOOT_LOADER_PIN_FW | SHOOT_LOADER_PIN_RV;
                    reverse = 0;
                    counting = 0;
                    running = 0;
                    return DONE;
                }
            }
        }
    }

    return NOT_DONE;
}


stateResponse_t shootBalls(){
    static int counting = 0;

    if(!counting){
        // Set solenoid
        AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrs = SHOOT_LOADER_PIN_SOLENOID;
        pwmStartCount();
        counting = 1;
    }else{
        if(getCount() >= PWM_CNT_HALFS){
            // Release solenoid
            AVR32_GPIO.port[SHOOT_LOADER_PORT].ovrc = SHOOT_LOADER_PIN_SOLENOID;
            counting = 0;
            return DONE;
        }
    }

    return NOT_DONE;
}

void setResetFan(uint8_t set){
    if(set){
        /** 60% Duty Cycle **/
        AVR32_PWM.channel[1].cdty = PWM_CDTY_FAN_80PD;
    }else{
        /** 0% Duty Cycle **/
        AVR32_PWM.channel[1].cdty = 0;
    }
}
