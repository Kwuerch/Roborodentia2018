#ifndef _BTN_H_
#define _BTN_H_

#include <stdint.h>
#include "gpio.h"

#define BTN_1 GPIO_PIN_17
#define BTN_2 GPIO_PIN_16

#define BTN_PORT PORT_D

int btn_is_pressed(uint32_t btnMask);

#endif
