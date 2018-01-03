/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp.h
  * @作者       ： strongerHuang
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年04月10日
  * @摘要       ： 板级支持包头文件
  ******************************************************************************/

/* 定义防止递归包含 ----------------------------------------------------------*/
#ifndef _BSP_H
#define _BSP_H

/* 包含的头文件 --------------------------------------------------------------*/
#include "stm8s.h"


/* 宏定义 --------------------------------------------------------------------*/
#define LED_GPIO_PORT             GPIOB
#define LED_GPIO_PIN              GPIO_PIN_4            //LED的控制脚

#define CODE2_GPIO_PORT           GPIOD
#define CODE2_GPIO_PIN            GPIO_PIN_6            //CODE2_DET 外部输入引脚

#define CODE_GPIO_PORT            GPIOD
#define CODE_GPIO_PIN             GPIO_PIN_3            //CODE_DET 外部输入引脚
#define CODE_DET_DATA             0x8


/* LED */
#define LED_ON                    GPIO_WriteHigh(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_OFF                   GPIO_WriteLow(LED_GPIO_PORT, LED_GPIO_PIN)
#define LED_REVERSE               GPIO_WriteReverse(LED_GPIO_PORT, LED_GPIO_PIN)

#define Timer4cnt                 1           //定时500us，2K周期去检测CODE_DET

/* 函数申明 ------------------------------------------------------------------*/
void CLK_Configuration(void);
void GPIO_Configuration(void);
void SYS_Initializes(void);
void EXTI_INIT(void);                   //外部中断配置
//void Flash_Delay(UINT16 z);             //呼吸灯延迟

BitStatus GPIO_ReadInPin(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin);       //读取某一个IO的值

#endif /* _BSP_H */

/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
