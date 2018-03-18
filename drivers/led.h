#ifndef _LED_H_
#define _LED_H_

#include <stdint.h>
#include "gpio.h"

#define LED_1 GPIO_PIN_18
#define LED_2 GPIO_PIN_19
#define LED_3 GPIO_PIN_20
#define LED_4 GPIO_PIN_21

#define LED_PORT PORT_D

void led_set(uint32_t ledMask);
void led_clear(uint32_t ledMask);

#endif
