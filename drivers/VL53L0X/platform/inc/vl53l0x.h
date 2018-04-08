#ifndef VL53L0X_H_
#define VL53L0X_H_

#include "vl53l0x_platform.h"
#include "vl53l0x_api.h"
#include "vl53l0x_api_calibration.h"
#include "vl53l0x_api_core.h"
#include "vl53l0x_api_ranging.h"
#include "vl53l0x_api_strings.h"
#include "board.h"
#include "state.h"
#include "motors.h"

#define VL53L0X_DEFAULT_ADDR 0x29

VL53L0X_Error vl53l0x_init(VL53L0X_DEV dev);

VL53L0X_Error vl53l0x_set_addresss(VL53L0X_DEV dev, uint8_t addr);

VL53L0X_Error vl53l0x_init_longrange(VL53L0X_DEV dev);

VL53L0X_Error vl53l0x_calibrate(VL53L0X_DEV dev);

void vl53l0x_calibrate_all();

void vl53l0x_print_error(VL53L0X_Error err);

void vl53l0x_init_all();

void vl53l0x_start();

uint16_t calcXPosition(uint16_t l, uint16_t r);
uint16_t calcYPosition(uint16_t f, uint16_t b);

void updatePosition(position_t *pos);
#endif
