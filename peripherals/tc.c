#include <avr32/io.h>

void tc_init(){
    // GENERATE SIGNALS: Waveform operating mode.
    // DRV8711 STEP A
    AVR32_TC0.channel[0].cmr = AVR32_TC_CMR0_ACPA_SET << AVR32_TC_ACPA_OFFSET |
                               AVR32_TC_CMR0_ACPC_CLEAR << AVR32_TC_ACPC_OFFSET |
                               AVR32_TC_CMR0_WAVE_MASK |
                               AVR32_TC_CMR0_WAVSEL_UP_AUTO << AVR32_TC_CMR0_WAVSEL_OFFSET |
                               AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK2 << AVR32_TC_CMR0_TCCLKS_OFFSET;

    AVR32_TC0.channel[0].ra = 0;
    AVR32_TC0.channel[0].rc = 0;
    AVR32_TC0.channel[0].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;

    // GENERATE SIGNALS: Waveform operating mode.
    // DRV8711 STEP B
    AVR32_TC0.channel[1].cmr = AVR32_TC_CMR0_ACPA_SET << AVR32_TC_ACPA_OFFSET |
                               AVR32_TC_CMR0_ACPC_CLEAR << AVR32_TC_ACPC_OFFSET |
                               AVR32_TC_CMR0_WAVE_MASK |
                               AVR32_TC_CMR0_WAVSEL_UP_AUTO << AVR32_TC_CMR0_WAVSEL_OFFSET |
                               AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK2 << AVR32_TC_CMR0_TCCLKS_OFFSET;

    AVR32_TC0.channel[1].ra = 0;
    AVR32_TC0.channel[1].rc = 0;
    AVR32_TC0.channel[1].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;

    // GENERATE SIGNALS: Waveform operating mode.
    // DRV8711 STEP C
    AVR32_TC1.channel[0].cmr = AVR32_TC_CMR0_ACPA_SET << AVR32_TC_ACPA_OFFSET |
                               AVR32_TC_CMR0_ACPC_CLEAR << AVR32_TC_ACPC_OFFSET |
                               AVR32_TC_CMR0_WAVE_MASK |
                               AVR32_TC_CMR0_WAVSEL_UP_AUTO << AVR32_TC_CMR0_WAVSEL_OFFSET |
                               AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK2 << AVR32_TC_CMR0_TCCLKS_OFFSET;

    AVR32_TC1.channel[0].ra = 0;
    AVR32_TC1.channel[0].rc = 0;
    AVR32_TC1.channel[0].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;

    // GENERATE SIGNALS: Waveform operating mode.
    // DRV8711 STEP D
    AVR32_TC1.channel[1].cmr = AVR32_TC_CMR0_ACPA_SET << AVR32_TC_ACPA_OFFSET |
                               AVR32_TC_CMR0_ACPC_CLEAR << AVR32_TC_ACPC_OFFSET |
                               AVR32_TC_CMR0_WAVE_MASK |
                               AVR32_TC_CMR0_WAVSEL_UP_AUTO << AVR32_TC_CMR0_WAVSEL_OFFSET |
                               AVR32_TC_CMR0_TCCLKS_TIMER_CLOCK2 << AVR32_TC_CMR0_TCCLKS_OFFSET;

    AVR32_TC1.channel[1].ra = 0;
    AVR32_TC1.channel[1].rc = 0;
    AVR32_TC1.channel[1].ccr = AVR32_TC_SWTRG_MASK | AVR32_TC_CLKEN_MASK;
}
