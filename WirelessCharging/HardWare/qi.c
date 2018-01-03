#include "qi.h"
#include "pid.h"


void WPC_QI()
{
    switch(WPCQi_Phase)
    {
        case Ping_Phase:
          //开始ping，开始65ms定时，判断是否接收到信号强度包，没有接收到则跳出（延时20），有接收到阶段设为配置阶段 
        case Identify_Config_Phase:
          //
        case Power_Transfer_Phase:
          
    }
}
