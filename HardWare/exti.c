#include "exti.h"
#include "stm8s.h"
#include "qi.h"



//��������ʹ�õ�״̬����
volatile u8 bufferIndex;
volatile u8 bufferLength;
volatile u8 bitNum;
volatile u8 Bytecheck;
volatile u8 start_record;
volatile u8 Byte_State;

//����ʹ�õ�״̬����
volatile u8 Times_State = INIT_STATE;
volatile u8 pre_bit;
volatile u8 first_start_bit;
volatile u8 Level_State ;

void exti_init()
{
    EXTI_DeInit();
    GPIO_Init(GPIOD,   GPIO_PIN_5,   GPIO_MODE_IN_PU_IT);  
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);
    EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
}

void TIM4_Init()
{
  TIM4_TimeBaseInit(TIM4_PRESCALER_64, 150);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
}

//TIM4��λ
void TIM4_Reset()
{
    TIM4->SR1 &= ~0x01;
    TIM4->CNTR =0;
}

//ֹͣ���ν��գ�����״̬������λ
void Stop_Rec()
{
   // u8 clear;
    
    //for(clear=0;clear<bufferLength;clear++){
  //     Qi_Packet.Message[clear] = CLEAR_VALUE;
  //  }
    TIM4_Reset();
  //TIM4_Cmd(DISABLE);
    bufferIndex=0;
    Byte_State=STARTCHECK;
    bufferLength=0;
    bitNum =0;
    start_record=0;
    pre_bit =0;
    Bytecheck = 0;
    first_start_bit=0;
    Qi_Packet.CheckSum =0;
}




/*
У������ͷ�������ؼ�������ݳ���

*/

u8 decode_header(u8 header)
{
    switch(header){        
        case STRENGTH_PACKET:
            Qi_Packet.Header =STRENGTH_PACKET;
            return 3;
            break;
        case END_POWER_PACKET:
            Qi_Packet.Header =END_POWER_PACKET;
            return 3;
            break;
        case CONTROL_ERROR_PACKET:
            Qi_Packet.Header =CONTROL_ERROR_PACKET;
            return 3;
            break;
        case RECEIVED_P0WER_PACKET:
            Qi_Packet.Header = RECEIVED_P0WER_PACKET;
            return 3;
            break;
        case CONFIG_PACKET:
            Qi_Packet.Header = CONFIG_PACKET;
            return 7;
            break;
        case ID_PACKET:
            Qi_Packet.Header = ID_PACKET;
            return 9;
            break;
        default:
            return 0;
            break;
    }
}

void Save_Bit(u8 bit)
{
 u8 a =80 ;

  if(!start_record && (pre_bit>=10)&&(pre_bit<=25)&& first_start_bit==1){    //���������11��25��1�����յ���ʼλ0
    start_record =1;
  }

  if(start_record){
      switch(Byte_State){
          case STARTCHECK:              //�տ�ʼ���գ�У����ʼλ
        GPIOB->ODR &= ~(1<<5);
      while(a--);
       GPIOB->ODR |= 1<<5;
              
              if(bit!=0){		//���������ʼλ��ֹͣ����
                  Stop_Rec();
		  break;
	      }else{
		  Byte_State=REC_DATA;   //�������ʼλ���´ν�����ս׶�
		  break;
	      }
          case REC_DATA:                //��������
            
              if(bit){

	          Qi_Packet.Message[bufferIndex] |=1<<bitNum;           
		  Bytecheck++; 
	      }else{
	          Qi_Packet.Message[bufferIndex] &= ~(1<<bitNum);
	      }
              
	      bitNum++;
	      if(bitNum == 8){
	          Byte_State=BYTECHECK;
                  bufferIndex++;
	      }
	      break;
          case BYTECHECK:               //��żУ�飬��żλ��У��
            
              if(Bytecheck%2 == bit){
                  Stop_Rec();
	          break; 
	      }
   
	      Byte_State =STOPCHECK; 
              Bytecheck = 0;
	      break;
          case STOPCHECK:               //ֹͣλУ��
            
            	if(bit){
                    if(bufferIndex==1){
                       if(decode_header(Qi_Packet.Message[0])){
                         
                            bufferLength = decode_header(Qi_Packet.Message[0]);
                            Qi_Packet.CheckSum =Qi_Packet.Message[0];
                            
                       }else{
                            Stop_Rec();                         //��Ч�����ݰ�
                            break;
                       }
                    }
                    
                    Byte_State = STARTCHECK;
                    bitNum =0;
                   
                    if(bufferIndex==bufferLength && bufferLength){
                        bufferIndex -= 0x02;
                        for(bufferIndex;bufferIndex>0;bufferIndex--){
                            Qi_Packet.CheckSum ^= Qi_Packet.Message[bufferIndex];     //���ݰ�У��
                        }  
                        
                       if(Qi_Packet.CheckSum== Qi_Packet.Message[bufferLength-1]){
                           Qi_Packet.Flag =1;
                       }
                       
                    }
		}else{
                    Stop_Rec();
		}
		break;
    }
  }

}



INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{

    u8  time =0;
    u8  total_bit=0;

    if(TIM4->SR1 & 0x01) {                
        Stop_Rec();
        Times_State = INIT_STATE;
    }
    
    TIM4_Cmd(ENABLE);       
    if(Times_State==INIT_STATE){
      Level_State = GPIOD->IDR;
      if(!(Level_State & 0x20)){
         Times_State =TIME1;
         TIM4_Reset();
      } 
    }
        switch(Times_State){
            case TIME1:                                 //���¼���״̬���տ�ʼ��¼�����ϴμ�¼��ɵ�״̬
                Times_State = TIME2;
                break;
            case TIME2:                                 //�����TIME1��˵���ڶ��ν����ж�
              
                time = TIM4->CNTR;  

                if(time>100){                            //����400us������һ��0
                    first_start_bit++; 
                    Save_Bit(0);
                    
                    TIM4_Reset();
                    Times_State = TIME2;
                }else{
                    Times_State = TIME3;
                }
                
                break;
                
            case TIME3:
                time = TIM4->CNTR;
                    if(time>100){                            //�������400us
 
                        pre_bit++;
                        Save_Bit(1);                         //����һ��1
                        TIM4_Reset();                        //���¼�ʱ
                        Times_State = TIME2;
                    }
                
                break;
            default:
                break;
        }
    total_bit = pre_bit;
    total_bit+= first_start_bit;
    if(total_bit> 110){
        Stop_Rec();
    }

}


