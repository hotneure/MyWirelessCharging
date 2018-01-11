#include "adc.h"
#include "exti.h"
#include "qi.h"
#include "pid.h"
#include "tim2.h"
#include "pwm.h"
#include "stm8s.h"


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


void main(void)
{
  WPCQi_Phase = Ping_Phase;
  //disableInterrupts();
  CLK_Configuration();
  TIM2_Init();
  //ADC_Config();
  pwm_init();

  GPIOB->CR1|=1<<5;
  GPIOB->DDR|=1<<5;
  
  exti_init();                //�ⲿ�ж�ʵ�飬ʹ�õ�������PWM��ͻ
  TIM4_Init();
  enableInterrupts();
  //Set_PWM_Fre(155330);
  while(1)
  {
   WPC_QI();
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

