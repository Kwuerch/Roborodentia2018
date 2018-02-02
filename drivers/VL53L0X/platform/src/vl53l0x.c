#include "vl53l0x.h"

void vl53l0x_init(VL53L0X_DEV dev){

    VL53L0X_DataInit(dev);
    VL53L0X_StaticInit(dev);

    /**
     * VL53L0X_PerformRefSpadMeasurement(pMyDevice, &refSpadCount, &isAperatureSpads)
     * VL53L0X_PerformRefCalibration(pMyDevice, &vhvSettings, &phaseCal)
     *
     * VL53L0X_SetDeviceMode
     * VL53L0X_Set_offsetCalbirationDataMicroMeter();
     */


    VL53L0X_SetDeviceMode(dev, VL53L0X_DEVICEMODE_SINGLE_RANGING);
}

void vl53l0x_init_longrange(VL53L0X_DEV dev){
    VL53L0X_Error status;
    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, (FixPoint1616_t)(0.1*65536));

    status = VL53L0X_SetLimitCheckValue(dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, (FixPoint1616_t)(60*65536));

    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds(dev, 33000);

    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, 18);

    status = VL53L0X_SetVcselPulsePeriod(dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, 14);
}

