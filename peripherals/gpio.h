#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdint.h>

#define GPIO_PIN_0 ((uint32_t)1<<0)
#define GPIO_PIN_1 ((uint32_t)1<<1)
#define GPIO_PIN_2 ((uint32_t)1<<2)
#define GPIO_PIN_3 ((uint32_t)1<<3)
#define GPIO_PIN_4 ((uint32_t)1<<4)
#define GPIO_PIN_5 ((uint32_t)1<<5)
#define GPIO_PIN_6 ((uint32_t)1<<6)
#define GPIO_PIN_7 ((uint32_t)1<<7)
#define GPIO_PIN_8 ((uint32_t)1<<8)
#define GPIO_PIN_9 ((uint32_t)1<<9)
#define GPIO_PIN_10 ((uint32_t)1<<10)
#define GPIO_PIN_11 ((uint32_t)1<<11)
#define GPIO_PIN_12 ((uint32_t)1<<12)
#define GPIO_PIN_13 ((uint32_t)1<<13)
#define GPIO_PIN_14 ((uint32_t)1<<14)
#define GPIO_PIN_15 ((uint32_t)1<<15)
#define GPIO_PIN_16 ((uint32_t)1<<16)
#define GPIO_PIN_17 ((uint32_t)1<<17)
#define GPIO_PIN_18 ((uint32_t)1<<18)
#define GPIO_PIN_19 ((uint32_t)1<<19)
#define GPIO_PIN_20 ((uint32_t)1<<20)
#define GPIO_PIN_21 ((uint32_t)1<<21)
#define GPIO_PIN_22 ((uint32_t)1<<22)
#define GPIO_PIN_23 ((uint32_t)1<<23)
#define GPIO_PIN_24 ((uint32_t)1<<24)
#define GPIO_PIN_25 ((uint32_t)1<<25)
#define GPIO_PIN_26 ((uint32_t)1<<26)
#define GPIO_PIN_27 ((uint32_t)1<<27)
#define GPIO_PIN_28 ((uint32_t)1<<28)
#define GPIO_PIN_29 ((uint32_t)1<<29)
#define GPIO_PIN_30 ((uint32_t)1<<30)
#define GPIO_PIN_31 ((uint32_t)1<<31)

typedef enum GPIO_FUNC{
    FUNC_A = 0,
    FUNC_B,
    FUNC_C,
    FUNC_D,
    FUNC_E,
    FUNC_F
}GPIO_FUNC;

typedef enum GPIO_TYPE{
    GPIO_OUTPUT = 0,
    GPIO_INPUT_NOPULL,
    GPIO_INPUT_PU,
    GPIO_INPUT_PD,
    GPIO_AF
}GPIO_TYPE;

typedef enum GPIO_PORT{
    PORT_A = 0,
    PORT_B,
    PORT_C,
    PORT_D
}GPIO_PORT;

typedef struct gpioInit{
    uint32_t pins;
    GPIO_TYPE type;
    GPIO_PORT port;
    GPIO_FUNC af;
}gpioInit;

void GPIO_INIT();
void gpio_init(gpioInit* gi);

#endif
