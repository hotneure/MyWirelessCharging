/**
  ******************************************************************************
  * @file    Project/SimUArt.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "SimUart.h"
#include "bsp.h"
#include "stm8s.h"

u8 StartTIM4;
u8 RxDataBuf[7];
u8 RxDataNum;
u8 RxData_ValidNum;
u16 RxDataValue_Temp;
u16 RxDataValue;           //��������  
u16 ExitIntCnt;            //���ڲⶨ�ⲿ�ж�ʱ��CODE_DET�ĵ�ƽ

u8 parity = 0;      //��żЧ��

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void ReadCode_det(void)
{
 RxDataNum++;                    //���յ���λ����  
     
    //�ڶ�������IO����   
    if( !(GPIOD->IDR & 0x08) )//�ߵ�ƽ��1  
    {  
        RxDataValue |= ( 0x01 << (RxDataNum) );//��λ ��1    
        parity = (parity+1) & 0x01;
    }      
   
      
    //���������ж��Ƿ������11��λ            //���ݸ�ʽ 1����ʼλ+8λ����+1λ��Ч��λ+1λֹͣλ              
    if( ( 9 == RxDataNum ) )                 //�������  
    {  
        RxDataNum = 0;  
           
        //������Ч����  
        if( ((RxDataValue & 0x02) >> 1) == parity )          //��Ч��λ��ȷ
        {  
            RxDataValue_Temp = ( RxDataValue >> 2 );       
            RxDataBuf[ RxData_ValidNum ] = RxDataValue_Temp;  
            RxData_ValidNum ++;  
              
            //=========================�û����==============================  
              
            if(  RxData_ValidNum <= 1 )          //����ͷ  
            {  
                if( (RxData_ValidNum == 1) )  
                {  
                    /******************************** 
                    ��������Ϊ���£� 
                    16 05 02 01 01 01 01 21 
                    *********************************/  
                    if( (RxDataBuf[0] == 0Xff)  )  //��ͷ����0xffΪ��Ч����
                    {  
                        RxDataBuf[0] = RxDataBuf[1];  
                        RxData_ValidNum = 1;  
                        StartTIM4=0;                              //�رն�ʱ��
                        return;
                    }  
                }  
                 
            }  
              
            //�����һ�����ݰ�  
            if( 7 == RxData_ValidNum )  
            {   
                //���У��λ  
                u8 i = 0;  
                u8 Check = 0 ;  
                for( i = 0; i < 6; i++ )  
                {  
                    Check += RxDataBuf[i];  
                }  
                //Check = 0xFF - Check + 1;  
                  
                //���У��λ������ش���  
                if( Check == RxDataBuf[6] )         //���У��λ  
                {  
    
                      
                    RxData_ValidNum = 0;            //��������  
                }  
            }  
        }               
    }  

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
