#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>

#define MAX_FLASH ((uint32_t*)0x80080000)
#define FLASH_DEBUG_START ((uint32_t*)0x80060000)

void flashcdw_issue_command(unsigned int command);
void flashcdw_set_flash_waitstate_and_readmode();
void flash_set_wait_state(unsigned int state);
void flash_write(uint8_t* data, uint8_t size);

#endif
