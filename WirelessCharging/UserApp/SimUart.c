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
u16 RxDataValue;           //变量清零  
u16 ExitIntCnt;            //用于测定外部中断时，CODE_DET的电平

u8 parity = 0;      //奇偶效验

/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void ReadCode_det(void)
{
 RxDataNum++;                    //接收到的位计数  
     
    //第二步，读IO输入   
    if( !(GPIOD->IDR & 0x08) )//高电平置1  
    {  
        RxDataValue |= ( 0x01 << (RxDataNum) );//该位 置1    
        parity = (parity+1) & 0x01;
    }      
   
      
    //第三步，判断是否接收了11个位            //数据格式 1个起始位+8位数据+1位奇效验位+1位停止位              
    if( ( 9 == RxDataNum ) )                 //如果是则  
    {  
        RxDataNum = 0;  
           
        //处理有效数据  
        if( ((RxDataValue & 0x02) >> 1) == parity )          //奇效验位正确
        {  
            RxDataValue_Temp = ( RxDataValue >> 2 );       
            RxDataBuf[ RxData_ValidNum ] = RxDataValue_Temp;  
            RxData_ValidNum ++;  
              
            //=========================用户添加==============================  
              
            if(  RxData_ValidNum <= 1 )          //数据头  
            {  
                if( (RxData_ValidNum == 1) )  
                {  
                    /******************************** 
                    测试数据为如下： 
                    16 05 02 01 01 01 01 21 
                    *********************************/  
                    if( (RxDataBuf[0] == 0Xff)  )  //包头等于0xff为无效数据
                    {  
                        RxDataBuf[0] = RxDataBuf[1];  
                        RxData_ValidNum = 1;  
                        StartTIM4=0;                              //关闭定时器
                        return;
                    }  
                }  
                 
            }  
              
            //获得了一个数据包  
            if( 7 == RxData_ValidNum )  
            {   
                //获得校验位  
                u8 i = 0;  
                u8 Check = 0 ;  
                for( i = 0; i < 6; i++ )  
                {  
                    Check += RxDataBuf[i];  
                }  
                //Check = 0xFF - Check + 1;  
                  
                //检查校验位并做相关处理  
                if( Check == RxDataBuf[6] )         //检查校验位  
                {  
    
                      
                    RxData_ValidNum = 0;            //长度清零  
                }  
            }  
        }               
    }  

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
