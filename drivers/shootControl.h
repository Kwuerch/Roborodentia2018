#ifndef _SHOOT_CONTROL_H_
#define _SHOOT_CONTROL_H_

#include <stdint.h>
#include "state.h"

stateResponse_t loadBalls();
void setResetFan(uint8_t set);
stateResponse_t shootBalls();

#endif
