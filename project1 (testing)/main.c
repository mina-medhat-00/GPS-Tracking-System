#include "stdint.h"
#include "stdio.h"
#include "C:/Keil/EE319Kware/inc/tm4c123gh6pm.h"
void SystemInit(){}
void sys_delay(uint32_t time)
{
    //enter time in milli second
    uint32_t count;
    count=time/(62.5*1e-6);
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R=count-1;
    NVIC_ST_CURRENT_R=0;
    NVIC_ST_CTRL_R=5;
    while((NVIC_ST_CTRL_R&0x10000)==0){};

}

void lot_delay(int n)
    {
        int i=0;
    for (;i<n;i++){
        sys_delay(1000);
    }

}
int main(){}
	
	