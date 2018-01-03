/**
  **********************************  STM8S  ***********************************
  * @�ļ���     �� bsp.c
  * @����       �� ����
  * @��汾     �� V2.2.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��12��14��
  * @ժҪ       �� �弶֧�ְ�Դ�ļ�
  ******************************************************************************/
/*----------------------------------------------------------------------------
  ������־:
  2017-04-10 V1.0.0:��ʼ�汾
  ----------------------------------------------------------------------------*/
/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "bsp.h"
#include "stm8s.h"



/************************************************
�������� �� CLK_Configuration
��    �� �� ʱ������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� ����
*************************************************/
void CLK_Configuration(void)
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //HSI = 16M (1��Ƶ)
  CLK_ClockSecuritySystemEnable();
}

/************************************************
�������� �� GPIO_Configuration
��    �� �� ����IO��������
��    �� �� ��
�� �� ֵ �� ��
��    �� �� ����
*************************************************/
void GPIO_Configuration(void)
{
  GPIO_DeInit(GPIOD);
  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(CODE_GPIO_PORT, (GPIO_Pin_TypeDef)CODE_GPIO_PIN,GPIO_MODE_IN_FL_NO_IT);

#if 0 
  /* ϵͳʱ����� */
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);
  CLK_CCOCmd(ENABLE);
#endif
}

/************************************************
�������� ��  EXTI_INIT
��    �� �� �ⲿ�жϴ�����ʽ����
��    �� �� ��
�� �� ֵ �� ��
��    �� �� ����
*************************************************/
void EXTI_INIT(void)
{
  EXTI_DeInit();
  GPIO_Init(CODE2_GPIO_PORT ,(GPIO_Pin_TypeDef)CODE2_GPIO_PIN ,GPIO_MODE_IN_FL_IT);    //CODE2�ⲿ�ж�����      
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD , EXTI_SENSITIVITY_RISE_ONLY);              //�½��ش���
}
/************************************************
�������� �� SYS_Initializes
��    �� �� �弶֧�ְ���ʼ��
��    �� �� ��
�� �� ֵ �� ��
��    �� �� ����
*************************************************/
void SYS_Initializes(void)
{
  CLK_Configuration();          //ʱ��
  //GPIO_Configuration();         //����
 // EXTI_INIT();                  //�ⲿ�ж�
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
