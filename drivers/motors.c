#include <avr32/io.h>
#include <stdint.h>
#include "board.h"
#include "drv8711.h"
#include "vl53l0x.h"
#include "delay.h"
#include "console.h"

#define RAMP_DELAY_MS 3

uint32_t SPEED_LUT[256] = {
    0,62244,58992,56063,53412,51000,48796,46775,
    44914,43196,41605,40126,38750,37464,36261,35133,
    34073,33075,32134,31245,30403,29606,28850,28131,
    27447,26796,26175,25582,25015,24473,23954,23456,
    22979,22521,22081,21657,21250,20857,20479,20114,
    19762,19422,19094,18776,18469,18172,17884,17605,
    17335,17073,16819,16572,16332,16099,15873,15653,
    15439,15231,15028,14831,14638,14451,14268,14090,
    13917,13747,13582,13421,13263,13109,12959,12811,
    12668,12527,12390,12255,12124,11995,11869,11745,
    11625,11506,11390,11276,11165,11055,10948,10843,
    10740,10639,10540,10442,10346,10252,10160,10070,
    9981,9893,9807,9723,9640,9558,9478,9399,
    9321,9245,9170,9096,9023,8952,8882,8812,
    8744,8677,8611,8545,8481,8418,8356,8294,
    8234,8174,8116,8058,8001,7944,7889,7834,
    7780,7727,7674,7622,7571,7521,7471,7422,
    7374,7326,7279,7232,7186,7140,7096,7051,
    7007,6964,6922,6879,6838,6797,6756,6716,
    6676,6637,6598,6560,6522,6484,6447,6411,
    6375,6339,6303,6268,6234,6200,6166,6132,
    6099,6066,6034,6002,5970,5939,5908,5877,
    5846,5816,5786,5757,5728,5699,5670,5642,
    5614,5586,5559,5531,5504,5478,5451,5425,
    5399,5373,5348,5323,5298,5273,5249,5224,
    5200,5176,5153,5129,5106,5083,5060,5038,
    5015,4993,4971,4949,4928,4906,4885,4864,
    4843,4823,4802,4782,4762,4742,4722,4702,
    4683,4663,4644,4625,4606,4587,4569,4550,
    4532,4514,4496,4478,4461,4443,4426,4408,
    4391,4374,4357,4341,4324,4307,4291,4275,
    4259,4243,4227,4211,4195,4180,4164,4149,
};

static uint8_t cur_speed[4] = {
    0, 0, 0, 0
};

/**
uint8_t SCALE_LUT[256] = {


}
**/

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


    if(dir){
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrs = pin;
    }else{
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrc = pin;
    }

    if(speed){
        if(cur_speed[id] == 0){
            drv8711_write_torque(id, DRV8711_ON_TORQUE, SMPLTH_50_US);
        }

        (tc -> channel)[chan].ra = SPEED_LUT[speed];
        (tc -> channel)[chan].rc = SPEED_LUT[speed] << 1;
    }else{
        (tc -> channel)[chan].ra = 0;
        (tc -> channel)[chan].rc = 0;
        if(cur_speed[id] != 0){
            drv8711_write_torque(id, DRV8711_OFF_TORQUE, SMPLTH_50_US);
        }
    }

    cur_speed[id] = speed;
}

void drive_motor_ramp(DRV8711_ID id, uint8_t dir, uint8_t speed){
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

    if(dir){
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrs = pin;
    }else{
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrc = pin;
    }

    if(speed != 0){
        drv8711_write_torque(id, DRV8711_ON_TORQUE, SMPLTH_50_US);
    }

    /**
     * Ramp Up for Faster Speed
     */
    while(cur_speed[id] < speed){
        cur_speed[id]++;
        (tc -> channel)[chan].ra = SPEED_LUT[cur_speed[id]];
        (tc -> channel)[chan].rc = SPEED_LUT[cur_speed[id]] << 1;
        delay_ms(2);
    }

    /**
     * Ramp Down for Slower Speed
     */
    while(cur_speed[id] > speed){
        cur_speed[id]--;
        (tc -> channel)[chan].ra = SPEED_LUT[cur_speed[id]];
        (tc -> channel)[chan].rc = SPEED_LUT[cur_speed[id]] << 1;
        delay_ms(2);
    }

    /**
     * Set Torque Low to decrease Current Consumption if Speed is Zero
     */
    if(cur_speed[id] == 0){
        (tc -> channel)[chan].ra = 0;
        (tc -> channel)[chan].rc = 0;
        drv8711_write_torque(id, DRV8711_OFF_TORQUE, SMPLTH_50_US);
    }
}

/**
 * Precondition - Motors are currently running at the same speed
 *
 */
