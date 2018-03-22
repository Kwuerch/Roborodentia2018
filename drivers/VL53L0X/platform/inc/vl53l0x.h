#ifndef VL53L0X_H_
#define VL53L0X_H_

#include "vl53l0x_platform.h"
#include "vl53l0x_api.h"
#include "vl53l0x_api_calibration.h"
#include "vl53l0x_api_core.h"
#include "vl53l0x_api_ranging.h"
#include "vl53l0x_api_strings.h"
#include "board.h"

#define VL53L0X_DEFAULT_ADDR 0x29

VL53L0X_Error vl53l0x_init(VL53L0X_DEV dev);

VL53L0X_Error vl53l0x_set_addresss(VL53L0X_DEV dev, uint8_t addr);

VL53L0X_Error vl53l0x_init_longrange(VL53L0X_DEV dev);

VL53L0X_Error vl53l0x_calibrate(VL53L0X_DEV dev);
void vl53l0x_calibrate_all();

void vl53l0x_print_error(VL53L0X_Error err);

void vl53l0x_init_all();

uint16_t vl53l0x_measure(VL53L0X_ID id);

uint16_t getXPosition();
uint16_t getYPosition();

void positionBufferXClear();
void positionBufferYClear();

#endif
