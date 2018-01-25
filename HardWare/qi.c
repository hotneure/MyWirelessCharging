#include "qi.h"
#include "pid.h"
#include "exti.h"
#include "tim2.h"
#include "pwm.h"
#include "adc.h"

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
volatile u8 Samsung_Fast;
volatile u16 Timer;
volatile u8 Timer_Start;
volatile u32 Base_Fre;
volatile u8 Samsung;

void GPIO_UART(u16 data)
{
    u8 bit=0;
    long delay;
    GPIOB->ODR &=~1<<5;
    for(delay=20;delay>0;delay--){}
    GPIOB->ODR |= 1<<5;
    for(delay=5;delay>0;delay--){}
    for(bit=0;bit<10;bit++){
      if((data>>bit) & 0x01){
         GPIOB->ODR &=~1<<5;
         for(delay=3;delay>0;delay--){}
         GPIOB->ODR |= 1<<5;
         for(delay=5;delay>0;delay--){}      
      }else{
         GPIOB->ODR &=~1<<5;
         for(delay=10;delay>0;delay--){}
         GPIOB->ODR |= 1<<5;
         for(delay=5;delay>0;delay--){}
      }
    }
}

void delayms(u16 ms)
{ 
    
    ms *=100;
    ms /=32;
    Timer_Start =1;
    Timer =0;
    while(Timer<ms);
    Timer_Start = 0;
    
}


void Voltage_Check()
{
    frequency = 220000;
    PWM_Handler(SET_FREQUENCY); 
    PWM_Handler(OPEN_FOUR_PWM);
    Voltage_State = Get_ADC_Average(3);
   // Voltage_State=110;//zxy
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

    Breath_Led=0;
    GPIOB->DDR &= ~(1<<4);
    Rec_Start_Count=0;
    Timer_Counter=0;
    Qi_Packet.Flag =0;
    Rec_Timer_Counter=0;
    WPCQi_Phase = Ping_Phase;
    Samsung_Fast =0;
    Samsung=0;
    Stop_Rec();
    PWM_Handler(CLOSE_FOUR_PWM);
    while(Timer_Counter<1562);
    Start_Count=0;
}


void Convert_fre()
{

    if(Base_Fre == frequency/*GPIOB->IDR & 0X20*/){
        frequency -=5000;
         //GPIOB->ODR &= ~(1<<5);
    }else{
        frequency = Base_Fre;
      //GPIOB->ODR |= 1<<5;
    }
    PWM_Handler(SET_FREQUENCY);      
}


void Send_Data(u32 data,u8 data_length)
{
    u8 bit =0;
    
    Timer_Start =1;
    bit = data &0x01;
    while(data_length){
          Convert_fre();
          data_length--;    
          if(bit){
              Timer=0;
              while(Timer<5);
              Convert_fre();
              Timer=0;
              while(Timer<4);
          }else{
              Timer=0;Timer=0;
              while(Timer<9);
          }
          data>>=1;
          bit = data&0x01;    
    }
    Timer_Start =0;
}


void Fast_Charge_Request()
{
        Base_Fre = HIGH_VOLTAGE_FRE;
        frequency = Base_Fre;
        PWM_Handler(SET_FREQUENCY); 
        delayms(5);
        Send_Data(0x180B02,21);
        if(Qi_Packet.Flag) Qi_Packet.Flag=0;
}


void Ping()
{
    Timer_Counter =0;
    while(Timer_Counter < PING_TIME_OUT){
        if(Qi_Packet.Flag && Qi_Packet.Header == 0x01) break;
    }
    if(Qi_Packet.Flag && Qi_Packet.Header == 0x01){
        Stop_Rec();
        WPCQi_Phase = Identify_Config_Phase;            //开呼吸灯，进入配置阶段
        Breath_Led =1;
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
    Timer_Counter =0;
    while(Timer_Counter < PACKET_MAX){
        if(Qi_Packet.Flag && Qi_Packet.Header==0x71){
            if(Qi_Packet.Message[1]==0x11 && Qi_Packet.Message[2]==0x00 && Qi_Packet.Message[3]==0x42){
                if(Voltage_State){
                    Samsung_Fast =1;        //三星启动快充标志
                }
                Samsung = 1;
            }
            Last_Packet = 0x71;
            Timer_Counter=0;
            Stop_Rec();
            return;
         }
        
        if(Qi_Packet.Flag && Qi_Packet.Header==0x51 && Last_Packet == 0x71){
             Stop_Rec();
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
    static u8 time_out=0;
    u16 Timeout_Voltage=0;
    
    while(Timer_Counter < ERROR_PACKET_TIME_OUT && Rec_Timer_Counter <REC_PACKET_TIME_OUT){
      if(Timer > 8125 && time_out <3){                         //如果在2.6s内，超时的次数不超过3次   拿开手机时，一定会连续超时，如果在2.6s时间内超时次数不到3次，则手机还在
          time_out=0;
          Timer=0;
          Timer_Start=0;
      }
      if(Qi_Packet.Flag){
          Timer_Counter=0;
          Rec_Timer_Counter=0;
          switch(Qi_Packet.Header){
              case CONTROL_ERROR_PACKET:
                  if(Samsung_Fast){
                      Fast_Charge_Request();
                      return;
                  }
                  
                  if(Samsung){
                      PID(Qi_Packet.Message[1]);
                  }else{
                      if(Qi_Packet.Message[1]){
                          if(Qi_Packet.Message[1] & 0x80){
                              frequency += 3000;
                              PWM_Handler(SET_FREQUENCY); 
                          }else {
                              frequency -= 3000;
                              PWM_Handler(SET_FREQUENCY);
                          }                        
                      }
                  }
                  
                  break;     
              case RECEIVED_P0WER_PACKET:
                  Rec_Timer_Counter=0;
                  break;
              case SAMSUNG_FAST_CHARGE:
                  Samsung_Fast = 0;
                  break;
              case SAMSUNG_CHARGE_STATE:
                  break;
              case END_POWER_PACKET:
                  break;
          }
          Stop_Rec();
          return;
      }
    }
    Stop_Rec();
    Timer_Counter=0;
    Rec_Timer_Counter=0;
    if(Samsung){
        Return_Ping();//三星正常停止
      }else{
        time_out++;
        if(time_out==1){
            Timer_Start=1;
            Timer=0;
        }
        if(!(time_out%2)){
          if(Voltage_State){
              frequency =156800;
          }else{
              frequency =170000;
          }
            
            PWM_Handler(SET_FREQUENCY);
        }
        Timeout_Voltage=Get_ADC_Average(6);
        GPIO_UART(Timeout_Voltage);
        if(time_out>2){
            time_out=0;
            if(Timeout_Voltage <20){
                Return_Ping();
            }  
        }
    }

}

void WPC_QI()
{
    switch(WPCQi_Phase)
    {
        case Ping_Phase:
             if(Voltage_State){
                 Voltage_Check();
             }else{
                 frequency = LOW_VOLTAGE_FRE;
                 PWM_Handler(SET_FREQUENCY); 
                 PWM_Handler(OPEN_FOUR_PWM);
             }
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




