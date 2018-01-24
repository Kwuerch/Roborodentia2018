#ifndef FLASH_H
#define FLASH_H

void flashcdw_issue_command(unsigned int command);
void flashcdw_set_flash_waitstate_and_readmode();
void flash_set_wait_state(unsigned int state);

#endif
