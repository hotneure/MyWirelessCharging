#ifndef _ADC_H
#define _ADC_H

#include "stm8s.h"

void ADC_Config(void);
u16 Get_ADC_Average(unsigned char);
u16 Get_ADC_Data(void);

#endif
