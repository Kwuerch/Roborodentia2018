#include "vl53l0x.h"
#include "console.h"

VL53L0X_Error vl53l0x_init(VL53L0X_DEV dev){
	VL53L0X_Error Status = VL53L0X_ERROR_NONE;

    uint32_t refSpadCount;
    uint8_t isAperatureSpads;
    uint8_t vhvSettings;
    uint8_t phaseCal;

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
