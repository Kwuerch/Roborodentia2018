#include <avr32/io.h>
#include "led.h"

void led_set(uint32_t ledMask){
    ledMask &= (LED_1 | LED_2 | LED_3 | LED_4);

    AVR32_GPIO.port[LED_PORT].ovrs = ledMask;
}

void led_clear(uint32_t ledMask){
    ledMask &= (LED_1 | LED_2 | LED_3 | LED_4);

    AVR32_GPIO.port[LED_PORT].ovrc = ledMask;
}
