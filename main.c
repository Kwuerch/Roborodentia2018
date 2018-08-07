#include <avr32/io.h>
#include <stdlib.h>
#include <stdio.h>

#include "action.h"
#include "brushless.h"
#include "btn.h"
#include "console.h"
#include "delay.h"
#include "led.h"
#include "shootControl.h"
#include "vl53l0x.h"
#include "waypoints.h"

action_t actions[] = {
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
    int actionIdx = 0;
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

    action_t action = actions[actionIdx];

    enableDisableSensor(VL53L0X_F, 0);

    while(1){
        updatePosition(&pos);
        if(actionHandler(&action, &pos) == DONE){
            actionIdx = (actionIdx + 1) % (sizeof(actions)/sizeof(action_t));
            action = actions[actionIdx];
        }
    }
}
