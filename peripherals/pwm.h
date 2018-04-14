#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

#define PWM_CPRD_50HZ  320664
#define PWM_CDTY_1_1MS 17637
#define PWM_CDTY_2MS   32066


#define PWM_CPRD_FAN_25KHZ 2565 
#define PWM_CDTY_FAN_60PD  1539

#define PWM_CPRD_QTRHZ 1002075
#define PWM_CNT_1S     250519
#define PWM_CNT_2S     501038
#define PWM_CNT_HALFS  125260
#define PWM_CNT_100MS  25052
#define PWM_CNT_200MS  50104
#define PWM_CNT_1MS    251

void PWM_INIT();
void pwmStartCount();
uint32_t getCount();

#endif
