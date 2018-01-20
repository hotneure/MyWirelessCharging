#include "adc.h"
#include "stm8s.h"

void ADC1_DeInit(void)
{
  ADC1->CSR  = 0x00;
  ADC1->CR1  = 0x00;
  ADC1->CR2  = 0x00;
  ADC1->CR3  = 0x00;
  ADC1->TDRH = 0x00;
  ADC1->TDRL = 0x03;
  ADC1->HTRH = 0xFF;
  ADC1->HTRL = 0x03;
  ADC1->LTRH = 0x03;
  ADC1->LTRL = 0x03;
  ADC1->AWCRH = 0x03;
  ADC1->AWCRL = 0x03;
}

void ADC_Config()
{
    CLK->PCKENR2 |=0x08;

    GPIOD->CR2 &= (uint8_t)(~0x04);
    GPIOD->DDR &= (uint8_t)(~0x04);
    GPIOD->CR1 &= (uint8_t)(~0x04);
    
    /* De-Init ADC peripheral*/
    ADC1_DeInit();
    ADC1->CR2 &= (uint8_t)(~0x08);
    ADC1->CR2 |= (uint8_t)(0x00);
    ADC1->CR1 |= 0x02;
    ADC1->CSR &= (uint8_t)(~0x0F);
    ADC1->CSR |= (uint8_t)(0x03);
    ADC1->CR1 &= (uint8_t)(~0x70);
    ADC1->CR1 |= (uint8_t)(0x00);
    ADC1->CR2 &= (uint8_t)(~0x40);
    ADC1->CR2 &= (uint8_t)(~0x30);
    ADC1->CR2 &= (uint8_t)(~0x40);
    ADC1->CR2 |= (uint8_t)(0x00);
    ADC1->TDRL |= (uint8_t)((uint8_t)0x01 << (uint8_t)0x03);
    ADC1->CR1 |= 0x01;
}

u16 Get_ADC_Average(u8 times)
{
    u8 i;
    unsigned long sum=0;
    sum = Get_ADC_Data();
    for(i=0;i<times;i++)
    {
        sum += Get_ADC_Data();
    }
    return sum/times;
}

u16 Get_ADC_Data()
{
    unsigned int delay_times=2;
    unsigned int temp=0;
    
    ADC1->CR1 |= 0x01;
    while(delay_times--);
    ADC1->CR1 |= 0x01;
    while(!(ADC1->CSR & 0x80)){};
    ADC1->CSR &= (uint8_t)(~0x80);
    temp |= ADC1->DRH<<2;
    temp |= ADC1->DRL ; 
    return temp;
}