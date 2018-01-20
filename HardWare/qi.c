#include "qi.h"
#include "pid.h"
#include "exti.h"
#include "tim2.h"
#include "pwm.h"
#include "adc.h"

u8 Large_Power;
uint32_t frequency;
QI_POWER_TRANSFER_PHASE_TYPEDEF      WPCQi_Phase;
QI_DATA_PACKET_TYPEDEF               Qi_Packet;
volatile u8 PWM_State;
volatile u8 Breath_Led;
volatile u8 Start_Count;
volatile u8 Rec_Start_Count;
volatile u16 Voltage_State;
volatile u16 Timer_Counter;
volatile u32 Rec_Timer_Counter;

void GPIO_UART(u16 data)
{
    u8 bit=0;
    long delay;
    GPIOB->ODR &=~1<<5;
    for(delay=20;delay>0;delay--){
        
    }
    GPIOB->ODR |= 1<<5;
    for(delay=5;delay>0;delay--){
        
    }
    for(bit=0;bit<8;bit++){
      if((data>>bit) & 0x01){
         GPIOB->ODR &=~1<<5;
         for(delay=3;delay>0;delay--){
        
         }
         GPIOB->ODR |= 1<<5;
         for(delay=5;delay>0;delay--){
        
         }      
      }else{
         GPIOB->ODR &=~1<<5;
         for(delay=10;delay>0;delay--){
        
         }
         GPIOB->ODR |= 1<<5;
         for(delay=5;delay>0;delay--){
        
         }
      }
    }
}


void Voltage_Check()
{
  
    Voltage_State = Get_ADC_Average(3);
   // GPIO_UART(Voltage_State);   //zhang
    if(Voltage_State>20){
        Voltage_State = 1;
        PWM_Handler(CLOSE_TWO_PWM);
        frequency = HIGH_VOLTAGE_FRE;
        PWM_Handler(SET_FREQUENCY);
    }else{
        Voltage_State = 0;
        GPIOA->ODR &=~(1<<2);
        frequency = LOW_VOLTAGE_FRE;
        PWM_Handler(SET_FREQUENCY);
    }
    
}

void Return_Ping()
{
  GPIO_UART(Timer_Counter);
    Breath_Led=0;
    GPIOB->DDR &= ~(1<<4);
    Start_Count=0;
    Rec_Start_Count=0;
    Timer_Counter=0;
    Qi_Packet.Flag =0;
    Rec_Timer_Counter=0;
    WPCQi_Phase = Ping_Phase;
    Stop_Rec();
}

void Ping()
{
    while(Timer_Counter < PING_TIME_OUT){
        if(Qi_Packet.Flag && Qi_Packet.Header == 0x01) break;
    }
    if(Qi_Packet.Flag && Qi_Packet.Header == 0x01){
        Stop_Rec();
        WPCQi_Phase = Identify_Config_Phase;            //开呼吸灯，进入配置阶段
        Breath_Led =1;
        Qi_Packet.Flag =0; 
    }else{
       while(Timer_Counter < PING_TIME){};                                               //延时到100ms，重新ping
       PWM_Handler(CLOSE_FOUR_PWM);
       Timer_Counter=0;
       while(Timer_Counter < PING_DELAY){};
       Timer_Counter=0;
   }
   
   
}



void ID_Config()
{
    static u8 Last_Packet=0;
    
    while(Timer_Counter < PACKET_MAX){
        if(Qi_Packet.Flag && Qi_Packet.Header==0x71){
            Last_Packet = 0x71;
            Timer_Counter=0;
            return;
         }
        if(Qi_Packet.Flag && Qi_Packet.Header==0x51 && Last_Packet == 0x71){
             WPCQi_Phase = Power_Transfer_Phase;
             Timer_Counter=0;
             Rec_Start_Count=1;
             return;
        }
    }
    Return_Ping();  
}


void Power_Transfer()
{
    Timer_Counter=0;
    while(Timer_Counter < ERROR_PACKET_TIME_OUT && Rec_Timer_Counter <REC_PACKET_TIME_OUT){
      if(Qi_Packet.Flag){
          switch(Qi_Packet.Header){
              case CONTROL_ERROR_PACKET:
                  PID(Qi_Packet.Message[1]);
                  break;     
              case RECEIVED_P0WER_PACKET:
                  Rec_Timer_Counter=0;
                  break;
              case SAMSUNG_FAST_CHARGE:
                  break;
              case SAMSUNG_CHARGE_STATE:
                  break;
              case END_POWER_PACKET:
                  break;
          }
          Qi_Packet.Flag=0;
          return;
      }
    }
    Return_Ping();
}

void WPC_QI()
{
    switch(WPCQi_Phase)
    {
        case Ping_Phase:

             PWM_Handler(OPEN_FOUR_PWM);    
             if(Voltage_State) Voltage_Check();
             Start_Count=1;
             TIM2->CR1|=0x01;                            
             Ping();
             break;
             
        case Identify_Config_Phase:                          
            ID_Config();
            break;
        case Power_Transfer_Phase:
            Power_Transfer();
            break;
    }
}


