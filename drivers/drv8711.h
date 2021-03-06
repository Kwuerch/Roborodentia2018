#ifndef DRV8711_H_
#define DRV8711_H_

#include <stdint.h>
#include "spi_master.h"

#define DRV8711_ADDR_OFFSET 12
#define DRV8711_WRITE_ADDR_MASK 0x7000
#define DRV8711_WRITE_DATA_MASK 0x0FFF
#define DRV8711_WRITE_MASK 0x8000
#define DRV8711_READ_MASK 0x8000

#define DRV8711_CTRL_ADDRESS 0x00
#define DRV8711_CTRL_ENBL_MASK 0x001
#define DRV8711_CTRL_ENBL_OFFSET 0
#define DRV8711_CTRL_RDIR_MASK 0x002
#define DRV8711_CTRL_RDIR_OFFSET 1
#define DRV8711_CTRL_RSTEP_MASK 0x004
#define DRV8711_CTRL_RSTEP_OFFSET 2 
#define DRV8711_CTRL_MODE_MASK 0x078
#define DRV8711_CTRL_MODE_OFFSET 3 
#define DRV8711_CTRL_EXSTALL_MASK 0x080 
#define DRV8711_CTRL_EXSTALL_OFFSET 7
#define DRV8711_CTRL_ISGAIN_MASK 0x300 
#define DRV8711_CTRL_ISGAIN_OFFSET 8
#define DRV8711_CTRL_DTIME_MASK 0xC00 
#define DRV8711_CTRL_DTIME_OFFSET 10 

#define DRV8711_TORQUE_ADDRESS 0x01
#define DRV8711_TORQUE_TORQUE_MASK 0x0FF
#define DRV8711_TORQUE_TORQUE_OFFSET 0
#define DRV8711_TORQUE_SMPLTH_MASK 0x700
#define DRV8711_TORQUE_SMPLTH_OFFSET 8

#define DRV8711_OFF_ADDRESS 0x02
#define DRV8711_OFF_TOFF_MASK 0x0FF
#define DRV8711_OFF_TOFF_OFFSET 0
#define DRV8711_OFF_PWMMODE_MASK 0x100
#define DRV8711_OFF_PWMMODE_OFFSET 8

#define DRV8711_BLANK_ADDRESS 0x03
#define DRV8711_BLANK_TBLANK_MASK 0x0FF
#define DRV8711_BLANK_TBLANK_OFFSET 0
#define DRV8711_BLANK_ABT_MASK 0x100
#define DRV8711_BLANK_ABT_OFFSET 8

#define DRV8711_DECAY_ADDRESS 0x04
#define DRV8711_DECAY_TDECAY_MASK 0x0FF
#define DRV8711_DECAY_TDECAY_OFFSET 0
#define DRV8711_DECAY_DECMOD_MASK 0x700
#define DRV8711_DECAY_DECMOD_OFFSET 8

#define DRV8711_STALL_ADDRESS 0x05
#define DRV8711_STALL_SDTHR_MASK 0x0FF
#define DRV8711_STALL_SDTHR_OFFSET 0
#define DRV8711_STALL_SDCNT_MASK 0x300
#define DRV8711_STALL_SDCNT_OFFSET 8
#define DRV8711_STALL_VDIV_MASK 0xC00
#define DRV8711_STALL_VDIV_OFFSET 10

#define DRV8711_DRIVE_ADDRESS 0x06
#define DRV8711_DRIVE_OCPTH_MASK 0x003
#define DRV8711_DRIVE_OCPTH_OFFSET 0
#define DRV8711_DRIVE_OCPDEG_MASK 0x00C
#define DRV8711_DRIVE_OCPDEG_OFFSET 2
#define DRV8711_DRIVE_TDRIVEN_MASK 0x030
#define DRV8711_DRIVE_TDRIVEN_OFFSET 4 
#define DRV8711_DRIVE_TDRIVEP_MASK 0x0C0
#define DRV8711_DRIVE_TDRIVEP_OFFSET 6 
#define DRV8711_DRIVE_IDRIVEN_MASK 0x300
#define DRV8711_DRIVE_IDRIVEN_OFFSET 8 
#define DRV8711_DRIVE_IDRIVEP_MASK 0xC00
#define DRV8711_DRIVE_IDRIVEP_OFFSET 10 

#define DRV8711_STATUS_ADDRESS 0x07
#define DRV8711_STATUS_OTS_MASK 0x001
#define DRV8711_STATUS_OTS_OFFSET 0
#define DRV8711_STATUS_AOCP_MASK 0x002
#define DRV8711_STATUS_AOCP_OFFSET 1
#define DRV8711_STATUS_BOCP_MASK 0x004
#define DRV8711_STATUS_BOCP_OFFSET 2
#define DRV8711_STATUS_APDF_MASK 0x008
#define DRV8711_STATUS_APDF_OFFSET 3
#define DRV8711_STATUS_BPDF_MASK 0x010
#define DRV8711_STATUS_BPDF_OFFSET 4
#define DRV8711_STATUS_UVLO_MASK 0x020
#define DRV8711_STATUS_UVLO_OFFSET 5
#define DRV8711_STATUS_STD_MASK 0x040
#define DRV8711_STATUS_STD_OFFSET 6
#define DRV8711_STATUS_STDLAT_MASK 0x080
#define DRV8711_STATUS_STDLAT_OFFSET 7

