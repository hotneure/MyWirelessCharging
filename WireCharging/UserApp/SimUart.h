/********************************************
Title     :     SimUart_h
McuBody   :     stm8s
Compiler  :		iar
SysClock  :     16MHz
Author    :     lw
Updata    :     2017-4-30
Version   :     V02              
********************************************/

#ifndef __SimUart_H__
#define __SimUart_H__

#include "stm8s.h"

extern u8 StartTIM4;               //判断是否开启定时器
extern u8 RxDataBuf[7];
extern u8 RxDataNum;
extern u8 RxData_ValidNum;
extern u16 RxDataValue_Temp;
extern u16 RxDataValue;           //变量清零  

extern u16 ExitIntCnt;            //用于测定外部中断时，=1时，CODE_DET的电平，0=有数据，1=没数据
                                                     //>1时，无数据
/* Includes ------------------------------------------------------------------*/
void ReadCode_det(void);
#endif