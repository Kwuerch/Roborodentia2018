#ifndef VL53L0X_H_
#define VL53L0X_H_

#include "vl53l0x_platform.h"
#include "vl53l0x_api.h"
#include "vl53l0x_api_calibration.h"
#include "vl53l0x_api_core.h"
#include "vl53l0x_api_ranging.h"
#include "vl53l0x_api_strings.h"

#define I2C_ADDR 0x29

void vl53l0x_init(VL53L0X_DEV dev);

void vl53l0x_init_longrange(VL53L0X_DEV dev);

#endif
