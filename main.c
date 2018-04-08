#include <avr32/io.h>
#include <stdlib.h>
#include <stdio.h>

#include "board.h"
#include "brushless.h"
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
#include "led.h"
#include "btn.h"

typedef enum STATE{
    STATE_S1,
    STATE_S2,
    STATE_SS12,
    STATE_S3,
    STATE_S4,
    STATE_SS34,
    STATE_R1,
    STATE_R2,
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_D
}STATE;
/** STATES are defined by the Waypoint they are trying to reach **/

 /*************S****************************S************
 *                                                      *
 *            S3           SS34            S4           *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                          A                           *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                         R2                           *
 *                                                      *
 *            IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII           *
 ********************************************************
 *                                                      *
 *                                                      *
 *                                                      *
 *                          B                           *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                          C                           *
 *                                                      *
 *                                                      *
 *                                                      *
 ********************************************************
 *            IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII           *
 *                                                      *
 *                         R1                           *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                          D                           *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *          S1            SS12              S2          *
 *                                                      *
 ************S*******************************S**********/

int main(void){
    /** This clock is not used **/
    /** This function does not even work :( **/
    //scif_enable_OSC32K();

    init_board();
    init_drivers();

    console_printf("Hello\r\n");
    led_set(LED_1 | LED_3);

    brushless_set_speed(0);
    delay_ms(5000);
    brushless_set_speed(255);
    

    
    while(!btn_is_pressed(BTN_1));

    /**
    AVR32_GPIO.port[1].oders = 0x01;
    AVR32_GPIO.port[1].ovrc = 0x01;
    AVR32_GPIO.port[1].gpers = 0x01;

    uint16_t xVal;
    AVR32_GPIO.port[1].ovrs = 0x01;
    delay_ms(1);
    AVR32_GPIO.port[1].ovrc = 0x01;
    xVal = getXPosition();
    AVR32_GPIO.port[1].ovrs = 0x01;

    while(1);
    **/

    //vl53l0x_calibrate_all();
    
    //drive_motors_ramp(DRV8711_FL, DRV8711_BR, 0, 1, 255);
    //drive_motors_ramp(DRV8711_FR, DRV8711_BL, 0, 1, 0);

    /**
    uint16_t xVal, yVal;

    while(1){
        xVal = getXPosition();
        yVal = getYPosition();

        console_printf("Position: (%u,%u)\r\n", xVal, yVal);
    }
    **/

    while(1){
        drive_toX(610, 8);
    }

    /**
    STATE state = STATE_SS12;
    while(1){
        switch(state){
            case STATE_S1:
                drive_to(WAY_S1_X, WAY_S1_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_S2:
                drive_to(WAY_S2_X, WAY_S2_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_SS12:
                drive_to(WAY_SS12_X, WAY_SS12_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_S3:
                drive_to(WAY_S3_X, WAY_S3_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_S4:
                drive_to(WAY_S4_X, WAY_S4_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_SS34:
                drive_to(WAY_SS34_X, WAY_SS34_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_R1:
                drive_to(WAY_R1_X, WAY_R1_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_R2:
                drive_to(WAY_R2_X, WAY_R2_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_A:
                drive_to(WAY_A_X, WAY_A_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_B:
                drive_to(WAY_B_X, WAY_B_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_C:
                drive_to(WAY_C_X, WAY_C_Y, WAYPOINT_TOLERANCE);
                break;
            case STATE_D:
                drive_to(WAY_D_X, WAY_D_Y, WAYPOINT_TOLERANCE);
                break;
        }

        xVal = getXPosition();
        yVal = getYPosition();

        console_printf("Current Position (%u, %u)\r\n", xVal, yVal);

    }
    **/
}

void print_flash(){
    led_set(LED_1 | LED_4);

    uint32_t* flashAddr = FLASH_DEBUG_START;
    uint32_t word;
    int byteOffset;
    uint8_t byte;

    while(flashAddr < MAX_FLASH){
        word = (*flashAddr >> byteOffset) & 0xFF;
        console_printf("THe num %u\r\n", word);

        for(byteOffset = 24; byteOffset > -1; byteOffset -= 8){
            byte = (word >> byteOffset) & 0xFF;

            if(byte == 0xFF){
                break; 
            }

            console_print((char)(byte));
        }

        if(byte == 0xFF){
            break;
        }

        flashAddr++;
    }

    while(1){
        led_set(LED_1 | LED_4);
        delay_ms(500);
        led_clear(LED_1 | LED_4);
        delay_ms(500);
    }
}
