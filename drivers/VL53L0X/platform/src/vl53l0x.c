#include <avr32/io.h>
#include "board.h"
#include "console.h"
#include "delay.h"
#include "vl53l0x.h"
#include "btn.h"

VL53L0X_Error vl53l0x_set_address(VL53L0X_DEV dev, uint8_t addr);

VL53L0X_Dev_t dev_f = {.I2cDevAddr = VL53L0X_ADDR_F, .pin = VL53L0X_SD_PIN_F, .enable = 1};
VL53L0X_Dev_t dev_b = {.I2cDevAddr = VL53L0X_ADDR_B, .pin = VL53L0X_SD_PIN_B, .enable = 1};
VL53L0X_Dev_t dev_r = {.I2cDevAddr = VL53L0X_ADDR_R, .pin = VL53L0X_SD_PIN_R, .enable = 1};
VL53L0X_Dev_t dev_l = {.I2cDevAddr = VL53L0X_ADDR_L, .pin = VL53L0X_SD_PIN_L, .enable = 1};

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
        Status = VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_CONTINUOUS_RANGING);
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

    console_print_str("Calibration Complete\r\n");

    return Status;
}

void vl53l0x_calibrate_all(){
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    console_print_str("Calibrating Front: Press Button 2\r\n");
    while(!btn_is_pressed(BTN_2));
    Status = vl53l0x_calibrate(&dev_f);

    console_print_str("Calibrating Right: Press Button 2\r\n");
    while(!btn_is_pressed(BTN_2));
    Status = vl53l0x_calibrate(&dev_r);

    console_print_str("Calibrating Back: Press Button 2\r\n");
    while(!btn_is_pressed(BTN_2));
    Status = vl53l0x_calibrate(&dev_b);

    console_print_str("Calibrating Left: Press Button 2\r\n");
    while(!btn_is_pressed(BTN_2));
    Status = vl53l0x_calibrate(&dev_l);

    if(Status != VL53L0X_ERROR_NONE){
        console_print_str("Calibration Error\r\n");
    }

    while(1);
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
    AVR32_GPIO.port[VL53L0X_AVDD_PORT].ovrs = (VL53L0X_AVDD_PIN);

    delay_ms(10);
    vl53l0x_init_dev(VL53L0X_F);
    vl53l0x_init_dev(VL53L0X_B);
    vl53l0x_init_dev(VL53L0X_R);
    vl53l0x_init_dev(VL53L0X_L);
}

static uint16_t bufL[4] = {0, 0, 0, START_L};
static uint16_t bufR[4] = {0, 0, 0, START_R};
static uint16_t bufF[4] = {0, 0, 0, START_F};
static uint16_t bufB[4] = {0, 0, 0, START_B};

static uint8_t bufLIdx = 0;
static uint8_t bufRIdx = 0;
static uint8_t bufFIdx = 0;
static uint8_t bufBIdx = 0;

/** vl53l0x_start starts the continuous measurement of all the sensors **/
void vl53l0x_start(){
    VL53L0X_StartMeasurement(&dev_f);
    VL53L0X_StartMeasurement(&dev_b);
    VL53L0X_StartMeasurement(&dev_r);
    VL53L0X_StartMeasurement(&dev_l);
}

stateResponse_t vl53l0x_is_ready(VL53L0X_DEV dev){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
	uint8_t NewDataReady = 0;

    status = VL53L0X_GetMeasurementDataReady(dev, &NewDataReady);

    if(status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
    }

    return NewDataReady ? DONE : NOT_DONE;
}

