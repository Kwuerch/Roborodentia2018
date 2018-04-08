#ifndef _BRUSHLESS_H_
#define _BRUSHLESS_H_

#include <stdint.h>
#include "state.h"

void brushless_set_speed(uint8_t speed);

stateResponse_t shoot_balls(uint8_t speed);

#endif
