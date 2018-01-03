#ifndef _ECTI_H
#define _EXTI_H

void exti_init(void);
void TIM4_Init(void);
void Save_Bit(unsigned char);
char data_check(int data);
char decode_header(int header);

#endif