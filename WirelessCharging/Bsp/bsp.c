/**
  **********************************  STM8S  ***********************************
  * @文件名     ： bsp.c
  * @作者       ： 周勇
  * @库版本     ： V2.2.0
  * @文件版本   ： V1.0.0
  * @日期       ： 2017年12月14日
  * @摘要       ： 板级支持包源文件
  ******************************************************************************/
/*----------------------------------------------------------------------------
  更新日志:
  2017-04-10 V1.0.0:初始版本
  ----------------------------------------------------------------------------*/
/* 包含的头文件 --------------------------------------------------------------*/
#include "bsp.h"
#include "stm8s.h"



/************************************************
函数名称 ： CLK_Configuration
功    能 ： 时钟配置
参    数 ： 无
返 回 值 ： 无
作    者 ： 周勇
*************************************************/
void CLK_Configuration(void)
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //HSI = 16M (1分频)
  CLK_ClockSecuritySystemEnable();
}

/************************************************
函数名称 ： GPIO_Configuration
功    能 ： 基本IO引脚配置
参    数 ： 无
返 回 值 ： 无
作    者 ： 周勇
*************************************************/
void GPIO_Configuration(void)
{
  GPIO_DeInit(GPIOD);
  GPIO_Init(LED_GPIO_PORT, (GPIO_Pin_TypeDef)LED_GPIO_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(CODE_GPIO_PORT, (GPIO_Pin_TypeDef)CODE_GPIO_PIN,GPIO_MODE_IN_FL_NO_IT);

#if 0 
  /* 系统时钟输出 */
  GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)GPIO_PIN_4, GPIO_MODE_OUT_PP_HIGH_FAST);
  CLK_CCOCmd(ENABLE);
#endif
}

/************************************************
函数名称 ：  EXTI_INIT
功    能 ： 外部中断触发方式配置
参    数 ： 无
返 回 值 ： 无
作    者 ： 周勇
*************************************************/
void EXTI_INIT(void)
{
  EXTI_DeInit();
  GPIO_Init(CODE2_GPIO_PORT ,(GPIO_Pin_TypeDef)CODE2_GPIO_PIN ,GPIO_MODE_IN_FL_IT);    //CODE2外部中断引脚      
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD , EXTI_SENSITIVITY_RISE_ONLY);              //下降沿触发
}
/************************************************
函数名称 ： SYS_Initializes
功    能 ： 板级支持包初始化
参    数 ： 无
返 回 值 ： 无
作    者 ： 周勇
*************************************************/
void SYS_Initializes(void)
{
  CLK_Configuration();          //时钟
  //GPIO_Configuration();         //引脚
 // EXTI_INIT();                  //外部中断
}


/**** Copyright (C)2017 strongerHuang. All Rights Reserved **** END OF FILE ****/
