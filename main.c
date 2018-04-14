#include <avr32/io.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "brushless.h"
#include "flash.h"
#include "pm.h"
#include "pwm.h"
#include "scif.h"
#include "tc.h"
#include "twi.h"
#include "usart.h"
#include "spi_master.h"
#include "gpio.h"
#include "console.h"
#include "delay.h"
#include "vl53l0x.h"
#include "drv8711.h"
#include "motors.h"
#include "led.h"
#include "btn.h"
#include "state.h"
#include "shootControl.h"

typedef enum actionType{
    MOVE, SHOOT, MOVE_SHOOT, DISABLE_F, DISABLE_B, ENABLE_F, ENABLE_B, RAMP_UFW, RAMP_URV
}actionType_t;

typedef struct action{
    actionType_t actionType;
    int arg1;
    int arg2;
}action_t;

/** Actions and Their Arguments
    actionType arg1 arg2
    MOVE       x    y
    SHOOT      spd  n/a 
**/

#define MOVE_TOL 5 

#define WAY_S1_X 305
#define WAY_S1_Y 200 

#define WAY_S2_X 966 
#define WAY_S2_Y 200 

#define WAY_SS12_X 610 
#define WAY_SS12_Y 200 

#define WAY_C_X 610 
#define WAY_C_Y 984 

#define WAY_B_X 610 
#define WAY_B_Y 1455 

#define WAY_SS34_X 610
#define WAY_SS34_Y 2210 

#define WAY_S3_X 254 
#define WAY_S3_Y 2210 

#define WAY_S4_X 966 
#define WAY_S4_Y 2210 

#define SHOOT_SPEED 50

    //{ ENABLE_SHOOTER, 0, 0 },
action_t actions[18] = {
    { MOVE_SHOOT, WAY_S1_X, WAY_S1_Y },
    { MOVE, WAY_S2_X, WAY_S2_Y },

    { MOVE, WAY_SS12_X, WAY_SS12_Y },
    { RAMP_UFW, 0, 0 },
    { ENABLE_F, 0, 0 },

    { MOVE, WAY_B_X, WAY_B_Y },
    { SHOOT, 0, 0 },
    { DISABLE_B, 0, 0 },

    { MOVE, WAY_SS34_X, WAY_SS34_Y },

    { MOVE_SHOOT, WAY_S3_X, WAY_S3_Y },
    { MOVE, WAY_S4_X, WAY_S4_Y },

    { MOVE, WAY_SS34_X, WAY_SS34_Y },
    { RAMP_URV, 0, 0 },
    { ENABLE_B, 0, 0 },

    { MOVE, WAY_C_X, WAY_C_Y },
    { SHOOT, 0, 0 },
    { DISABLE_F, 0, 0 },

    { MOVE, WAY_SS12_X, WAY_SS12_Y },
};

stateResponse_t rampUpFW(position_t *pos){
    static int first = 1;
    int diff;

    if(first){
        drive_motors_ramp(DRV8711_FL, DRV8711_BR, 0, 1, 255);
        first = 0;
    }

    diff = WAY_C_Y - pos->y;
    if(diff > -1 * MOVE_TOL && diff < MOVE_TOL){
        first = 1;
        return DONE;
    }

    return NOT_DONE;
}

stateResponse_t rampUpRV(position_t *pos){
    static int first = 1;
    int diff;

    if(first){
        drive_motors_ramp(DRV8711_FL, DRV8711_BR, 1, 0, 255);
        first = 0;
    }

    diff = pos->y - WAY_B_Y;
    if(diff > -1 * MOVE_TOL && diff < MOVE_TOL){
        first = 1;
        return DONE;
    }

    return NOT_DONE;
}

int main(void){
    int actionCnt = 0;
    int runPullIn = 0;
    position_t pos = { .x = 0, .y = 0 };

    init_board();
    init_drivers();

    console_printf("Hello\r\n");
    led_set(LED_1 | LED_3);
    
    while(!btn_is_pressed(BTN_1));

    /** Start Measurements **/
    vl53l0x_start();

    stateResponse_t response = NOT_DONE;
    stateResponse_t auxResp = NOT_DONE;
    action_t action = actions[actionCnt];

    enableDisableSensor(VL53L0X_F, 0);
    brushless_set_speed(50);
    setResetFan(1);

    while(1){
        updatePosition(&pos);

        switch(action.actionType){
            case MOVE:
                response = drive_to(action.arg1, action.arg2, MOVE_TOL, &pos);
                break;
            case SHOOT:
                response = shootBalls();
                break;
            case MOVE_SHOOT:
                if(auxResp == NOT_DONE){
                    auxResp = shootBalls();
                }
                response = drive_to(action.arg1, action.arg2, MOVE_TOL, &pos);
                break;
            case DISABLE_F:
                enableDisableSensor(VL53L0X_F, 0);
                response = DONE;
                break;
            case DISABLE_B:
                enableDisableSensor(VL53L0X_B, 0);
                response = DONE;
                break;
            case ENABLE_F:
                enableDisableSensor(VL53L0X_F, 1);
                response = DONE;
                break;
            case ENABLE_B:
                enableDisableSensor(VL53L0X_B, 1);
                response = DONE;
                break;
            case RAMP_UFW:
                response = rampUpFW(&pos);
                break;
            case RAMP_URV:
                response = rampUpRV(&pos);
                break;
            default:
                actionCnt = 0;
        }
        
        if(response == DONE){
            if(action.actionType == MOVE || action.actionType == MOVE_SHOOT){
                runPullIn = 1;
            }

            actionCnt = (actionCnt + 1) % sizeof(actions);
            action = actions[actionCnt];

            response = NOT_DONE;
            auxResp = NOT_DONE;
        }

        if(runPullIn && loadBalls() == DONE){
            runPullIn = 0;
        }
    }
}

