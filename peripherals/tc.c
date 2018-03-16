#include <avr32/io.h>
#include "gpio.h"
#include "board.h"

void TC0_INIT(){
    /** Initialize timers connected to Step A and Step B in waveform mode **/
    gpioInit gi;

    /** DRV8711 STEP A and B **/
    gi.pins = DRV8711_STEP_PIN_A | DRV8711_STEP_PIN_B;
    gi.type = GPIO_AF;
    gi.port = DRV8711_STEPAB_PORT;
    gi.af = FUNC_A;
    gpio_init(&gi);

    /** STEP A **/
    AVR32_TC0.channel[0].cmr = AVR32_TC_CMR0_ACPA_SET << AVR32_TC_ACPA_OFFSET |
                               AVR32_TC_CMR0_ACPC_CLEAR << AVR32_TC_ACPC_OFFSET |
                               AVR32_TC_CMR0_WAVE_MASK |
                               AVR32_TC_CMR0_WAVSEL_UP_AUTO << AVR32_TC_CMR0_WAVSEL_OFFSET |
                               AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK2 << AVR32_TC_CMR0_TCCLKS_OFFSET;

    AVR32_TC0.channel[0].ra = 0;
    AVR32_TC0.channel[0].rc = 0;
    AVR32_TC0.channel[0].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;

    /** STEP B **/
    AVR32_TC0.channel[1].cmr = AVR32_TC_CMR0_ACPA_SET << AVR32_TC_ACPA_OFFSET |
                               AVR32_TC_CMR0_ACPC_CLEAR << AVR32_TC_ACPC_OFFSET |
                               AVR32_TC_CMR0_WAVE_MASK |
                               AVR32_TC_CMR0_WAVSEL_UP_AUTO << AVR32_TC_CMR0_WAVSEL_OFFSET |
                               AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK2 << AVR32_TC_CMR0_TCCLKS_OFFSET;

    AVR32_TC0.channel[1].ra = 0;
    AVR32_TC0.channel[1].rc = 0;
    AVR32_TC0.channel[1].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;

}

void TC1_INIT(){
    /** Initialize timers connected to Step C and Step D in waveform mode **/
    gpioInit gi;

    /** DRV8711 STEP C **/
    gi.pins = DRV8711_STEP_PIN_C;
    gi.type = GPIO_AF;
    gi.port = DRV8711_STEPC_PORT;
    gi.af = FUNC_D ;
    gpio_init(&gi);

    /** DRV8711 STEP D **/
    gi.pins = DRV8711_STEP_PIN_D;
    gi.type = GPIO_AF;
    gi.port = DRV8711_STEPD_PORT;
    gi.af = FUNC_B ;
    gpio_init(&gi);

    /** Step C **/
    AVR32_TC1.channel[0].cmr = AVR32_TC_CMR0_ACPA_SET << AVR32_TC_ACPA_OFFSET |
                               AVR32_TC_CMR0_ACPC_CLEAR << AVR32_TC_ACPC_OFFSET |
                               AVR32_TC_CMR0_WAVE_MASK |
                               AVR32_TC_CMR0_WAVSEL_UP_AUTO << AVR32_TC_CMR0_WAVSEL_OFFSET |
                               AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK2 << AVR32_TC_CMR0_TCCLKS_OFFSET;

    AVR32_TC1.channel[0].ra = 0;
    AVR32_TC1.channel[0].rc = 0;
    AVR32_TC1.channel[0].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;

    /** Step D **/
    AVR32_TC1.channel[1].cmr = AVR32_TC_CMR0_ACPA_SET << AVR32_TC_ACPA_OFFSET |
                               AVR32_TC_CMR0_ACPC_CLEAR << AVR32_TC_ACPC_OFFSET |
                               AVR32_TC_CMR0_WAVE_MASK |
                               AVR32_TC_CMR0_WAVSEL_UP_AUTO << AVR32_TC_CMR0_WAVSEL_OFFSET |
                               AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK2 << AVR32_TC_CMR0_TCCLKS_OFFSET;

    AVR32_TC1.channel[1].ra = 0;
    AVR32_TC1.channel[1].rc = 0;
    AVR32_TC1.channel[1].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;
}
