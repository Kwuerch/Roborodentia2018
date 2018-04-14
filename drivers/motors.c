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
    100,100,100,101,101,101,102,102,
    103,103,103,104,104,105,105,105,
    106,106,107,107,107,108,108,108,
    109,109,110,110,110,111,111,112,
    112,112,113,113,114,114,114,115,
    115,116,116,116,117,117,117,118,
    118,119,119,119,120,120,121,121,
    121,122,122,123,123,123,124,124,
    125,125,125,126,126,126,127,127,
    128,128,128,129,129,130,130,130,
    131,131,132,132,132,133,133,133,
    134,134,135,135,135,136,136,137,
    137,137,138,138,139,139,139,140,
    140,141,141,141,142,142,142,143,
    143,144,144,144,145,145,146,146,
    146,147,147,148,148,148,149,149,
    150,150,150,151,151,151,152,152,
    153,153,153,154,154,155,155,155,
    156,156,157,157,157,158,158,158,
    159,159,160,160,160,161,161,162,
    162,162,163,163,164,164,164,165,
    165,166,166,166,167,167,167,168,
    168,169,169,169,170,170,171,171,
    171,172,172,173,173,173,174,174,
    175,175,175,176,176,176,177,177,
    178,178,178,179,179,180,180,180,
    181,181,182,182,182,183,183,183,
    184,184,185,185,185,186,186,187,
    187,187,188,188,189,189,189,190,
    190,191,191,191,192,192,192,193,
    193,194,194,194,195,195,196,196,
    196,197,197,198,198,198,199,199
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
    while(curSpeed > speed){
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
