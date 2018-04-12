#include <avr32/io.h>
#include <stdint.h>
#include "board.h"
#include "drv8711.h"
#include "vl53l0x.h"
#include "delay.h"
#include "console.h"
#include "motors.h"

#define RAMP_DELAY_MS 3

uint32_t SPEED_LUT[256] = {
    0,31431,30639,29886,29169,28486,27834,27211,
    26616,26046,25500,24976,24473,23990,23526,23080,
    22650,22236,21837,21451,21080,20720,20373,20038,
    19713,19398,19094,18799,18512,18235,17965,17704,
    17450,17203,16963,16730,16503,16282,16067,15857,
    15653,15454,15260,15071,14887,14706,14531,14359,
    14192,14028,13868,13712,13559,13409,13263,13120,
    12980,12843,12709,12577,12448,12322,12199,12077,
    11959,11842,11728,11616,11506,11398,11292,11188,
    11086,10986,10888,10791,10696,10603,10511,10421,
    10333,10246,10160,10076,9993,9912,9832,9753,
    9675,9599,9524,9450,9377,9305,9234,9165,
    9096,9029,8962,8897,8832,8768,8705,8644,
    8583,8522,8463,8405,8347,8290,8234,8178,
    8124,8070,8017,7964,7912,7861,7811,7761,
    7712,7663,7615,7568,7521,7475,7429,7384,
    7339,7295,7252,7209,7166,7124,7083,7042,
    7001,6961,6922,6882,6844,6805,6767,6730,
    6693,6656,6620,6584,6549,6514,6479,6445,
    6411,6377,6344,6311,6278,6246,6214,6183,
    6151,6120,6090,6059,6029,6000,5970,5941,
    5912,5883,5855,5827,5799,5772,5744,5717,
    5691,5664,5638,5612,5586,5561,5535,5510,
    5485,5461,5436,5412,5388,5364,5341,5317,
    5294,5271,5249,5226,5204,5181,5159,5138,
    5116,5095,5073,5052,5031,5011,4990,4970,
    4949,4929,4909,4890,4870,4851,4831,4812,
    4793,4774,4756,4737,4719,4701,4683,4665,
    4647,4629,4612,4594,4577,4560,4543,4526,
    4509,4492,4476,4459,4443,4427,4411,4395,
    4379,4363,4348,4332,4317,4302,4286,4271,
    4256,4242,4227,4212,4198,4183,4169,4155
};

uint8_t TORQUE_LUT[256] = {
    0,199,198,198,197,197,196,195,
    195,194,194,193,192,192,191,191,
    190,190,189,188,188,187,187,186,
    185,185,184,184,183,183,182,181,
    181,180,180,179,178,178,177,177,
    176,175,175,174,174,173,173,172,
    171,171,170,170,169,168,168,167,
    167,166,166,165,164,164,163,163,
    162,161,161,160,160,159,158,158,
    157,157,156,156,155,154,154,153,
    153,152,151,151,150,150,149,149,
    148,147,147,146,146,145,144,144,
    143,143,142,141,141,140,140,139,
    139,138,137,137,136,136,135,134,
    134,133,133,132,132,131,130,130,
    129,129,128,127,127,126,126,125,
    125,124,123,123,122,122,121,120,
    120,119,119,118,117,117,116,116,
    115,115,114,113,113,112,112,111,
    110,110,109,109,108,108,107,106,
    106,105,105,104,103,103,102,102,
    101,100,100,99,99,98,98,97,
    96,96,95,95,94,93,93,92,
    92,91,91,90,89,89,88,88,
    87,86,86,85,85,84,83,83,
    82,82,81,81,80,79,79,78,
    78,77,76,76,75,75,74,74,
    73,72,72,71,71,70,69,69,
    68,68,67,66,66,65,65,64,
    64,63,62,62,61,61,60,59,
    59,58,58,57,57,56,55,55,
    54,54,53,52,52,51,51,50
};

typedef struct motorInfo{
    uint32_t pin;
    uint8_t chan;
    volatile avr32_tc_t* tc;
}motorInfo_t;

