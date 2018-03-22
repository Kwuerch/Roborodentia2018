#ifndef MOTORS_H_
#define MOTORS_H_

#include <stdint.h>
#include "board.h"

#define MOTOR_IN_RANGE 1

void drive_motor(DRV8711_ID id, uint8_t dir, uint8_t torque, uint8_t speed);
void drive_motor_ramp(DRV8711_ID id, uint8_t dir, uint8_t torque, uint8_t speed);
void drive_motors_ramp(DRV8711_ID id1, DRV8711_ID id2, uint8_t dir1, uint8_t dir2, uint8_t torque, uint8_t speed);
void drive_toX(uint16_t x, uint16_t y);

#endif
