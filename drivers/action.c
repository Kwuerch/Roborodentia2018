#include "action.h"
#include "gameStateHandler.h"
#include "shootControl.h"
#include "vl53l0x.h"

#define MOVE_TOL 7 

stateResponse_t actionHandler(action_t* action, position_t* pos){
    static int runPullIn = 0;

    static stateResponse_t response = NOT_DONE;
    static stateResponse_t auxResp = NOT_DONE;

    stateResponse_t handlerResponse = NOT_DONE;
    
    switch(action->actionType){
        case MOVE:
            response = drive_to(action->arg1, action->arg2, MOVE_TOL, pos);
            break;
        case MOVE_ALONG_WALL_FW:
            response = drive_to(action->arg1, action->arg2, MOVE_TOL, pos);
            driveAlongWallFW();
            break;
        case MOVE_ALONG_WALL_RV:
            response = drive_to(action->arg1, action->arg2, MOVE_TOL, pos);
            driveAlongWallRV();
            break;
        case MOVE_ALONG_WALL_SHOOT_FW:
            if(auxResp == NOT_DONE){
                auxResp = shootBalls();
            }
            response = drive_to(action->arg1, action->arg2, MOVE_TOL, pos);
            driveAlongWallFW();
            break;
        case MOVE_ALONG_WALL_SHOOT_RV:
            if(auxResp == NOT_DONE){
                auxResp = shootBalls();
            }
            response = drive_to(action->arg1, action->arg2, MOVE_TOL, pos);
            driveAlongWallRV();
            break;
        case SHOOT:
            response = shootBalls();
            break;
        case MOVE_SHOOT:
            if(auxResp == NOT_DONE){
                auxResp = shootBalls();
            }
            response = drive_to(action->arg1, action->arg2, MOVE_TOL, pos);
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
            response = rampUpFW(pos);
            break;
        case RAMP_URV:
            response = rampUpRV(pos);
            break;
        case RAMP_DFW:
            response = rampDownFW();
            break;
        case RAMP_DRV:
            response = rampDownRV();
            break;
        default:
            return DONE;
            break;
    }

    if(response == DONE){
        if(action->actionType == SHOOT || action->actionType == MOVE_SHOOT){
            runPullIn = 1;
        }
        
        handlerResponse = DONE;
        response = NOT_DONE;
        auxResp = NOT_DONE;
    }

    if(runPullIn && loadBalls() == DONE){
        runPullIn = 0;
    }

    return handlerResponse;
}
