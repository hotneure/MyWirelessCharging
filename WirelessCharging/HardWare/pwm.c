#include "pwm.h"
#include "stm8s.h"

uint8_t DeadTime = 3;
uint32_t frequency;

void TIM1_DeInit(void)
{
  TIM1->CR1  = 0x00;
  TIM1->CR2  = 0x00;
  TIM1->SMCR = 0x00;
  TIM1->ETR  = 0x00;
  TIM1->IER  = 0x00;
  TIM1->SR2  = 0x00;
  // Disable channels 
  TIM1->CCER1 = 0x00;
  TIM1->CCER2 = 0x00;
  //Configure channels as inputs: it is necessary if lock level is equal to 2 or 3 
  TIM1->CCMR1 = 0x01;
  TIM1->CCMR2 = 0x01;
  TIM1->CCMR3 = 0x01;
  TIM1->CCMR4 = 0x01;
  //Then reset channel registers: it also works if lock level is equal to 2 or 3 
  TIM1->CCER1 = 0x00;
  TIM1->CCER2 = 0x00;
  TIM1->CCMR1 = 0x00;
  TIM1->CCMR2 = 0x00;
  TIM1->CCMR3 = 0x00;
  TIM1->CCMR4 = 0x00;
  TIM1->CNTRH = 0x00;
  TIM1->CNTRL = 0x00;
  TIM1->PSCRH = 0x00;
  TIM1->PSCRL = 0x00;
  TIM1->ARRH  = 0xFF;
  TIM1->ARRL  = 0xFF;
  TIM1->CCR1H = 0x00;
  TIM1->CCR1L = 0x00;
  TIM1->CCR2H = 0x00;
  TIM1->CCR2L = 0x00;
  TIM1->CCR3H = 0x00;
  TIM1->CCR3L = 0x00;
  TIM1->CCR4H = 0x00;
  TIM1->CCR4L = 0x00;
  TIM1->OISR  = 0x00;
  TIM1->EGR   = 0x01; // TIM1_EGR_UG 
  TIM1->DTR   = 0x00;
  TIM1->BKR   = 0x00;
  TIM1->RCR   = 0x00;
  TIM1->SR1   = 0x00;
}


/*
PWM初始化：
CH1和CH2互补输出
TIM1_CH1 有效高电平
TIM1_CH1N 有效高电平
TIM1_CH2 有效高电平
TIM1_CH2N 有效高电平
*/
void pwm_init(void)
{
    //配置GPIO为输出，高速，推挽
    GPIOC->CR2 &= (uint8_t)(~0xD8); 
    GPIOC->DDR |= (uint8_t)0xD8;
    GPIOC->CR1 |= (uint8_t)0xD8;
    GPIOC->CR2 |= (uint8_t)0xD8;
    
    CLK->PCKENR1 |= 0x80;
 
    TIM1_DeInit();
    
    TIM1->ARRH = (uint8_t)(0x5C >> 8);
    TIM1->ARRL = (uint8_t)(0x5C);
    TIM1->CCER1 |= (uint8_t)0x05;
    TIM1->CCMR1 = (uint8_t)((uint8_t)(TIM1->CCMR1 & (uint8_t)(~0x70))| (uint8_t)0x60);
    TIM1->CCR1H = (uint8_t)(0x2E >> 8);
    TIM1->CCR1L = (uint8_t)(0x2E);
    TIM1->CCER1 |= (uint8_t)(0x50);
    TIM1->CCMR2 = (uint8_t)((uint8_t)(TIM1->CCMR2 & (uint8_t)(~0x70)) |(uint8_t)0x60);
    TIM1->CCR2H = (uint8_t)(0x2E >> 8);
    TIM1->CCR2L = (uint8_t)(0x2E);
    TIM1->DTR = (uint8_t)(DeadTime);
    TIM1->BKR  =  (uint8_t)(0x05);
    TIM1->OISR |= 0x04;
    TIM1->IER |= 0x01;
    TIM1->CR1 |=0x01;
    TIM1->BKR |= 0x80;
}

//开TIM1更新事件中断
void Set_PWM_Fre()
{
    TIM1->IER |= 0x01;   
}
    
//单独关CH2
void PWM_CH2_Close()
{
   TIM1->CCMR2 &= ~(7<<4);
   TIM1->CCMR2 |= 0x40;
   TIM1->CCER1 |= 1<<5;
}

//单独开CH2
void PWM_CH2_Return()
{
    TIM1->CCMR2 &= ~(7<<4);
    TIM1->CCMR2 |= 0x60;
    TIM1->CCER1 &= ~(1<<5);
} 

//四路PWM全部关闭
void PWM_Close()
{
    TIM1->BKR &= ~0x80;
}
//四路PWM恢复
void PWM_Return()
{
    
    frequency = 175000;
    Set_PWM_Fre();
    PWM_CH2_Return();
    TIM1->BKR |= 0x80;
}


//中断处理函数，重新设置PWM频率
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11)
{
    unsigned int reg;
    
    reg = 16000000/frequency;
    reg -= 1;
    TIM1->ARRH = (unsigned char)(reg >> 8);
    TIM1->ARRL = (unsigned char)(reg);
    reg >>= 1;
    TIM1->CCR1H = (unsigned char)(reg >> 8);
    TIM1->CCR1L = (unsigned char)(reg);
    
    TIM1->CCR2H = (unsigned char)(reg >> 8);
    TIM1->CCR2L = (unsigned char)(reg);

    TIM1->IER &= 0xFE;
    TIM1->SR1 &= 0xFE;
}
