#include <avr32/io.h>
#include "btn.h"

int btn_is_pressed(uint32_t btnMask){
    btnMask &= BTN_1 | BTN_2;
    return !(AVR32_GPIO.port[BTN_PORT].pvr & btnMask);
}
