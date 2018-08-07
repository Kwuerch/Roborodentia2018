#ifndef _ACTION_H_
#define _ACTION_H_

#include "state.h"
#include "motors.h"

typedef enum actionType{
    MOVE, SHOOT, MOVE_SHOOT, DISABLE_F, DISABLE_B, ENABLE_F, ENABLE_B,
    RAMP_UFW, RAMP_URV, RAMP_DFW, RAMP_DRV, MOVE_ALONG_WALL_FW,
    MOVE_ALONG_WALL_RV, MOVE_ALONG_WALL_SHOOT_FW, MOVE_ALONG_WALL_SHOOT_RV,
    MOVE_TO_WALL_RV, MOVE_TO_WALL_FW
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

stateResponse_t actionHandler(action_t* action, position_t* pos);

#endif