/** Returns 0 for invalid measurement **/
uint16_t vl53l0x_measure(VL53L0X_DEV dev, uint16_t preVal){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    VL53L0X_RangingMeasurementData_t meas;

    status = VL53L0X_GetRangingMeasurementData(dev, &meas);

    if(status != VL53L0X_ERROR_NONE || meas.RangeStatus || !dev->enable){
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


uint16_t calcXPosition(uint16_t l, uint16_t r){
    int lValid, rValid;

    lValid = !(l == 0 || l > MAX_RANGE);
    rValid = !(r == 0 || r > MAX_RANGE);

    /** Values are Valid **/
    if(lValid && rValid){
        return ((l + BOARD_HALF_X_SIZE) + (FIELD_X_SIZE - BOARD_HALF_X_SIZE - r)) >> 1;
    }

    if(rValid){
        return FIELD_X_SIZE - BOARD_HALF_X_SIZE - r;
    }

    if(lValid){
        return l + BOARD_HALF_X_SIZE;
    }

    /** Invalid Measurement by Both Sensors **/
    return 0;
}

uint16_t calcYPosition(uint16_t f, uint16_t b){
    int fValid, bValid;

    fValid = !(f == 0 || f > MAX_RANGE);
    bValid = !(b == 0 || b > MAX_RANGE);

    /** Values are Valid **/
    if(fValid && bValid){
        return ((b + BOARD_HALF_Y_SIZE) + (FIELD_Y_SIZE - BOARD_HALF_Y_SIZE - f)) >> 1;
    }

    if(fValid){
        return FIELD_Y_SIZE - BOARD_HALF_Y_SIZE - f;
    }

    if(bValid){
        return b + BOARD_HALF_Y_SIZE;
    }

    /**  0 Represents Invalid measurement by both sensors **/
    return 0;
}

#define BUF_SIZE 3

void positionBufferXClear(){
    int i = 0;
    for(i = 0; i < BUF_SIZE; i++){
        bufL[i] = 0;
        bufR[i] = 0;
    }
}

void positionBufferYClear(){
    int i = 0;
    for(i = 0; i < BUF_SIZE; i++){
        bufF[i] = 0;
        bufB[i] = 0;
    }
}

void enableDisableSensor(VL53L0X_ID id, int enable){
    switch(id){
        case VL53L0X_F:
            dev_f.enable = enable;
            break;
        case VL53L0X_B:
            dev_b.enable = enable;
            break;
        case VL53L0X_L:
            dev_l.enable = enable;
            break;
        case VL53L0X_R:
            dev_r.enable = enable;
            break;
    }
}

void updatePosition(position_t *pos){
    int chngXFlag = 0;
    int chngYFlag = 0;

    if(vl53l0x_is_ready(&dev_f) == DONE){
        bufF[(bufFIdx + 1) % BUF_SIZE] = vl53l0x_measure(&dev_f, bufF[bufFIdx]); 
        bufFIdx = (bufFIdx + 1) % BUF_SIZE;
        chngYFlag = 1;
    }

    if(vl53l0x_is_ready(&dev_b) == DONE){
        bufB[(bufBIdx + 1) % BUF_SIZE] = vl53l0x_measure(&dev_b, bufB[bufBIdx]); 
        bufBIdx = (bufBIdx + 1) % BUF_SIZE;
        chngYFlag = 1;
    }


    if(vl53l0x_is_ready(&dev_l) == DONE){
        bufL[(bufLIdx + 1) % BUF_SIZE] = vl53l0x_measure(&dev_l, bufL[bufLIdx]); 
        bufLIdx = (bufLIdx + 1) % BUF_SIZE;
        chngXFlag = 1;
    }

    if(vl53l0x_is_ready(&dev_r) == DONE){
        bufR[(bufRIdx + 1) % BUF_SIZE] = vl53l0x_measure(&dev_r, bufR[bufRIdx]); 
        bufRIdx = (bufRIdx + 1) % BUF_SIZE;
        chngXFlag = 1;
    }

    if(chngXFlag){
        // Store med of three as 4th element
        bufL[3] = medOfThree(bufL);
        bufR[3] = medOfThree(bufR);
        pos->x = calcXPosition(bufL[3], bufR[3]);
    }

    if(chngYFlag){
        // Store med of three as 4th element
        bufF[3] = medOfThree(bufF);
        bufB[3] = medOfThree(bufB);
        pos->y = calcYPosition(bufF[3], bufB[3]);
    }
}
