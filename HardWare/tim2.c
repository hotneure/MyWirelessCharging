#include "tim2.h"
#include "stm8s.h"
#include "qi.h"

void CLK_Configuration(void)
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); //HSI = 16M (1·ÖÆµ)
  CLK_ClockSecuritySystemEnable();
}

void TIM2_DeInit(void)
{
  TIM2->CR1 = (uint8_t)0x00;
  TIM2->IER = (uint8_t)0x00;
  TIM2->SR2 = (uint8_t)0x00;
  
  /* Disable channels */
  TIM2->CCER1 = (uint8_t)0x00;
  TIM2->CCER2 = (uint8_t)0x00;
  
  
  /* Then reset channel registers: it also works if lock level is equal to 2 or 3 */
  TIM2->CCER1 = (uint8_t)0x00;
  TIM2->CCER2 = (uint8_t)0x00;
  TIM2->CCMR1 = (uint8_t)0x00;
  TIM2->CCMR2 = (uint8_t)0x00;
  TIM2->CCMR3 = (uint8_t)0x00;
  TIM2->CNTRH = (uint8_t)0x00;
  TIM2->CNTRL = (uint8_t)0x00;
  TIM2->PSCR = (uint8_t)0x00;
  TIM2->ARRH  = (uint8_t)0xFF;
  TIM2->ARRL  = (uint8_t)0xFF;
  TIM2->CCR1H = (uint8_t)0x00;
  TIM2->CCR1L = (uint8_t)0x00;
  TIM2->CCR2H = (uint8_t)0x00;
  TIM2->CCR2L = (uint8_t)0x00;
  TIM2->CCR3H = (uint8_t)0x00;
  TIM2->CCR3L = (uint8_t)0x00;
  TIM2->SR1 = (uint8_t)0x00;
}


void TIM2_Init()
{ 
  
    TIM2_DeInit();

    TIM2->PSCR = (uint8_t)(0x07);
    TIM2->ARRH = (uint8_t)( 0x28>> 8);
    TIM2->ARRL = (uint8_t)(0x28);
    TIM2->IER |=0x01;
}
   
 INTERRUPT_HANDLER(TIM2_UPD_OVF_BRK_IRQHandler, 13)
 {
    static u16 Counter=0;
    static u8 duty = 1;
    static u8 LED_State =0;
    static u8 expire=0;
    
    if(Breath_Led){
        Counter++;
        if(LED_State==0 && Counter==1550){
            LED_State=2;
            expire=0;
            Counter=0;
        }else if(LED_State==1 && Counter==1550){
            LED_State=2;
            expire=1;
            Counter=0;
        }else if(LED_State==2){
            if(expire){
                if(duty==0){
                    LED_State=0;
                    GPIOB->DDR &=~(1<<4);
                    return;
                }
                if(Counter==65){
                GPIOB->DDR|=1<<4;
                Counter =0;
                duty--;
                }
            }else{
                if(duty==65){
                    LED_State=1;
                    GPIOB->DDR|=1<<4;
                    return;
                }
                if(Counter==65){
                    GPIOB->DDR|=1<<4;
                    Counter =0;
                    duty++;
                } 
            }
            if(Counter==duty)  GPIOB->DDR &=~(1<<4);
        }       
    }

    if(Start_Count){
        Timer_Counter++;
        if(Rec_Start_Count) Rec_Timer_Counter++;
    }
    TIM2->SR1 &= ~(0x01);
 }