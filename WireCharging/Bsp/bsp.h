/**
  **********************************  STM8S  ***********************************
  * @�ļ���     �� bsp.h
  * @����       �� strongerHuang
  * @��汾     �� V2.2.0
  * @�ļ��汾   �� V1.0.0
  * @����       �� 2017��04��10��
  * @ժҪ       �� �弶֧�ְ�ͷ�ļ�
  ******************************************************************************/

/* �����ֹ�ݹ���� ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* ������ͷ�ļ� --------------------------------------------------------------*/
#include "stm8s.h"


/* �궨�� --------------------------------------------------------------------*/
#define LED_GPIO_PORT             GPIOB
#define LED_GPIO_PIN              GPIO_PIN_4            //LED�Ŀ��ƽ�

#define CODE2_GPIO_PORT           GPIOD
#define CODE2_GPIO_PIN            GPIO_PIN_6            //CODE2_DET �ⲿ��������

#define CODE_GPIO_PORT            GPIOD
#define CODE_GPIO_PIN             GPIO_PIN_3            //CODE_DET �ⲿ��������
#define CODE_DET_DATA             0x8


/* LED */
#define LED_ON                    GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_OFF                   GPIO_WriteLow(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_REVERSE               GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN)

#define Timer4cnt                 1           //��ʱ500us��2K����ȥ���CODE_DET

/* �������� ------------------------------------------------------------------*/
void CLK_Configuration(void);
void GPIO_Configuration(void);
void SYS_Initializes(void);
void EXTI_INIT(void);                   //�ⲿ�ж�����
//void Flash_Delay(UINT16 z);             //�������ӳ�

BitStatus GPIO_ReadInPin(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin);       //��ȡĳһ��IO��ֵ

#endif /* _BSP_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
