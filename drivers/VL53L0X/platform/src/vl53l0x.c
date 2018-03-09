#include <avr32/io.h>
#include "board.h"
#include "console.h"
#include "delay.h"
#include "vl53l0x.h"

VL53L0X_Error vl53l0x_set_address(VL53L0X_DEV dev, uint8_t addr);

VL53L0X_Dev_t dev_f = {.I2cDevAddr = VL53L0X_ADDR_F, .pin = VL53L0X_SD_PIN_F};
VL53L0X_Dev_t dev_b = {.I2cDevAddr = VL53L0X_ADDR_B, .pin = VL53L0X_SD_PIN_B};
VL53L0X_Dev_t dev_r = {.I2cDevAddr = VL53L0X_ADDR_R, .pin = VL53L0X_SD_PIN_R};
VL53L0X_Dev_t dev_l = {.I2cDevAddr = VL53L0X_ADDR_L, .pin = VL53L0X_SD_PIN_L};


VL53L0X_Error vl53l0x_init(VL53L0X_DEV dev){
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    uint32_t refSpadCount;
    uint8_t isAperatureSpads;
    uint8_t vhvSettings;
    uint8_t phaseCal;

    if(dev -> I2cDevAddr != VL53L0X_DEFAULT_ADDR){
        uint8_t addr = dev -> I2cDevAddr;
        dev -> I2cDevAddr = VL53L0X_DEFAULT_ADDR;
        Status = vl53l0x_set_address(dev, addr); 
    }else{
        if (Status == VL53L0X_ERROR_NONE){
            Status = VL53L0X_DataInit(dev);
        }
    }

	if (Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_StaticInit(dev);
    }

    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isAperatureSpads);
    }

    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_PerformRefCalibration(dev, &vhvSettings, &phaseCal);
    }

    /**
     * VL53L0X_Set_offsetCalbirationDataMicroMeter();
     */


	if (Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);
    }

    if(Status != VL53L0X_ERROR_NONE){
        console_print_str("Initialization Error\r\n");
    }

    return Status;
}

VL53L0X_Error vl53l0x_set_address(VL53L0X_DEV dev, uint8_t addr){
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;

	if (Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_DataInit(dev);
    }

    if(Status == VL53L0X_ERROR_NONE && addr != VL53L0X_DEFAULT_ADDR){
        Status = VL53L0X_SetDeviceAddress(dev, addr*2);
        dev ->  I2cDevAddr = addr;
    }

    return Status;
}

VL53L0X_Error vl53l0x_calibrate(VL53L0X_DEV dev){
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    uint32_t refSpadCount;
    uint8_t isAperatureSpads;
    uint8_t vhvSettings;
    uint8_t phaseCal;
    int32_t offsetMicroMeter;
    FixPoint1616_t calDistanceMilliMeter = 200;
    FixPoint1616_t XTalkCalDistance = 200;
    FixPoint1616_t xTalkCompensationRate;


	if (Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_DataInit(dev);
    }

	if (Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_StaticInit(dev);
    }

    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_PerformRefSpadManagement(dev, &refSpadCount, &isAperatureSpads);
    }

    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_PerformRefCalibration(dev, &vhvSettings, &phaseCal);
    }

    console_print_str("Performing Offset Calibration in 3s... Set White Target\r\n");
    delay_ms(3000);

    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_PerformOffsetCalibration(dev, calDistanceMilliMeter, &offsetMicroMeter);
    }

    console_print_str("Performing XTalk Calibration in 3s... Set Grey Target\r\n");
    delay_ms(3000);

    if(Status == VL53L0X_ERROR_NONE){
        Status = VL53L0X_PerformXTalkCalibration(dev, XTalkCalDistance, &xTalkCompensationRate);
    }

    if(Status != VL53L0X_ERROR_NONE){
        console_print_str("Calibration Error\r\n");
    }

    while(1);

    return Status;
}

VL53L0X_Error vl53l0x_init_longrange(VL53L0X_DEV dev){
    VL53L0X_Error Status = VL53L0X_ERROR_NONE;

	if (Status == VL53L0X_ERROR_NONE)
        Status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));

	if (Status == VL53L0X_ERROR_NONE)
        Status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));

	if (Status == VL53L0X_ERROR_NONE)
        Status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, 33000);

	if (Status == VL53L0X_ERROR_NONE)
        Status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);

	if (Status == VL53L0X_ERROR_NONE)
        Status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);

    return Status;
}

