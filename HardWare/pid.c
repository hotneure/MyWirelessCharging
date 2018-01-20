#include "pid.h"
#include "pwm.h"
#include "adc.h"
#include "stm8s.h"
#include "qi.h"


const float tim_inner=3.0E+0;
const float Kp=1.0E+1;
const float  M1=3.0E+3;
const float Mpid=2.0E+4;
const float Ki= 0.05;
float integral=0;

void PID(signed char error_value)
{
	char i;
	float Last_Cir_Curr=0 ,Act_Curr=0,Deter_Curr=0;
        
        Act_Curr =(float)Get_ADC_Average(10);					        //读取的实际电流值
        if(error_value & 0x80){
            Deter_Curr = Act_Curr*(1-(float)(~(error_value-1))/128);
        }else{
            Deter_Curr = Act_Curr*(1+(float)error_value/128);
        }
        
	for(i=0;i<6;i++)
	{
		Last_Cir_Curr =(float)Get_ADC_Average(10);				//读取的实际电流值
		PID_Circulation(Deter_Curr,Last_Cir_Curr);
                    
	}
        integral=0;

}


void PID_Circulation(float DeterCurr, float LastCirCurrent)
{
        float  PID_Sum=0,proportion=0,deviation=0,Sv=0;
        
        if(110000<frequency && frequency<140000){
            Sv = 1.5;
        }else if(140000<frequency && frequency<160000){
            Sv = 2.0;
        }else if(160000<frequency && frequency<180000){
            Sv = 3.0;
        }else if(180000<frequency && frequency<205000){
            Sv = 5.0;
        }
	deviation =DeterCurr - LastCirCurrent;

	proportion = Kp*deviation;
	integral += Ki*deviation*tim_inner;

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
        
        
	frequency -= (uint32_t)(Sv * PID_Sum);
        
        if(frequency<(float)MINFREQUENCY) frequency =(float)MINFREQUENCY;
        if(frequency>(float)MAXFREQUENCY) frequency =(float)MAXFREQUENCY;
        
        //frequency = (uint32_t)(((float)FREQUENCY_MAGNIFY) * Contr_Varia);
        PWM_Handler(SET_FREQUENCY);
}

