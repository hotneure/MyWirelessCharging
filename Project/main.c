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

void GPIO_Config(void)
{
  GPIOB->CR1|=1<<5;
  GPIOB->DDR|=1<<5;

  GPIOA->CR1|=1<<2;
  GPIOA->DDR|=1<<2;  

  GPIOB->CR1|=1<<4;
}


void main(void)
{
  WPCQi_Phase = Ping_Phase;
  CLK_Configuration();
  GPIO_Config();
  TIM2_Init();
  ADC_Config();
  pwm_init();
//  GPIOA->ODR|=1<<2;          //��Դ���
  exti_init();                //�ⲿ�ж�ʵ�飬ʹ�õ�������PWM��ͻ
  TIM4_Init();
  enableInterrupts();

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

