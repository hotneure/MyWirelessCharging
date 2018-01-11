#include "tim2.h"
#include "stm8s.h"

void CLK_Configuration(void)
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //HSI = 16M (1·ÖÆµ)
  CLK_ClockSecuritySystemEnable();
}

void TIM2_Init()
{ 
    TIM2_DeInit();
    TIM2_TimeBaseInit(TIM2_PRESCALER_128,37500);
}

void TIM2_Clr()
{
  TIM2->SR1 &= ~0x01; 
  TIM2->CNTRH = 0x00;
  TIM2->CNTRL = 0x00;
 // TIM2->CR1 &= ~0x01;
}
   
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
 }