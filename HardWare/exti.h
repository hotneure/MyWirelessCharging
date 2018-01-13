#ifndef _ECTI_H
#define _EXTI_H

#include "stm8s.h"

#define CLEAR_VALUE     0x00

#define STARTCHECK      0x00
#define REC_DATA        0x01
#define BYTECHECK       0x02
#define STOPCHECK       0x03

#define INIT_STATE    0x00
#define TIME1         0x01
#define TIME2         0x02
#define TIME3         0x03

void exti_init(void);
void TIM4_Init(void);
void Save_Bit(unsigned char);

#endif