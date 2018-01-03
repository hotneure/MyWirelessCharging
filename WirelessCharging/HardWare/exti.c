#include "exti.h"
#include "stm8s.h"

#define MASTER  1
#define SLAVER  0

u16 buffer[10];
static u8 bufferIndex=0,bufferLength=0,bitNum = 10;
static u8 pre_bit=0,no_pre_bit=0,noFinish=0, Last_State=0,start_record;

void exti_init()
{
    EXTI_DeInit();
    GPIO_Init(GPIOD,   GPIO_PIN_5,   GPIO_MODE_IN_PU_IT);  
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
    EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);
}

void TIM4_Init()
{
  CLK->PCKENR1 |= 0x10;
  TIM4_TimeBaseInit(TIM4_PRESCALER_64, 150);
  /* Clear TIM4 update flag */
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
}
void TIM4_Reset()
{
    TIM4_ClearFlag(TIM4_FLAG_UPDATE);
    TIM4->CNTR =0;
    TIM4_Cmd(DISABLE);
}

void Stop_Rec()
{
    TIM4_Reset();
    EXTI->CR1 &=~(3<<6);
    EXTI->CR1 |= 2<<6;
    noFinish =0;
    Last_State=0;
    bitNum = 10;
    bufferIndex=0;
    bufferLength=0;
    start_record=0;
    pre_bit =0;
    no_pre_bit=0;
}


char data_check(int data)
{
    char parityNum,parity=0,result =0;
    parity= (data &0x02)>>1;
    data >>=2;
    for(parityNum=0;parityNum<8;parityNum++){
        result += (data & 0x01);
        data >>=1;
    }
    if((result+1)%2 ==parity){
        return 1;
    }else{
        return 0;
    }
}


char decode_header(int header)
{
  if(data_check(header)){
    switch(header>>2){          //У��ɹ�
        case 0x01:
        case 0x03:
        case 0x04:
            return 3;
            break;
        case 0x51:
            return 7;
            break;
        case 0x71:
            return 9;
            break;
        case 0xFF:
            return 2;
            break;
        default:
            return 0;
            break;
    }
  }else{
    //У��ʧ�ܣ���������
    Stop_Rec();
    return 0;
  }
}

void Save_Bit(unsigned char bit)
{
  if((pre_bit>=11)&&(pre_bit<=25)&& no_pre_bit==0 && bit==0){    //���������11��25��1���Ҹս��յ�0
    start_record =1;
  }
  if(start_record){
        if(bitNum ==0){
            if(bit){
                buffer[bufferIndex] |= 1;
            }else{
                buffer[bufferIndex] &=~1;
            }
                bufferIndex++;
                bitNum = 10;
            if(bufferIndex==1){
                bufferLength =decode_header(buffer[0]);
            }
            if(bufferIndex==bufferLength){
                Stop_Rec();
            }
         }else{
            if(bit){
                if(bitNum>0){
                    buffer[bufferIndex] |= 1<< bitNum;
                    bitNum--; 
                }
            }else{
                if(bitNum>0){
                    buffer[bufferIndex] &=~(1<<bitNum); 
                    bitNum--; 
                }
            }
        }
  }
}
#if MASTER

INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
    static u8 time1=0,time2=0;
    GPIOC->ODR |= 0x40;
    if(noFinish){
        if(Last_State){             //�ϴ�δ������ĵ�ƽ�Ǹߵ�ƽ
            while((!(GPIOD->IDR & 0x20))&& (!(TIM4->SR1 & 0x01)));
            time2 = TIM4->CNTR;
            if(TIM4->SR1 & 0x01){   //��������ĵȴ�
                Stop_Rec();
            }else{
                if(time2>100 ){         //���T1��T2����400us
                    Save_Bit(1);        //����һ��1
                    no_pre_bit++;
                    TIM4_Reset();
                }
            }
            Last_State=0;
        }else{                     //�ϴ�δ������ĵ�ƽ�ǵ͵�ƽ
            while((GPIOD->IDR & 0x20)&& (!(TIM4->SR1 & 0x01)));
            time2 = TIM4->CNTR;
            if(TIM4->SR1 & 0x01){   //��������ĵȴ�
                Stop_Rec();
            }else{
                if(time2>100 ){
                    Save_Bit(1);         //����һ��1
                    pre_bit++;
                    TIM4_Reset();
                }
            }
        }
        noFinish=0;
    }else{
        TIM4_Cmd(ENABLE);
        if(GPIOD->IDR & 0x20){             //������ ����
            while((GPIOD->IDR & 0x20)&&(!(TIM4->SR1 & 0x01)));
            time1 = TIM4->CNTR ;
            if(TIM4->SR1 & 0x01){   //��������ĵȴ�
                Stop_Rec();
            }else{
                if(time1>100){
                    Save_Bit(0);//����һ��0
                    TIM4_Reset();
                }else{
                    noFinish = 1;       //����δ�����꣬��¼��ƽ״̬
                    Last_State = 1;
                }
            }
        }else{                      //�½��ش���
            EXTI->CR1 |= 3<<6;
            while((!(GPIOD->IDR & 0x20))&&(!(TIM4->SR1 & 0x01)));      //�ȴ���ƽ��Ϊ�ߣ����Ҽ�ʱ��T4���������ȴ�
            time1 = TIM4->CNTR ;
            if(TIM4->SR1 & 0x01){   //����������ĵȴ�
                Stop_Rec();
            }else{
                if(time1>100){
                    Save_Bit(0);//����һ��0
                    no_pre_bit++;
                    TIM4_Reset();
                }else{
                    noFinish = 1;       //����δ�����꣬��¼��ƽ״̬
                    Last_State = 0;
                }
            }
        }
    }
    GPIOC->ODR &=~ 0x40;
}

#endif

#if SLAVER

char haha[20];

INTERRUPT_HANDLER(EXTI_PORTD_IRQHandler, 6)
{
    static char times;
    
    //EXTI->CR1 |= 3<<6;
    
    if(noFinish){
      if(Last_State){
            
      }else{
            
      }
    }else{
        if(GPIOD->IDR & 0x08){              //�����ش���  
            TIM4_Cmd(ENABLE);
            while(GPIOD->IDR & 0x08);
            haha[times++] = TIM4->CNTR;
            TIM4_Reset();
       
        } else{                            //�½��ش���
            TIM4_Cmd(ENABLE);
            while(!(GPIOD->IDR & 0x08));
            haha[times] = TIM4->CNTR;
            if(haha[times]>100){
                no_pre_bit++;
                TIM4_Reset();
            }else{
                noFinish = 1;       //����δ�����꣬��¼��ƽ״̬
                Last_State = 0;
            }

        }
    }
    if(times>=19)
    times =0;
}

#endif