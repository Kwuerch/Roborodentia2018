#include <avr32/io.h>
#include "flash.h"

void flashcdw_issue_command(unsigned int command)
{
	avr32_flashc_fcmd_t cmd;

	cmd.cmd = command;
	cmd.key = AVR32_FLASHC_FCMD_KEY_KEY;

    while((AVR32_FLASHC.fsr & AVR32_FLASHC_FSR_FRDY));
	AVR32_FLASHC.FCMD = cmd;
    while((AVR32_FLASHC.fsr & AVR32_FLASHC_FSR_FRDY));

}

void flashcdw_set_flash_waitstate_and_readmode()
{
    AVR32_FLASHC.fcr = AVR32_FLASHC_FWS_MASK;
	flashcdw_issue_command(AVR32_FLASHC_FCMD_CMD_HSEN);
}

void flash_set_wait_state(unsigned int state){
    if(state){
        AVR32_FLASHC.fcr |= AVR32_FLASHC_FWS_MASK;
    }else{
        AVR32_FLASHC.fcr &= !AVR32_FLASHC_FWS_MASK;
    }
}

    /**
    while((AVR32_FLASHC.fsr & AVR32_FLASHC_FSR_FRDY));
    AVR32_GPIO.port[1].ovr = 0xA0;
    AVR32_FLASHC.fcmd = (AVR32_FLASHC_FCMD_KEY_KEY << AVR32_FLASHC_FCMD_KEY_OFFSET) |
                       AVR32_FLASHC_FCMD_CMD_HSEN;
                       **/
