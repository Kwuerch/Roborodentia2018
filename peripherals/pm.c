#include <avr32/io.h>
#include "pm.h"

/**
 * pm_divide_clk
 *
 * divides the clk for the specified clk register,
 * either CPUSEL, HSBSEL, PBASEL, PBBSEL, or PBCSEL
 *
 * The clk will be divided by 2^sel with a max divide of 16 
 */
void pm_divide_clk(clk_bus_t clk_bus, clk_div_t clk_div){
    unsigned int reg_offset;

    switch(clk_bus){
        case CPU_CLK:
            reg_offset = AVR32_PM_CPUSEL;
            break;
        case PBA_CLK:
            reg_offset = AVR32_PM_PBASEL;
            break;
        case PBB_CLK:
            reg_offset = AVR32_PM_PBBSEL;
            break;
        case PBC_CLK:
            reg_offset = AVR32_PM_PBCSEL;
            break;
        default:
            return;
    }

    unsigned int clksel = CLK_SEL_DIV_MASK;
    if(clk_div != CLK_DIV_1){
        clksel |= (clk_div - 1) & CLK_SEL_SEL_MASK;
    }

    /** Offset is in bytes, must divide by 4 for integer arithmetic **/
    unsigned int* reg = ((unsigned int*)AVR32_PM_ADDRESS + (reg_offset>>2));
    PM_UNLOCK(reg_offset);
    *reg = clksel;
}
    
void pm_master_clk_sel(clk_t clock){
    while(!(AVR32_PM.sr & AVR32_PM_CKRDY_MASK));
    PM_UNLOCK(AVR32_PM_MCCTRL);
    AVR32_PM.mcctrl = clock;
}
