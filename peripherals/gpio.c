#include <avr32/io.h>
#include "gpio.h"
#include "board.h"

void GPIO_INIT(){
    gpioInit gi;

    /** VL53L0X Shutdown Pins **/
    gi.pins = VL53L0X_SD_PIN_R | VL53L0X_SD_PIN_L | VL53L0X_SD_PIN_F | VL53L0X_SD_PIN_B;
    gi.type = GPIO_OUTPUT;
    gi.port = VL53L0X_PORT;
    gpio_init(&gi);

    /** VL53L0X Voltage Regulator Pin **/
    gi.pins = VL53L0X_AVDD_PIN;
    gi.type = GPIO_OUTPUT;
    gi.port = VL53L0X_AVDD_PORT;
    gpio_init(&gi);

    /** DRV8711 NSLP and RESET Pins **/
    gi.pins = DRV8711_NSLP_PIN | DRV8711_RST_PIN;
    gi.type = GPIO_OUTPUT;
    gi.port = DRV8711_CTRL_PORT;
    gpio_init(&gi);

    /** SLP is Active Low **/
    AVR32_GPIO.port[DRV8711_CTRL_PORT].ovrs =  DRV8711_NSLP_PIN;

    /** DRV8711 Direction Pins **/
    gi.pins = DRV8711_DIR_PIN_A | DRV8711_DIR_PIN_B | DRV8711_DIR_PIN_C | DRV8711_DIR_PIN_D;
    gi.type = GPIO_OUTPUT;
    gi.port = DRV8711_DIR_PORT;
    gpio_init(&gi);

    /** Button Inputs **/
    gi.pins = GPIO_PIN_16 | GPIO_PIN_17;
    gi.type = GPIO_INPUT_PU;
    gi.port = PORT_D;
    gpio_init(&gi);

    /** LED Outputs **/
    gi.pins = GPIO_PIN_21 | GPIO_PIN_20 | GPIO_PIN_19 | GPIO_PIN_18;
    gi.type = GPIO_OUTPUT;
    gi.port = PORT_D;
    gpio_init(&gi);
}

void gpio_init(gpioInit* gi){
    switch(gi -> type){
        case GPIO_OUTPUT:
            AVR32_GPIO.port[gi->port].oders = gi->pins;
            AVR32_GPIO.port[gi->port].ovrc = gi->pins;
            AVR32_GPIO.port[gi->port].gpers = gi->pins;
            break;
        case GPIO_INPUT_NOPULL:
            AVR32_GPIO.port[gi->port].oderc = gi->pins;
            AVR32_GPIO.port[gi->port].puerc = gi->pins;
            AVR32_GPIO.port[gi->port].pderc = gi->pins;
            AVR32_GPIO.port[gi->port].gpers = gi->pins;
            break;
        case GPIO_INPUT_PU:
            AVR32_GPIO.port[gi->port].oderc = gi->pins;
            AVR32_GPIO.port[gi->port].puers = gi->pins;
            AVR32_GPIO.port[gi->port].pderc = gi->pins;
            AVR32_GPIO.port[gi->port].gpers = gi->pins;
            break;
        case GPIO_INPUT_PD:
            AVR32_GPIO.port[gi->port].oderc = gi->pins;
            AVR32_GPIO.port[gi->port].puerc = gi->pins;
            AVR32_GPIO.port[gi->port].pders = gi->pins;
            AVR32_GPIO.port[gi->port].gpers = gi->pins;
            break;
        case GPIO_AF:
            switch(gi -> af){
                case FUNC_A:
                    AVR32_GPIO.port[gi->port].pmr2c = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr1c = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr0c = gi->pins;
                    break;
                case FUNC_B:
                    AVR32_GPIO.port[gi->port].pmr2c = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr1c = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr0s = gi->pins;
                    break;
                case FUNC_C:
                    AVR32_GPIO.port[gi->port].pmr2c = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr1s = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr0c = gi->pins;
                    break;
                case FUNC_D:
                    AVR32_GPIO.port[gi->port].pmr2c = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr1s = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr0s = gi->pins;
                    break;
                case FUNC_E:
                    AVR32_GPIO.port[gi->port].pmr2s = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr1c = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr0c = gi->pins;
                    break;
                case FUNC_F:
                    AVR32_GPIO.port[gi->port].pmr2s = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr1c = gi->pins;
                    AVR32_GPIO.port[gi->port].pmr0s = gi->pins;
                    break;
            }

            AVR32_GPIO.port[gi->port].gperc = gi->pins;
            break;
    }
}
