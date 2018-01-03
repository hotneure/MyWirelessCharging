#ifndef _QI_H
#define _QI_H

//Power Transmitter timing in the ping phase
#define PING_TIME               65
#define FIRST_PACKET_TIMEOUT    20

//Power Transmitter timing in the identification & configuration phase
#define NEXT_TIME          21
#define MAX_PACKET_TIME    170

//Power Transmitter timing in the power transfer phase
#define MAX_POWER_TIME                   24000 
#define MAX_CONTROL_TIME                 1800 

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
    unsigned char  Header;
    unsigned char  Message[10];
    unsigned char  CheckSum;
    unsigned char  Flag;
}QI_DATA_PACKET_TYPEDEF;

extern unsigned char wpc_qi_pwm_freq;
extern QI_POWER_TRANSFER_PHASE_TYPEDEF WPCQi_Phase;

void WPC_QI(void);

#endif