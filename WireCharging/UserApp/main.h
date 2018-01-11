/********************************************
Title     :     main_h
McuBody   :     stm8s
Compiler  :		iar
SysClock  :     16MHz
Author    :     lw
Updata    :     2017-4-30
Version   :     V02              
********************************************/


#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/
#define OneU
// #define OneI
// #define ThreeU
// #define ThreeI
// #define ThreeUI

// #define COM

// #define SQRT_SELF
#define IN_I_E		5//输入额定电流

#include "stm8s.h"
#include "math.h"
#include "stm8s_it.h"
#include "stm8s_Dev.h"
//#include "Data.h"
//#include "Calculate.h"
//#include "TimLoop.h"
//#include "Display_Key.h"

typedef	struct
{
	bool				Sign;
	u16					T;
}Str_LoopDataU16;

typedef	struct
{
	bool				Sign;
	u8					T;
}Str_LoopSignByte;

// #define NUM_DEV_DATA 8
// 
// typedef	struct
// {
// 	uint16_t	Ia;
// 	uint16_t	Ib;
// 	uint16_t	Ic;
// 	uint8_t		State;
// 	uint8_t		rvs;
// }Str_DevData_Def;
// typedef	union
// {
// 	Str_DevData_Def		Str;
// 	uint8_t				Byte[NUM_DEV_DATA];
// }Un_DevData_Def;
// 
// extern	Un_DevData_Def		DevData;

#endif