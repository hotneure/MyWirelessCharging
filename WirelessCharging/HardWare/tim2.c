#include "tim2.h"
#include "stm8s.h"

void TIM2_Init()
{
    CLK->PCKENR1 |= 0x20;
 
    TIM2_DeInit();
    GPIO_Init(GPIOC,   GPIO_PIN_3,   GPIO_MODE_OUT_PP_HIGH_FAST);  
    
    TIM2_TimeBaseInit(TIM2_PRESCALER_16,500);
    TIM2_PrescalerConfig(TIM2_PRESCALER_16, TIM2_PSCRELOADMODE_IMMEDIATE);
}

void GPIO_Convert()
{
  char i;
  for(i=0;i<13;i++){
     TIM2_Cmd(ENABLE);
     GPIOC->ODR &= ~(1<<3);
     while(TIM2->CNTRL!=250){};
     GPIOC->ODR |= 1<<3;
     while(!(TIM2->SR1 & 0x01));
     TIM2->SR1 &= ~1;
     TIM2->CNTRH =0;
     TIM2->CNTRL =0;
     TIM2_Cmd(DISABLE);
  }

     TIM2_Cmd(ENABLE);
     GPIOC->ODR &= ~(1<<3);
     while(TIM2->CNTRL!=250){};
     TIM2->CNTRH =0;
     TIM2->CNTRL =0;
     TIM2_Cmd(DISABLE);
  
     for(i=0;i<11;i++){
     TIM2_Cmd(ENABLE);
     GPIOC->ODR &= ~(1<<3);
     while(TIM2->CNTRL!=250){};
     GPIOC->ODR |= 1<<3;
     while(!(TIM2->SR1 & 0x01));
     TIM2->SR1 &= ~1;
     TIM2->CNTRH =0;
     TIM2->CNTRL =0;
     TIM2_Cmd(DISABLE);
    }
  
     TIM2_Cmd(ENABLE);
     while(TIM2->CNTRL!=250){};
     TIM2->CNTRH =0;
     TIM2->CNTRL =0;
     TIM2_Cmd(DISABLE);
     
     for(i=0;i<11;i++){
     TIM2_Cmd(ENABLE);
     GPIOC->ODR &= ~(1<<3);
     while(TIM2->CNTRL!=250){};
     GPIOC->ODR |= 1<<3;
     while(!(TIM2->SR1 & 0x01));
     TIM2->SR1 &= ~1;
     TIM2->CNTRH =0;
     TIM2->CNTRL =0;
     TIM2_Cmd(DISABLE);
  }
 }
   