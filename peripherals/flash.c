#include <avr32/io.h>
#include <stdint.h>
#include "console.h"
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

uint8_t buf[4];
uint8_t bufIdx = 0;
uint32_t* flashAddr = FLASH_DEBUG_START;
uint8_t wordNum = 0;

/** TODO - must reset the write buffer after every write to flash **/

void write_word(uint32_t data){
    if(flashAddr < MAX_FLASH && flashAddr >= FLASH_DEBUG_START){
        *flashAddr = data;
        flashAddr++;
        wordNum++;

        if(wordNum == 128){
            flashcdw_issue_command(AVR32_FLASHC_FCMD_CMD_QPR);
            if(!(AVR32_FLASHC.fsr & AVR32_FLASHC_FSR_QPRR)){
                flashcdw_issue_command(AVR32_FLASHC_FCMD_CMD_EP);
            }

            //write flash;

            flashcdw_issue_command(AVR32_FLASHC_FCMD_CMD_WP);
            console_printf("Write %u\r\n", AVR32_FLASHC.fsr);

            flashcdw_issue_command(AVR32_FLASHC_FCMD_CMD_CPB);
            console_printf("Page Buffer %u\r\n", AVR32_FLASHC.fsr);

            wordNum = 0;
        }
    }

}

#include "console.h"
void flash_write(uint8_t* data, uint8_t size){
    uint32_t word;
    int i = 0, j = 0;
    int writeBuf = 0;

    /** Write first to bufidx to word align **/
    while(i < size && bufIdx != 0){
        buf[bufIdx] = data[i++];
        bufIdx = (bufIdx + 1) % 4;

        if(bufIdx == 0){
            writeBuf = 1;
            break;
        }
    }

    if(writeBuf){
        /** write buf word to flash buffer **/
        writeBuf = 0;
        //console_printf("writing word %c %c %c %c", (char)buf[3], (char)buf[2], (char)buf[1], (char)buf[0]);
        write_word((uint32_t)buf[0]);
    }

    /** Write 4 Byte Words **/
    for(; i < size; i+=4){
        word = 0;
        for(j = 0; j < 4; j++){
            word <<= 8;
            word |= data[i + j];
        }
        //console_printf("writing word %c %c %c %c", (char)((word >> 24) & 0xFF), (char)((word >> 16) & 0xFF), (char)((word >> 8) & 0xFF), (char)(word & 0xFF));
        write_word(word);
    }

    /** Write rest to buffer **/
    /** I is 4 past possible data end **/
    for(i = (i - 3); i < size; i++){
        buf[bufIdx] = data[i];
        bufIdx = (bufIdx + 1) % 4;

        if(bufIdx == 0){
            writeBuf = 1;
        }
    }

    if(writeBuf){
        //console_printf("writing buf %c %c %c %c", (char)buf[3], (char)buf[2], (char)buf[1], (char)buf[0]);
        write_word((uint32_t)buf[0]);
    }
}
