#include "bsp.h"
#include "bsp_timer.h"
#include "pwm.h"
#include "adc.h"
#include "exti.h"
#include "qi.h"
#include "pid.h"
#include "tim2.h"

void System_Initializes(void)
{
  SYS_Initializes();       
 }

/*
    ��ʱ10ms����
*/
void delay(void)
{
    long i;
    for(i=0;i<5000;i++)
    { 
    }
}

unsigned int a=1;

void main(void)
{
  //WPCQi_Phase = Ping_Phase;
  System_Initializes();
  //TIM2_Init();
  //GPIO_Init(GPIOC,   GPIO_PIN_3,   GPIO_MODE_OUT_PP_HIGH_FAST);   
  //GPIOC->ODR |= 1<<3;
  //ADC_Config();
  //pwm_init();
  GPIOC->CR1 |= 0x40;
  GPIOC->DDR |= 0x40;
  GPIOC->CR2 |= 0x40;
  exti_init();                //�ⲿ�ж�ʵ�飬ʹ�õ�������PWM��ͻ
  TIM4_Init();
  enableInterrupts();
  while(1)
  {
    /*
    PWM_Return();
    TIM2_Cmd(ENABLE);
    while(((TIM2->CNTRH)<<8|(TIM2->CNTRL))!=12500);
    TIM2->CNTRH =0;
    TIM2->CNTRL =0;
    PWM_Close();
    while(!(TIM2->SR1 & 0x01));
    TIM2->SR1 &= ~1;
    TIM2->CNTRH =0;
    TIM2->CNTRL =0;
    TIM2_Cmd(DISABLE);
    
    */
    
   // GPIO_Convert();
    //delay();
  }
}


#ifdef USE_FULL_ASSERT                           //����(�ο��ٷ�)

void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

