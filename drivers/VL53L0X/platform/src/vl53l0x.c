#include <avr32/io.h>
#include "board.h"
#include "console.h"
#include "delay.h"
#include "vl53l0x.h"

VL53L0X_Error vl53l0x_set_address(VL53L0X_DEV dev, uint8_t addr);

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

void vl53l0x_init_dev(VL53L0X_SENSOR id){
    VL53L0X_Error status = VL53L0X_ERROR_NONE;
    uint8_t addr;
    uint8_t pin;

    switch(id){
        case VL53L0X_F:
            addr = VL53L0X_I2C_F;
            pin = VL53L0X_SD_F;
            break;
        case VL53L0X_B:
            addr = VL53L0X_I2C_B;
            pin = VL53L0X_SD_B;
            break;
        case VL53L0X_R:
            addr = VL53L0X_I2C_R;
            pin = VL53L0X_SD_R;
            break;
        case VL53L0X_L:
            addr = VL53L0X_I2C_L;
            pin = VL53L0X_SD_L;
            break;
        default:
            addr = VL53L0X_DEFAULT_ADDR;
            return;
    }

    VL53L0X_Dev_t dev;
    //dev.I2cDevAddr = addr;
    dev.I2cDevAddr = 0x29;
    
    AVR32_GPIO.port[VL53L0X_PORT].ovrc = pin;
    console_printf("Pin is %i\r\n", pin);
    status = vl53l0x_init(&dev);

    if(status != VL53L0X_ERROR_NONE){
        vl53l0x_print_error(status);
    }
}

void vl53l0x_init_all(){
    /** Initialize Pins & Shutdown **/
    init_vl53l0x_sd();

    //vl53l0x_init_dev(VL53L0X_F);
    //vl53l0x_init_dev(VL53L0X_B);
    vl53l0x_init_dev(VL53L0X_R);
    //vl53l0x_init_dev(VL53L0X_L);
}
