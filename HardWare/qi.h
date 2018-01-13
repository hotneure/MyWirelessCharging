#ifndef _QI_H
#define _QI_H

#include "stm8s.h"

//Power Transmitter timing in the ping phase
#define PING_TIME_OUT               8125                //TIM2，128分频，65ms
#define PING_TIME                   4375                //延时到100ms
#define PING_DELAY                  62500               //TIM2，128分频，500ms
#define FIRST_PACKET_TIMEOUT        20

//Power Transmitter timing in the identification & configuration phase
#define NEXT_TIME          21
#define MAX_PACKET_TIME    170

//Power Transmitter timing in the power transfer phase
#define MAX_POWER_TIME                   24000 
#define MAX_CONTROL_TIME                 1800 

//data packet type
#define STRENGTH_PACKET                   0x01
#define END_POWER_PACKET                  0x02
#define CONTROL_ERROR_PACKET              0x03
#define RECEIVED_P0WER_PACKET             0x04
#define CONFIG_PACKET                     0x51
#define ID_PACKET                         0x71

#define HIGH_VOLTAGE_FRE                  160000
#define LOW_VOLTAGE_FRE                   173000

#define MINFREQUENCY                      110
#define MAXFREQUENCY                      200
#define FREQUENCY_MAGNIFY                 1000

//Voltage check
#define LARGE_POWER                       0x30

typedef enum
{
    Idle_Phase = 0,
    Selection_Phase,
    Ping_Phase,
    Identify_Config_Phase,
    Negotiation_Phase,
    Calibration_Phase,
    Power_Transfer_Phase,
    Renegotiation_Phase,
    Charge_Finish_Phase,
    Otp_Phase
}QI_POWER_TRANSFER_PHASE_TYPEDEF;

typedef enum
{
    Rx_Ready=0,
    Rx_Preamble,
    Rx_Byte
}QI_RX_DATA_STATE_TYPEDEF;

typedef struct
{
    volatile u8  Header;
    u8  Message[10];
    volatile u8  CheckSum;
    volatile u8  Flag;
}QI_DATA_PACKET_TYPEDEF;

extern QI_POWER_TRANSFER_PHASE_TYPEDEF      WPCQi_Phase;
extern QI_DATA_PACKET_TYPEDEF               Qi_Packet;
extern volatile u8 PWM_State;
extern u8 Large_Power;
extern u32 frequency;

void WPC_QI(void);

#endif