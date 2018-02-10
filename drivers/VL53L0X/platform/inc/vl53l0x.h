#ifndef VL53L0X_H_
#define VL53L0X_H_

#include "vl53l0x_platform.h"
#include "vl53l0x_api.h"
#include "vl53l0x_api_calibration.h"
#include "vl53l0x_api_core.h"
#include "vl53l0x_api_ranging.h"
#include "vl53l0x_api_strings.h"

#define VL53L0X_DEFAULT_ADDR 0x29

#define VL53L0X_I2C_ADDR1 0x20
#define VL53L0X_I2C_ADDR2 0x21
#define VL53L0X_I2C_ADDR3 0x22
#define VL53L0X_I2C_ADDR4 0x23

VL53L0X_Error vl53l0x_init(VL53L0X_DEV dev);

VL53L0X_Error vl53l0x_set_addresss(VL53L0X_DEV dev, uint8_t addr);

VL53L0X_Error vl53l0x_init_longrange(VL53L0X_DEV dev);

VL53L0X_Error vl53l0x_calibrate(VL53L0X_DEV dev);

void vl53l0x_print_error(VL53L0X_Error err);

#endif