motorInfo_t motors[4] = {
    {DRV8711_DIR_PIN_A, DRV8711_STEP_CHAN_AC, DRV8711_STEP_TC_AB}, /** FL **/
    {DRV8711_DIR_PIN_B, DRV8711_STEP_CHAN_BD, DRV8711_STEP_TC_AB}, /** BR **/
    {DRV8711_DIR_PIN_C, DRV8711_STEP_CHAN_AC, DRV8711_STEP_TC_CD}, /** FR **/
    {DRV8711_DIR_PIN_D, DRV8711_STEP_CHAN_BD, DRV8711_STEP_TC_CD}  /** BL **/
};

static uint8_t cur_speed[4] = {0};

void drive_motor(DRV8711_ID id, uint8_t dir, uint8_t speed){
    if(dir){
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrs = motors[id].pin;
    }else{
        AVR32_GPIO.port[DRV8711_DIR_PORT].ovrc = motors[id].pin;
    }

    if(cur_speed[id] != speed){
        drv8711_write_torque(id, TORQUE_LUT[speed], SMPLTH_50_US);
    }

    if(cur_speed[id] != speed){
        drv8711_write_torque(id, TORQUE_LUT[speed], SMPLTH_50_US);
        (motors[id].tc -> channel)[motors[id].chan].ra = SPEED_LUT[speed];
        (motors[id].tc -> channel)[motors[id].chan].rc = SPEED_LUT[speed] << 1;

        cur_speed[id] = speed;
    }
}

void drive_motor_ramp(DRV8711_ID id, uint8_t dir, uint8_t speed){
    /**
     * Ramp Up for Faster Speed
     */
    while(cur_speed[id] < speed){
        cur_speed[id]++;
        drive_motor(id, dir, speed); 
        delay_ms(RAMP_DELAY_MS);
    }

    /**
     * Ramp Down for Slower Speed
     */
    while(cur_speed[id] > speed){
        cur_speed[id]--;
        drive_motor(id, dir, speed); 
        delay_ms(RAMP_DELAY_MS);
    }
}

/**
 * Precondition - Motors are currently running at the same speed
 *
 */
void drive_motors_ramp(DRV8711_ID id1, DRV8711_ID id2, uint8_t dir1, uint8_t dir2, uint8_t speed){
    uint8_t curSpeed = cur_speed[id1];

    /**
     * Ramp Up for Faster Speed
     */
    while(curSpeed < speed){
        curSpeed++;

        drive_motor(id1, dir1, curSpeed);
        drive_motor(id2, dir2, curSpeed);

        delay_ms(RAMP_DELAY_MS);
    }

    /**
     * Ramp Down for Slower Speed
     */
    while(cur_speed[id1] > speed){
        curSpeed--;

        drive_motor(id1, dir1, curSpeed);
        drive_motor(id2, dir2, curSpeed);

        delay_ms(RAMP_DELAY_MS);
    }
}

stateResponse_t drive_to(uint16_t x, uint16_t y, uint16_t tol, position_t *curPos){
    int xInRange = 0, yInRange = 0;
    int difX, difY;

    /** Zero values are Invalid and should not be considered **/
    if(curPos->x){
        if(curPos->x < (x + tol) && curPos->x > (x - tol)){
            xInRange = 1;
            drive_motor(DRV8711_FR, 0, 0);
            drive_motor(DRV8711_BL, 1, 0);
        }else{
            difX = x - curPos->x;
            if(difX > 0){
                drive_motors_ramp(DRV8711_FR, DRV8711_BL, 0, 1, difX >> 2);
            }else{
                difX *= -1;
                drive_motors_ramp(DRV8711_FR, DRV8711_BL, 1, 0, difX >> 2);
            }
        }
    }

    /** Zero values are Invalid and should not be considered **/
    if(curPos->y){
        if(curPos->y < (y + tol) && curPos->y > (y - tol)){
            yInRange = 1;
            drive_motor(DRV8711_FL, 0, 0);
            drive_motor(DRV8711_BR, 1, 0);
        }else{
            difY = y - curPos->y;
            if(difY > 0){
                drive_motors_ramp(DRV8711_FL, DRV8711_BR, 0, 1, difY >> 3);
            }else{
                difY *= -1;
                drive_motors_ramp(DRV8711_FL, DRV8711_BR, 1, 0, difY >> 3);
            }
        }
    }


    return xInRange && yInRange ? DONE : NOT_DONE;
}
