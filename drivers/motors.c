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

static uint8_t cur_speed[4] = {
    0, 0, 0, 0
};

/**
uint8_t SCALE_LUT[256] = {


}
**/

void drive_motor(DRV8711_ID id, uint8_t dir, uint8_t torque, uint8_t speed){
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
            drv8711_write_torque(id, torque, SMPLTH_50_US);
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

void drive_motor_ramp(DRV8711_ID id, uint8_t dir, uint8_t torque, uint8_t speed){
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
        drv8711_write_torque(id, torque, SMPLTH_50_US);
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
void drive_motors_ramp(DRV8711_ID id1, DRV8711_ID id2, uint8_t dir1, uint8_t dir2, uint8_t torque, uint8_t speed){
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
        drv8711_write_torque(id1, torque, SMPLTH_50_US);
        drv8711_write_torque(id2, torque, SMPLTH_50_US);
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

stateResponse_t drive_to(uint16_t x, uint16_t y, uint16_t tol, position_t *curPos){
    int xInRange = 0, yInRange = 0;
    int difX, difY;

    /** Zero values are Invalid and should not be considered **/
    if(curPos->x){
        if(curPos->x < (x + tol) && curPos->x > (x - tol)){
            xInRange = 1;
            drive_motor(DRV8711_FR, 0, DRV8711_ON_TORQUE, 0);
            drive_motor(DRV8711_BL, 1, DRV8711_ON_TORQUE, 0);
        }else{
            difX = x - curPos->x;
            if(difX > 0){
                drive_motors_ramp(DRV8711_FR, DRV8711_BL, 0, 1, DRV8711_ON_TORQUE, difX >> 2);
            }else{
                difX *= -1;
                drive_motors_ramp(DRV8711_FR, DRV8711_BL, 1, 0, DRV8711_ON_TORQUE, difX >> 2);
            }
        }
    }

    /** Zero values are Invalid and should not be considered **/
    if(curPos->y){
        if(curPos->y < (y + tol) && curPos->y > (y - tol)){
            yInRange = 1;
            drive_motor(DRV8711_FL, 0, DRV8711_ON_TORQUE, 0);
            drive_motor(DRV8711_BR, 1, DRV8711_ON_TORQUE, 0);
        }else{
            difY = y - curPos->y;
            if(difY > 0){
                drive_motors_ramp(DRV8711_FL, DRV8711_BR, 0, 1, DRV8711_ON_TORQUE, difY >> 3);
            }else{
                difX *= -1;
                drive_motors_ramp(DRV8711_FL, DRV8711_BR, 1, 0, DRV8711_ON_TORQUE, difY >> 3);
            }
        }
    }


    return xInRange && yInRange ? DONE : NOT_DONE;
}
