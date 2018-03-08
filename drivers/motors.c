#include <avr32/io.h>
#include <stdint.h>
#include "board.h"
uint32_t SPEED_LUT[256] = {
    25600,25500,25400,25300,25200,25100,25000,24900,
    24800,24700,24600,24500,24400,24300,24200,24100,
    24000,23900,23800,23700,23600,23500,23400,23300,
    23200,23100,23000,22900,22800,22700,22600,22500,
    22400,22300,22200,22100,22000,21900,21800,21700,
    21600,21500,21400,21300,21200,21100,21000,20900,
    20800,20700,20600,20500,20400,20300,20200,20100,
    20000,19900,19800,19700,19600,19500,19400,19300,
    19200,19100,19000,18900,18800,18700,18600,18500,
    18400,18300,18200,18100,18000,17900,17800,17700,
    17600,17500,17400,17300,17200,17100,17000,16900,
    16800,16700,16600,16500,16400,16300,16200,16100,
    16000,15900,15800,15700,15600,15500,15400,15300,
    15200,15100,15000,14900,14800,14700,14600,14500,
    14400,14300,14200,14100,14000,13900,13800,13700,
    13600,13500,13400,13300,13200,13100,13000,12900,
    12800,12700,12600,12500,12400,12300,12200,12100,
    12000,11900,11800,11700,11600,11500,11400,11300,
    11200,11100,11000,10900,10800,10700,10600,10500,
    10400,10300,10200,10100,10000,9900,9800,9700,
    9600,9500,9400,9300,9200,9100,9000,8900,
    8800,8700,8600,8500,8400,8300,8200,8100,
    8000,7900,7800,7700,7600,7500,7400,7300,
    7200,7100,7000,6900,6800,6700,6600,6500,
    6400,6300,6200,6100,6000,5900,5800,5700,
    5600,5500,5400,5300,5200,5100,5000,4900,
    4800,4700,4600,4500,4400,4300,4200,4100,
    4000,3900,3800,3700,3600,3500,3400,3300,
    3200,3100,3000,2900,2800,2700,2600,2500,
    2400,2300,2200,2100,2000,1900,1800,1700,
    1600,1500,1400,1300,1200,1100,1000,900,
    800,700,600,500,400,300,200,100
};

void drive_motor(DRV8711_ID id, uint8_t dir, uint8_t speed){
    uint32_t pin;
    uint8_t chan;
    volatile avr32_tc_t* tc;

    switch(id){
        case DRV8711_FL:
            pin = DRV8711_DIR_PIN_A;
            tc = DRV8711_STEP_TC_AB; 
            chan = DRV8711_STEP_CHAN_AC;
            break;
        case DRV8711_BR:
            pin = DRV8711_DIR_PIN_B;
            tc = DRV8711_STEP_TC_AB; 
            chan = DRV8711_STEP_CHAN_BD;
            break;
        case DRV8711_FR:
            pin = DRV8711_DIR_PIN_C;
            tc = DRV8711_STEP_TC_CD; 
            chan = DRV8711_STEP_CHAN_AC;
            break;
        case DRV8711_BL:
            pin = DRV8711_DIR_PIN_D;
            tc = DRV8711_STEP_TC_CD; 
            chan = DRV8711_STEP_CHAN_BD;
            break;
        default:
            return;
    }

    if(speed){
        (tc -> channel)[chan].ra = SPEED_LUT[speed];
        (tc -> channel)[chan].rc = SPEED_LUT[speed] << 1;
    }else{
        (tc -> channel)[chan].ra = 0;
        (tc -> channel)[chan].rc = 0;
        
    }

    if(dir){
        console_printf("Setting Pin\r\n");
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrs = pin;
    }else{
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrc = pin;
    }
}

void drive_motor_ramp(DRV8711_ID id, uint8_t speed){

}


/**
void drive_to(uint16_t x, uint16_t y, uint16_t tol){
    int xInRange = 0;
    int yInRange = 0;
    int curX, curY;
    int difX, difY;

    while(!(xInRange && yInRange)){
        curX = getCurX();
        curY = getCurY();

        if(curX < (x + tol) && curX > (x - tol)){
            moveX(0, 0);
            xInRange = 1;

        }else{
            difX = x - curX;
            moveX(scale[difx],  (diff > 0));
        }

        if(curY < (x + tol) && curY > (x - tol)){
            moveY(0, 0);
            yInRange = 1;

        }else{
            difY = y - curY;
            moveY(scale[difx],  (diff > 0));
        }
    }
}
**/
