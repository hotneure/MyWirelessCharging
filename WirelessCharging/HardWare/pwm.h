#ifndef _PWM_H
#define _PWM_H
#include "stm8s.h"

extern uint32_t frequency;
void pwm_init(void);
void PWM_Close(void);
void PWM_Return(void);
void Set_PWM_Fre(void);
void PWM_CH2_Close(void);

#endif