void vl53l0x_print_error(VL53L0X_Error status){
    switch(status){
        case VL53L0X_ERROR_NONE:
            console_print_str("Error None\r\n");
            break;
        case VL53L0X_ERROR_CALIBRATION_WARNING:
            console_print_str("Error Calibration Warning\r\n");
            break;
        case VL53L0X_ERROR_MIN_CLIPPED:
            console_print_str("Error Min Clipped\r\n");
            break;
        case VL53L0X_ERROR_UNDEFINED:
            console_print_str("Error Undefined\r\n");
            break;
        case VL53L0X_ERROR_INVALID_PARAMS:
            console_print_str("Error Invalid Params\r\n");
            break;
        case VL53L0X_ERROR_NOT_SUPPORTED:
            console_print_str("Error Not Supported\r\n");
            break;
        case VL53L0X_ERROR_BUFFER_TOO_SMALL:
            console_print_str("Error Buffer Too Small\r\n");
            break;
        case VL53L0X_ERROR_GPIO_NOT_EXISTING:
            console_print_str("Error GPIO Not Existing\r\n");
            break;
        case VL53L0X_ERROR_GPIO_FUNCTIONALITY_NOT_SUPPORTED:
            console_print_str("Error GPIO Functionality Not Supported\r\n");
            break;
        case VL53L0X_ERROR_INTERRUPT_NOT_CLEARED:
            console_print_str("Error Interrupt Not Cleared\r\n");
            break;
        case VL53L0X_ERROR_CONTROL_INTERFACE:
            console_print_str("Error Control Interface\r\n");
            break;
        case VL53L0X_ERROR_INVALID_COMMAND:
            console_print_str("Error Invalid Command\r\n");
            break;
        case VL53L0X_ERROR_DIVISION_BY_ZERO:
            console_print_str("Error Division By Zero\r\n");
            break;
        case VL53L0X_ERROR_REF_SPAD_INIT:
            console_print_str("Error Ref Spad Init\r\n");
            break;
        case VL53L0X_ERROR_NOT_IMPLEMENTED:
            console_print_str("Error Not Implemented\r\n");
            break;
        default:
            console_print_str("Error Not Caught\r\n");
            break;
    }
}

void vl53l0x_init_dev(VL53L0X_ID id){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    VL53L0X_Dev_t* dev;

    switch(id){
        case VL53L0X_F:
            dev = &dev_f;
            break;

        case VL53L0X_B:
            dev = &dev_b;
            break;

        case VL53L0X_R:
            dev = &dev_r;
            break;

        case VL53L0X_L:
            dev = &dev_l;
            break;

        default:
            return;
    }

    AVR32_GPIO.port[VL53L0X_PORT].ovrs = dev -> pin; 
    console_printf("Pin %u is set high\r\n", dev -> pin);
    status = vl53l0x_init(dev);

    if(status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
    }
}

void vl53l0x_init_all(){
    /** Initialize Pins & Shutdown **/
    init_vl53l0x_sd();
    AVR32_GPIO.port[VL53L0X_AVDD_PORT].ovrs = (VL53L0X_AVDD_PIN);

    delay_ms(10);
    vl53l0x_init_dev(VL53L0X_F);
    vl53l0x_init_dev(VL53L0X_B);
    vl53l0x_init_dev(VL53L0X_R);
    vl53l0x_init_dev(VL53L0X_L);
}

static uint16_t bufL[3];
static uint16_t bufR[3];
static uint16_t bufF[3];
static uint16_t bufB[3];
static uint8_t bufLIdx = 0;
static uint8_t bufRIdx = 0;
static uint8_t bufFIdx = 0;
static uint8_t bufBIdx = 0;

