#include "qi.h"
#include "pid.h"
#include "exti.h"
#include "tim2.h"
#include "pwm.h"

volatile u8 PWM_State;
u8 Voltage_State;
u8 Large_Power;
volatile u8 T2_State;
uint32_t frequency;
QI_POWER_TRANSFER_PHASE_TYPEDEF      WPCQi_Phase;
QI_DATA_PACKET_TYPEDEF               Qi_Packet;


void Ping()
{
  // u16 Ping_Time_Out=0;
   
   TIM2->ARRH =(u8)(PING_TIME_OUT>>8);
   TIM2->ARRL|=(u8)PING_TIME_OUT;
   
   while(!(TIM2->SR1 & 0x01)){
       if(Qi_Packet.Flag){
           break;
       }
   }
   TIM2_Clr();
   if(Qi_Packet.Flag && Qi_Packet.Header == 0x01){
        WPCQi_Phase = Identify_Config_Phase;            //�������ƣ��������ý׶�
        Qi_Packet.Flag =0;
        
   }else{
     
       TIM2->ARRH =(u8)(PING_TIME>>8);
       TIM2->ARRL|=(u8)PING_TIME;
   
       while(!(TIM2->SR1 & 0x01)){};                                               //��ʱ��100ms������ping
       TIM2_Clr();
       PWM_Close();
       
       TIM2->ARRH =(u8)(PING_DELAY>>8);
       TIM2->ARRL|=(u8)PING_DELAY;
       
       while(!(TIM2->SR1 & 0x01)){
           //T2_State = TIM2->SR1;
       }
       TIM2_Clr();
   }
}



void ID_Config()
{
    
}


void Power_Transfer()
{
    
}


void WPC_QI()
{
    switch(WPCQi_Phase)
    {
        case Ping_Phase:
             PWM_Return();                               //��ʼping����ʼ65ms��ʱ���ж��Ƿ���յ��ź�ǿ�Ȱ���
             TIM2->CR1|=0x01;                            //û�н��յ�����������ʱ20�����н��յ��׶���Ϊ���ý׶� 
             Ping();
             break;
             
        case Identify_Config_Phase:                          
          //
            break;
        case Power_Transfer_Phase:
            break;
    }
}


