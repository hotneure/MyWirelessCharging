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

extern u8 StartTIM4;               //�ж��Ƿ�����ʱ��
extern u8 RxDataBuf[7];
extern u8 RxDataNum;
extern u8 RxData_ValidNum;
extern u16 RxDataValue_Temp;
extern u16 RxDataValue;           //��������  

extern u16 ExitIntCnt;            //���ڲⶨ�ⲿ�ж�ʱ��=1ʱ��CODE_DET�ĵ�ƽ��0=�����ݣ�1=û����
                                                     //>1ʱ��������
/* Includes ------------------------------------------------------------------*/
void ReadCode_det(void);
#endif