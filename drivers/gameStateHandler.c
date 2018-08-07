#include <avr32/io.h>

#include "btn.h"
#include "drv8711.h"
#include "gameStateHandler.h"
#include "pwm.h"
#include "waypoints.h"

stateResponse_t driveToWallFW(){
    static int first = 1;

    if(first){
        drive_motor(DRV8711_FL, 0, 50);
        drive_motor(DRV8711_BR, 1, 50);
        first = 0;
    }else{
        if(!(AVR32_GPIO.port[BTN_PORT].pvr & LIMIT_FRONT_PIN)){
            drive_motor(DRV8711_FL, 0, 0);
            drive_motor(DRV8711_BR, 1, 0);
            first = 1;
            return DONE;
        }
    }

    return NOT_DONE;
}

stateResponse_t driveToWallRV(){
    static int first = 1;

    if(first){
        drive_motor(DRV8711_FL, 1, 50);
        drive_motor(DRV8711_BR, 0, 50);
        first = 0;
    }else{
        if(!(AVR32_GPIO.port[BTN_PORT].pvr & LIMIT_BACK_PIN)){
            drive_motor(DRV8711_FL, 1, 0);
            drive_motor(DRV8711_BR, 0, 0);
            first = 1;
            return DONE;
        }
    }

    return NOT_DONE;
}

stateResponse_t driveAlongWallFW(){
    if(AVR32_GPIO.port[BTN_PORT].pvr & LIMIT_FRONT_PIN){
        drive_motor(DRV8711_FL, 0, 50);
        drive_motor(DRV8711_BR, 1, 50);
        return DONE;
    }else{
        drive_motor(DRV8711_FL, 0, 0);
        drive_motor(DRV8711_BR, 1, 0);
        return NOT_DONE;
    }
}

stateResponse_t driveAlongWallRV(){
    if(AVR32_GPIO.port[BTN_PORT].pvr & LIMIT_BACK_PIN){
        drive_motor(DRV8711_FL, 1, 50);
        drive_motor(DRV8711_BR, 0, 50);
        return DONE;
    }else{
        drive_motor(DRV8711_FL, 1, 0);
        drive_motor(DRV8711_BR, 0, 0);
        return NOT_DONE;
    }
}

stateResponse_t rampUpFW(position_t *pos){
    static int first = 1;

    if(first){
        drive_motors_ramp(DRV8711_FL, DRV8711_BR, 0, 1, 255);
        first = 0;
    }

    if(pos->y >= WAY_C_Y){
        first = 1;
        return DONE;
    }

    return NOT_DONE;
}

stateResponse_t rampUpRV(position_t *pos){
    static int first = 1;

    if(first){
        drive_motors_ramp(DRV8711_FL, DRV8711_BR, 1, 0, 255);
        first = 0;
    }

    if(pos->y < WAY_B_Y){
        first = 1;
        return DONE;
    }

    return NOT_DONE;
}

stateResponse_t rampDownFW(){
    static int counting = 0;

    if(!counting){
        drive_motors_ramp(DRV8711_FL, DRV8711_BR, 0, 1, 255);
        pwmStartCount();
        counting = 1;
    }else{
        if(getCount() >= PWM_CNT_HALFS){
            counting = 0;
            return DONE;
        }
    }

    return NOT_DONE;
}

stateResponse_t rampDownRV(){
    static int counting = 0;

    if(!counting){
        drive_motors_ramp(DRV8711_FL, DRV8711_BR, 1, 0, 255);
        pwmStartCount();
        counting = 1;
    }else{
        if(getCount() >= PWM_CNT_HALFS){
            counting = 0;
            return DONE;
        }
    }

    return NOT_DONE;
}