/** Returns 0 for invalid measurement **/
uint16_t vl53l0x_measure(VL53L0X_ID id){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    VL53L0X_RangingMeasurementData_t meas;
    VL53L0X_Dev_t* dev;

    switch(id){
        case VL53L0X_F:
            dev = &dev_f;
            break;

        case VL53L0X_B:
            dev = &dev_b;
            break;

        case VL53L0X_R:
            dev = &dev_r;
            break;

        case VL53L0X_L:
            dev = &dev_l;
            break;

        default:
            return 0;
    }

    status = VL53L0X_PerformSingleRangingMeasurement(dev, &meas);

    if(status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
    }

    if(meas.RangeStatus){
        return 0;
    }

    return meas.RangeMilliMeter;
}

int16_t medOfThree(uint16_t arr[]){
    uint16_t medIdx;
    uint16_t minIdx;

    if(arr[0] < arr[1]){
        medIdx = 1;
        minIdx = 0;
    }else{
        medIdx = 0;
        minIdx = 1;
    }

    if(arr[2] < medIdx){
        medIdx = 2;
    }

    return arr[medIdx] > arr[minIdx] ? arr[medIdx] : arr[minIdx];
}

uint16_t getXPosition(){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    VL53L0X_RangingMeasurementData_t meas;

    /** Measure Left Side **/
    status = VL53L0X_PerformSingleRangingMeasurement(&dev_l, &meas);
    if(meas.RangeMilliMeter < MAX_RANGE){
        bufL[bufLIdx] = meas.RangeMilliMeter;
        bufLIdx = (bufLIdx + 1) % 3;
    }

    if(status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
    }

    /** Measure Right Side **/
    status = VL53L0X_PerformSingleRangingMeasurement(&dev_r, &meas);
    if(meas.RangeMilliMeter < MAX_RANGE){
        bufR[bufRIdx] = meas.RangeMilliMeter;
        bufRIdx = (bufRIdx + 1) % 3;
    }

    if(status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
    }

    uint16_t lVal = medOfThree(bufL);
    uint16_t rVal = medOfThree(bufR);

    console_printf("Lvalue %i, RValue %i", lVal, rVal);

    /** Values are Valid **/
    if(lVal && rVal){
        return ((lVal + BOARD_HALF_X_SIZE) + (FIELD_X_SIZE - BOARD_HALF_X_SIZE - rVal)) >> 1;
    }

    /** TODO - THIS MIGHT CAUSE ISSUES when hitting the ramp **/
    /** Assuming both will work or only one will work TODO - could be an issue **/
    if(!lVal){
        return FIELD_X_SIZE - BOARD_HALF_X_SIZE - rVal;
    }

    if(!rVal){
        return lVal + BOARD_HALF_X_SIZE;
    }

    /** TODO - This could totally mess up the dynamics of driving **/
    return 0;
}

/** TODO - Ignore values that are greater than max_range. Is this OK???? **/
uint16_t getYPosition(){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    VL53L0X_RangingMeasurementData_t meas;

    /** Measure Left Side **/
    status = VL53L0X_PerformSingleRangingMeasurement(&dev_f, &meas);
    if(meas.RangeMilliMeter < MAX_RANGE){
        bufF[bufFIdx] = meas.RangeMilliMeter;
        bufFIdx = (bufFIdx + 1) % 3;
    }

    if(status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
    }

    /** Measure Right Side **/
    status = VL53L0X_PerformSingleRangingMeasurement(&dev_b, &meas);
    if(meas.RangeMilliMeter < MAX_RANGE){
        bufB[bufBIdx] = meas.RangeMilliMeter;
        bufBIdx = (bufBIdx + 1) % 3;
    }

    if(status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
    }

    uint16_t fVal = medOfThree(bufF);
    uint16_t bVal = medOfThree(bufB);

    console_printf("Fvalue %i, BValue %i", fVal, bVal);

    /** Values are Valid **/
    if(fVal && bVal){
        return ((bVal + BOARD_HALF_Y_SIZE) + (FIELD_Y_SIZE - BOARD_HALF_Y_SIZE - fVal)) >> 1;
    }

    /** TODO - THIS MIGHT CAUSE ISSUES when hitting the ramp **/
    /** Assuming both will work or only one will work TODO - could be an issue **/
    if(!bVal){
        return FIELD_Y_SIZE - BOARD_HALF_Y_SIZE - fVal;
    }

    if(!fVal){
        return bVal + BOARD_HALF_Y_SIZE;
    }

    /** TODO - This could totally mess up the dynamics of driving **/
    return 0;
}
