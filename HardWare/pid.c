#include "pid.h"
#include "pwm.h"
#include "adc.h"
#include "stm8s.h"
#include "qi.h"


u8 Rise_Power =0;
float tim_inner=3.0E+0;
float Kp=1.0E+1;
float  M1=3.0E+3;
float Mpid=2.0E+4;
float Sv=-1.0E-2, Ki= 0.05,integral=0;

void PID(signed char error_value)
{
	char i;
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
		PID_Circulation(Deter_Curr,Last_Cir_Curr);
                    
	}
        integral=0;

}


void PID_Circulation(float DeterCurr, float LastCirCurrent)
{
	static float  deviation=0, Contr_Varia=175;
        float  PID_Sum,proportion;
        

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
        
        
	Contr_Varia -= (Sv * PID_Sum);
        
        if(Contr_Varia<(float)MINFREQUENCY) Contr_Varia =(float)MINFREQUENCY;
        if(Contr_Varia>(float)MAXFREQUENCY) Contr_Varia =(float)MAXFREQUENCY;
        
        frequency = (uint32_t)(((float)FREQUENCY_MAGNIFY) * Contr_Varia);
        PWM_Handler(SET_FREQUENCY);
}