void drive_motors_ramp(DRV8711_ID id1, DRV8711_ID id2, uint8_t dir1, uint8_t dir2, uint8_t speed){
    uint32_t pin1;
    uint8_t chan1;
    volatile avr32_tc_t* tc1;
    uint32_t pin2;
    uint8_t chan2;
    volatile avr32_tc_t* tc2;

    switch(id1){
        case DRV8711_FL:
            pin1 = DRV8711_DIR_PIN_A;
            tc1 = DRV8711_STEP_TC_AB; 
            chan1 = DRV8711_STEP_CHAN_AC;
            break;
        case DRV8711_BR:
            pin1 = DRV8711_DIR_PIN_B;
            tc1 = DRV8711_STEP_TC_AB; 
            chan1 = DRV8711_STEP_CHAN_BD;
            break;
        case DRV8711_FR:
            pin1 = DRV8711_DIR_PIN_C;
            tc1 = DRV8711_STEP_TC_CD; 
            chan1 = DRV8711_STEP_CHAN_AC;
            break;
        case DRV8711_BL:
            pin1 = DRV8711_DIR_PIN_D;
            tc1 = DRV8711_STEP_TC_CD; 
            chan1 = DRV8711_STEP_CHAN_BD;
            break;
        default:
            return;
    }

    switch(id2){
        case DRV8711_FL:
            pin2 = DRV8711_DIR_PIN_A;
            tc2 = DRV8711_STEP_TC_AB; 
            chan2 = DRV8711_STEP_CHAN_AC;
            break;
        case DRV8711_BR:
            pin2 = DRV8711_DIR_PIN_B;
            tc2 = DRV8711_STEP_TC_AB; 
            chan2 = DRV8711_STEP_CHAN_BD;
            break;
        case DRV8711_FR:
            pin2 = DRV8711_DIR_PIN_C;
            tc2 = DRV8711_STEP_TC_CD; 
            chan2 = DRV8711_STEP_CHAN_AC;
            break;
        case DRV8711_BL:
            pin2 = DRV8711_DIR_PIN_D;
            tc2 = DRV8711_STEP_TC_CD; 
            chan2 = DRV8711_STEP_CHAN_BD;
            break;
        default:
            return;
    }

    if(dir1){
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrs = pin1;
    }else{
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrc = pin1;
    }

    if(dir2){
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrs = pin2;
    }else{
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrc = pin2;
    }

    if(speed != 0){
        drv8711_write_torque(id1, DRV8711_ON_TORQUE, SMPLTH_50_US);
        drv8711_write_torque(id2, DRV8711_ON_TORQUE, SMPLTH_50_US);
    }

    /**
     * Ramp Up for Faster Speed
     */
    while(cur_speed[id1] < speed){
        cur_speed[id1]++;
        cur_speed[id2]++;
        (tc1 -> channel)[chan1].ra = SPEED_LUT[cur_speed[id1]];
        (tc1 -> channel)[chan1].rc = SPEED_LUT[cur_speed[id1]] << 1;
        (tc2 -> channel)[chan2].ra = SPEED_LUT[cur_speed[id2]];
        (tc2 -> channel)[chan2].rc = SPEED_LUT[cur_speed[id2]] << 1;
        delay_ms(RAMP_DELAY_MS);
    }

    /**
     * Ramp Down for Slower Speed
     */
    while(cur_speed[id1] > speed){
        cur_speed[id1]--;
        cur_speed[id2]--;
        (tc1 -> channel)[chan1].ra = SPEED_LUT[cur_speed[id1]];
        (tc1 -> channel)[chan1].rc = SPEED_LUT[cur_speed[id1]] << 1;
        (tc2 -> channel)[chan2].ra = SPEED_LUT[cur_speed[id2]];
        (tc2 -> channel)[chan2].rc = SPEED_LUT[cur_speed[id2]] << 1;
        delay_ms(RAMP_DELAY_MS);
    }

    /**
     * Set Torque Low to decrease Current Consumption if Speed is Zero
     */
    if(cur_speed[id1] == 0){
        (tc1 -> channel)[chan1].ra = 0;
        (tc1 -> channel)[chan1].rc = 0;
        (tc2 -> channel)[chan2].ra = 0;
        (tc2 -> channel)[chan2].rc = 0;
        drv8711_write_torque(id1, DRV8711_OFF_TORQUE, SMPLTH_50_US);
        drv8711_write_torque(id2, DRV8711_OFF_TORQUE, SMPLTH_50_US);
    }
}

void drive_toX(uint16_t x, uint16_t tol){
    int xInRange = 0;
    uint16_t curX;
    int difX;

    while(!xInRange){
        curX = getXPosition();

        if(curX < (x + tol) && curX > (x - tol)){
            drive_motor(DRV8711_FR, 0, 0);
            drive_motor(DRV8711_BL, 1, 0);
            xInRange = 1;

        }else{
            difX = x - curX;
            if(difX > 0){
                drive_motor(DRV8711_FR, 0, 64);
                drive_motor(DRV8711_BL, 1, 64);
            }else{
                difX *= -1;
                drive_motor(DRV8711_BL, 0, 64);
                drive_motor(DRV8711_FR, 1, 64);
            }
        }
    }

}


void drive_to(uint16_t x, uint16_t y, uint16_t tol){
    int xInRange = 0;
    int yInRange = 0;
    int curX, curY;
    int difX, difY;

    while(!(xInRange && yInRange)){
        curX = getXPosition();
        curY = getYPosition();

        if(curX < (x + tol) && curX > (x - tol)){
            drive_motors_ramp(DRV8711_FR, DRV8711_BL, 0, 1, 0);
            drive_motors_ramp(DRV8711_FR, DRV8711_BL, 0, 1, 0);
            xInRange = 1;

        }else{
            difX = x - curX;
            if(difX > 0){
                drive_motors_ramp(DRV8711_FR, DRV8711_BL, 0, 1, difX >> 3);
            }else{
                difX *= -1;
                drive_motors_ramp(DRV8711_FR, DRV8711_BL, 1, 0, difX >> 3);
            }
        }

        if(curY < (x + tol) && curY > (x - tol)){
            //moveY(0, 0);
            yInRange = 1;

        }else{
            difY = y - curY;
            //moveY(scale[difx],  (diff > 0));
        }
    }
}
