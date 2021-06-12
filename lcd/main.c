#include "stdio.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"
 unsigned char data_s=0;
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

void LCD_command(unsigned char command)
{ 
	GPIO_PORTA_DATA_R =0;
	GPIO_PORTB_DATA_R=command;
  GPIO_PORTA_DATA_R =0x80;//port a bit 7,5 enable
	sys_delay(5);
	GPIO_PORTA_DATA_R=0;
	sys_delay(5);
}

void LCD_data(unsigned char data)
{
	GPIO_PORTA_DATA_R=0x20;//port a bit 5
	GPIO_PORTB_DATA_R=data;
	GPIO_PORTA_DATA_R =0xA0;
	sys_delay(5);
	GPIO_PORTA_DATA_R=0;
	sys_delay(5);
}
int main(){}
	

