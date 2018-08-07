#ifndef _GAMESTATEHANDLER_H_
#define _GAMESTATEHANDLER_H_

#include "state.h"
#include "motors.h"

stateResponse_t driveToWallFW();
stateResponse_t driveToWallRV();
stateResponse_t driveAlongWallFW();
stateResponse_t driveAlongWallRV();
stateResponse_t rampUpFW(position_t *pos);
stateResponse_t rampUpRV(position_t *pos);
stateResponse_t rampDownFW();
stateResponse_t rampDownRV();

#endif