/** Ctrl Register **/
typedef enum DRV8711_ENBL{
    ENBL_DISABLE = 0,
    ENBL_ENABLE
}DRV8711_ENBL;

typedef enum DRV8711_RDIR{
    RDIR_DIR_PIN = 0,
    RDIR_INV_DIR_PIN
}DRV8711_RDIR;

typedef enum DRV8711_RSTEP{
    RSTEP_NO_ACT = 0,
    RSTEP_INDEXER_ADV
}DRV8711_RSTEP;

typedef enum DRV8711_MODE{
    STEP_1_1= 0,
    STEP_1_2,
    STEP_1_4,
    STEP_1_8,
    STEP_1_16,
    STEP_1_32,
    STEP_1_64,
    STEP_1_128,
    STEP_1_256
}DRV8711_MODE;

typedef enum DRV8711_EXSTALL{
    INTERNAL_DETECT = 0,
    EXTERNAL_DETECT
}DRV8711_EXSTALL;

typedef enum DRV8711_ISGAIN{
    GAIN_5 = 0,
    GAIN_10,
    GAIN_20,
    GAIN_40
}DRV8711_ISGAIN;

typedef enum DRV8711_DTIME{
   DTIME_50_US = 0,
   DTIME_450_NS,
   DTIME_650_NS,
   DTIME_850_NS
}DRV8711_DTIME;

/** Torque Register **/
typedef enum DRV8711_SMPLTH{
   SMPLTH_50_US = 0,
   SMPLTH_100_US,
   SMPLTH_200_US,
   SMPLTH_300_US,
   SMPLTH_400_US,
   SMPLTH_600_US,
   SMPLTH_800_US,
   SMPLTH_1000_US
}DRV8711_SMPLTH;

/** Off Register **/
typedef enum DRV8711_PWMMODE{
   PWMMODE_INT_INDEXER = 0,
   PWMMODE_EXT_INDEXER
}DRV8711_PWMMODE;

/** Blank Register **/
typedef enum DRV8711_ABT{
   ABT_DISABLE = 0,
   ABT_ENABLE
}DRV8711_ABT;

/** Decay Register **/
typedef enum DRV8711_DECMOD{
   DECMOD_0 = 0,
   DECMOD_1,
   DECMOD_2,
   DECMOD_3,
   DECMOD_4,
   DECMOD_5,
   DECMOD_6,
   DECMOD_7
}DRV8711_DECMOD;

/** Stall Register **/
typedef enum DRV8711_SDCNT{
   SDCNT_FIRST_STEP = 0,
   SDCNT_SECOND_STEP,
   SDCNT_FOURTH_STEP,
   SDCNT_EIGTH_STEP
}DRV8711_SDCNT;

typedef enum DRV8711_VDIV{
   VDIV_DIV_32 = 0,
   VDIV_DIV_16,
   VDIV_DIV_8,
   VDIV_DIV_4
}DRV8711_VDIV;

/** Drive Register **/
typedef enum DRV8711_OCPTH{
   OCPTH_250_MV = 0,
   OCPTH_500_MV,
   OCPTH_750_MV,
   OCPTH_1000_MV
}DRV8711_OCPTH;

typedef enum DRV8711_OCPDEG{
   OCPDEG_1_US = 0,
   OCPDEG_2_US,
   OCPDEG_4_US,
   OCPDEG_8_US
}DRV8711_OCPDEG;

typedef enum DRV8711_TDRIVEN{
   TDRIVEN_250_NS = 0,
   TDRIVEN_500_NS,
   TDRIVEN_1_US,
   TDRIVEN_2_US
}DRV8711_TDRIVEN;

typedef enum DRV8711_TDRIVEP{
   TDRIVEP_250_NS = 0,
   TDRIVEP_500_NS,
   TDRIVEP_1_US,
   TDRIVEP_2_US
}DRV8711_TDRIVEP;

typedef enum DRV8711_IDRIVEN{
   IDRIVEN_100_MA = 0,
   IDRIVEN_200_MA,
   IDRIVEN_300_MA,
   IDRIVEN_400_MA
}DRV8711_IDRIVEN;

typedef enum DRV8711_IDRIVEP{
   IDRIVEP_50_MA = 0,
   IDRIVEP_100_MA,
   IDRIVEP_150_MA,
   IDRIVEP_200_MA
}DRV8711_IDRIVEP;

uint16_t drv8711_read_reg(uint8_t csid, uint8_t reg);
void drv8711_write_reg(uint8_t csid, uint8_t reg, uint16_t data);
void drv8711_init(uint8_t csid);
uint16_t drv8711_read_status(uint8_t csid);
void drv8711_print_registers(uint8_t csid);
void drv8711_write_torque(uint8_t csid, uint8_t torque, DRV8711_SMPLTH smplt);

#endif
