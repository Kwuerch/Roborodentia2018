#ifndef PM_H
#define PM_H

typedef enum{
    CLK_DIV_1 = 0,
    CLK_DIV_2,
    CLK_DIV_4,
    CLK_DIV_8,
    CLK_DIV_16,
}clk_div_t;

typedef enum{
    CPU_CLK = 0,
    PBA_CLK,
    PBB_CLK,
    PBC_CLK
}clk_bus_t;

typedef enum{
    RCSYS = 0,
    OSC0,
    OSC1,
    PLL0,
    PLL1,
    RC8M,
    /** MCSEL 6 is reserved **/
    RC120M = 7
}clk_t;

#define PM_UNLOCK(reg) (AVR32_PM.unlock = (AVR32_PM_UNLOCK_KEY_VALUE << AVR32_PM_UNLOCK_KEY_OFFSET)|(reg))

#define CLK_SEL_DIV_MASK 0x00000080
#define CLK_SEL_SEL_MASK 0x00000003

void pm_divide_clk(clk_bus_t clk_bus, clk_div_t clk_div);
void pm_master_clk_sel(clk_t clock);

#endif


