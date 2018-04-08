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

typedef enum actionType{
    MOVE, SHOOT
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

#define WAY_S1_X 0
#define WAY_S1_Y 0
#define WAY_S2_X 0
#define WAY_S2_Y 0
#define WAY_SS12_X 0
#define WAY_SS12_Y 0
#define WAY_S3_X 0
#define WAY_S3_Y 0
#define WAY_S4_X 0
#define WAY_S4_Y 0
#define WAY_SS34_X 0
#define WAY_SS34_Y 0
#define WAY_A_X 0
#define WAY_A_Y 0
#define WAY_B_X 0
#define WAY_B_Y 0
#define WAY_C_X 0
#define WAY_C_Y 0
#define WAY_D_X 0
#define WAY_D_Y 0

action_t actions[14] = {
    { MOVE, WAY_SS12_X, WAY_SS12_Y },
    { MOVE, WAY_S1_X, WAY_S1_Y },
    { MOVE, WAY_S2_X, WAY_S2_Y },
    { MOVE, WAY_D_X, WAY_D_Y },
    { SHOOT, 128, 0 },
    { MOVE, WAY_A_X, WAY_A_Y },
    { SHOOT, 128, 0 },

    { MOVE, WAY_SS34_X, WAY_SS34_Y },
    { MOVE, WAY_S3_X, WAY_S3_Y },
    { MOVE, WAY_S4_X, WAY_S4_Y },
    { MOVE, WAY_B_X, WAY_B_Y },
    { SHOOT, 128, 0 },
    { MOVE, WAY_C_X, WAY_C_Y },
    { SHOOT, 128, 0 },
};

int main(void){
    int actionCnt = 0;
    position_t pos = { .x = 0, .y = 0 };

    init_board();
    init_drivers();

    console_printf("Hello\r\n");
    led_set(LED_1 | LED_3);
    
    while(!btn_is_pressed(BTN_1));

    /** Start Measurements **/
    vl53l0x_start();

    stateResponse_t response = NOT_DONE;
    action_t action = actions[actionCnt];

    while(1){
        updatePosition(&pos);

        drive_to(610, 1220, MOVE_TOL, &pos);
        
        /**
        switch(action.actionType){
            case MOVE:
                response = drive_to(action.arg1, action.arg2, MOVE_TOL, &pos);
                break;
            case SHOOT:
                response = shoot_balls(50);
                break;
            default:
                actionCnt = 0;
        }
        
        if(response == DONE){
            actionCnt = (actionCnt + 1) % sizeof(actions);
        }
        **/
    }
}

