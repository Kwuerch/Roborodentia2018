#ifndef SCIF_H
#define SCIF_H

#define SCIF_UNLOCK(reg) (AVR32_SCIF.unlock = (AVR32_SCIF_UNLOCK_KEY_VALUE << AVR32_SCIF_UNLOCK_KEY_OFFSET)|(reg))

void scif_enable_RCCR8();
void scif_enable_RC120MCR();
void scif_enable_OSC32K();

#endif
