#include "pid.h"
#include "pwm.h"
#include "adc.h"
#include "stm8s.h"


//u8 Kd=0;
u8 Rise_Power =0;
float tim_inner=7.0E+0;
float Kp=1.0E+1;
float  M1=3.0E+3;
float Mpid=2.0E+4;
float Sv=-1.0E-2, Ki= 0.05,integral=0;
//float arry[8];
//int fre_buf[7];
//u8 lenth;

void PID(signed char error_value)
{
	char i;
        long delay3ms;
	float Last_Cir_Curr=0 ,Act_Curr=0,Deter_Curr=0;
        
        Act_Curr =(float)Get_ADC_Average(10);					        //读取的实际电流值
        if(error_value & 0x80){
            Rise_Power = 0;
            Deter_Curr = Act_Curr*(1+(float)(~(error_value-1))/128);
        }else{
            Rise_Power = 1;
            Deter_Curr = Act_Curr*(1-(float)error_value/128);
        }
        
	for(i=0;i<7;i++)
	{
		Last_Cir_Curr =(float)Get_ADC_Average(10);				//读取的实际电流值
                for(delay3ms=0;delay3ms<800;delay3ms++)
                {
                
                }
		PID_Circulation(Deter_Curr,Last_Cir_Curr);
                    
	}
       // Last_deviation=0 ;
        integral=0;
        //lenth=0;
}


void PID_Circulation(float DeterCurr, float LastCirCurrent)
{
        static u8 Double_Pwm=0;
	static float  deviation=0, Contr_Varia=175,MinFrequency = 1.1E+2,MaxFrequency = 2E+2;
        float  PID_Sum,proportion;
        
	//Last_deviation =deviation;
	deviation =DeterCurr - LastCirCurrent;
        //zhangxinyun
       //arry[lenth]=deviation;
	proportion = Kp*deviation;
	integral += Ki*deviation*tim_inner;
	//differential = Kd*(deviation-Last_deviation)/tim_inner;
        if(integral>M1){
            integral = M1;
        }
        if(integral< -1*M1){
            integral = -1*M1;
        }
        
	PID_Sum=proportion+integral;
        
        if(PID_Sum>Mpid){
            integral = Mpid;
        }
        if(PID_Sum< -1*Mpid){
            integral = -1*Mpid;
        }
        
        
	Contr_Varia -= (Sv * PID_Sum);
        
        if(Contr_Varia<MinFrequency){
            Contr_Varia =MinFrequency;
            if(Rise_Power && Double_Pwm ==0){
                Double_Pwm = 1;
                Contr_Varia=155;
                
            }
        }
        if(Contr_Varia>MaxFrequency){
            Contr_Varia =MaxFrequency;
            if(Rise_Power==0 && Double_Pwm){
                Double_Pwm = 0;
                Contr_Varia=175;
          }
        }
        //zhangxinyun
       // fre_buf[lenth++]= Contr_Varia;
        
        frequency = (uint32_t)((float)0x3E8*Contr_Varia);
        Set_PWM_Fre();
}

