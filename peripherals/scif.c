#include <avr32/io.h>
#include "scif.h"

void scif_enable_RCCR8(){
    SCIF_UNLOCK(AVR32_SCIF_RCCR8);
    AVR32_SCIF.rccr8 = AVR32_SCIF_RCCR8_RCOSC8_EN_MASK;
    while(!(AVR32_SCIF.rccr8 & AVR32_SCIF_RCCR8_RCOSC8_EN_MASK));
}

void scif_enable_RC120MCR(){
    SCIF_UNLOCK(AVR32_SCIF_RC120MCR);
    AVR32_SCIF.rc120mcr = AVR32_SCIF_RC120MCR_EN_MASK;
    while(!(AVR32_SCIF.rc120mcr & AVR32_SCIF_RC120MCR_EN_MASK));
}
