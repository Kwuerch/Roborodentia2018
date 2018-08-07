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
#include "waypoints.h"
#include "gameStateHandler.h"

typedef enum actionType{
    MOVE, SHOOT, MOVE_SHOOT, DISABLE_F, DISABLE_B, ENABLE_F, ENABLE_B, RAMP_UFW, RAMP_URV, RAMP_DFW, RAMP_DRV, MOVE_ALONG_WALL_FW, MOVE_ALONG_WALL_RV, MOVE_ALONG_WALL_SHOOT_FW, MOVE_ALONG_WALL_SHOOT_RV, MOVE_TO_WALL_RV, MOVE_TO_WALL_FW
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

#define MOVE_TOL 7 
#define NUM_ACTIONS 23

action_t actions[NUM_ACTIONS] = {
    { MOVE_TO_WALL_RV, 0, 0 },
    { MOVE_ALONG_WALL_SHOOT_RV, WAY_S1_X, 0 },
    { MOVE_ALONG_WALL_RV, WAY_S2_X, 0 },
    { MOVE_ALONG_WALL_RV, WAY_SS12_X, 0 },
    { MOVE_TO_WALL_RV, 0, 0 },

    { RAMP_UFW, 0, 0 },
    { ENABLE_F, 0, 0 },

    { MOVE, WAY_B_X, WAY_B_Y },
    { SHOOT, 0, 0 },
    { DISABLE_B, 0, 0 },
    { RAMP_DFW, 0, 0 },

    { MOVE, WAY_SS34_X, WAY_SS34_Y },
    { MOVE_TO_WALL_FW, 0, 0 },
    { MOVE_ALONG_WALL_SHOOT_FW, WAY_S3_X, 0 },
    { MOVE_ALONG_WALL_FW, WAY_S4_X, 0 },

    { MOVE_ALONG_WALL_FW, WAY_SS34_X, 0 },
    { RAMP_URV, 0, 0 },
    { ENABLE_B, 0, 0 },

    { MOVE, WAY_C_X, WAY_C_Y },
    { SHOOT, 0, 0 },
    { DISABLE_F, 0, 0 },
    { RAMP_DRV, 0, 0 },

    { MOVE, WAY_SS12_X, WAY_SS12_Y }
};

int main(void){
    int actionCnt = 0;
    int runPullIn = 1;
    position_t pos = { .x = 0, .y = 0 };

    init_board();
    init_drivers();

    delay_ms(300);
    console_printf("Hello\r\n");
    led_set(LED_1 | LED_3);

    setResetFan(1);
    brushless_set_speed(14);
    
    /**
     * Wait until BTN_1 or BTN_2 is pressed
     *
     * If BTN_2 is pressed load preload balls and wait until
     * BTN_2 1 is pressed to start
     **/
    while(!btn_is_pressed(BTN_1) && !btn_is_pressed(BTN_2));
    if(btn_is_pressed(BTN_2)){
        while(loadBalls() == NOT_DONE){
            if(btn_is_pressed(BTN_1)){
                break;
            }
        }
    }

    /** Start Measurements **/
    vl53l0x_start();

    stateResponse_t response = NOT_DONE;
    stateResponse_t auxResp = NOT_DONE;
    action_t action = actions[actionCnt];

    enableDisableSensor(VL53L0X_F, 0);

    while(1){
        updatePosition(&pos);

        switch(action.actionType){
            case MOVE:
                response = drive_to(action.arg1, action.arg2, MOVE_TOL, &pos);
                break;
            case MOVE_ALONG_WALL_FW:
                response = drive_to(action.arg1, action.arg2, MOVE_TOL, &pos);
                driveAlongWallFW();
                break;
            case MOVE_ALONG_WALL_RV:
                response = drive_to(action.arg1, action.arg2, MOVE_TOL, &pos);
                driveAlongWallRV();
                break;
            case MOVE_ALONG_WALL_SHOOT_FW:
                if(auxResp == NOT_DONE){
                    auxResp = shootBalls();
                }
                response = drive_to(action.arg1, action.arg2, MOVE_TOL, &pos);
                driveAlongWallFW();
                break;
            case MOVE_ALONG_WALL_SHOOT_RV:
                if(auxResp == NOT_DONE){
                    auxResp = shootBalls();
                }
                response = drive_to(action.arg1, action.arg2, MOVE_TOL, &pos);
                driveAlongWallRV();
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
            case MOVE_TO_WALL_FW:
                response = driveToWallFW();
                break;
            case MOVE_TO_WALL_RV:
                response = driveToWallRV();
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
            case RAMP_DFW:
                response = rampDownFW();
                break;
            case RAMP_DRV:
                response = rampDownRV();
                break;
            default:
                actionCnt = 0;
        }
        
        if(response == DONE){
            if(action.actionType == SHOOT || action.actionType == MOVE_SHOOT){
                runPullIn = 1;
            }

            actionCnt = (actionCnt + 1) % NUM_ACTIONS;
            action = actions[actionCnt];

            response = NOT_DONE;
            auxResp = NOT_DONE;
        }

        if(runPullIn && loadBalls() == DONE){
            runPullIn = 0;
        }
    }
}
