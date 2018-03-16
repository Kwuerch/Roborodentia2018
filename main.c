#include <avr32/io.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "flash.h"
#include "pm.h"
#include "pwm.h"
#include "scif.h"
#include "tc.h"
#include "twi.h"
#include "usart.h"
#include "spi_master.h"
#include "gpio.h"
#include "console.h"
#include "delay.h"
#include "vl53l0x.h"
#include "drv8711.h"
#include "motors.h"


int main(void){
    /** This clock is not used **/
    /** This function does not even work :( **/
    //scif_enable_OSC32K();
    init_board();
    console_print_str("Hello\r\n");
    
    while(AVR32_GPIO.port[PORT_D].pvr & GPIO_PIN_17);
    
    drive_motors_ramp(DRV8711_FL, DRV8711_BR, 0, 1, 255);
    drive_motors_ramp(DRV8711_FR, DRV8711_BL, 0, 1, 0);

    while(1){
        drv8711_print_registers(DRV8711_FL);
    }
}
