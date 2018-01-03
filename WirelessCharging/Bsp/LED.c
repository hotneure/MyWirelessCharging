/**
  **********************************  STM8S  ***********************************
  * @�ļ���     �� led.c
  * @����       �� ����
  * @��汾     �� V2.2.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��12��14��
  * @ժҪ       �� �弶֧�ְ�Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2017-12-14 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "LED.h"



INT16 count=0,value=30;

void Flash_Delay(UINT16 z)
{
	UINT16 x,y;
	for(x=z;x>0;x--)
		for(y=300;y>0;y--);
}

void Init_Clk(void)
{
	CLK_HSECmd(ENABLE);  
	CLK_ClockSwitchCmd(ENABLE);
	CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO,CLK_SOURCE_HSE,DISABLE,CLK_CURRENTCLOCKSTATE_ENABLE);
}

void set_LED_pwm(UINT16 frequency)
{
	TIM2_DeInit(); 
	TIM2_TimeBaseInit(TIM2_PRESCALER_1, frequency); 
	TIM2_OC2Init(TIM2_OCMODE_PWM2, TIM2_OUTPUTSTATE_ENABLE,0x00,TIM2_OCPOLARITY_LOW);
	TIM2_Cmd(ENABLE);                                      // TIM2 Main Output Enable
}

/************************************************
�������� �� CLK_Configuration
��    �� �� ʱ������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void CLK_Configuration(void)
{
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //HSI = 16M (1��Ƶ)
}

/************************************************
�������� �� GPIO_Configuration
��    �� �� ����IO��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void GPIO_Configuration(void)
{
  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST);

#if 0 
  /* ϵͳʱ����� */
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);
  CLK_CCOCmd(ENABLE);
#endif
}

/************************************************
�������� �� BSP_Initializes
��    �� �� �弶֧�ְ���ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� strongerHuang
*************************************************/
void BSP_Initializes(void)
{
  CLK_Configuration();
  GPIO_Configuration();
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
