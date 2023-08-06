#include "stdio.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"
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
  GPIO_PORTA_DATA_R =0x80;//port a bit 7 enable
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

void LCD_init(void)
 {
   SYSCTL_RCGCGPIO_R =0X03;//ENABLE PORT A AND PORT B
	 while ((SYSCTL_PRGPIO_R&0x03)==0){};
   GPIO_PORTA_DIR_R|=0xE0;
   GPIO_PORTA_DEN_R|=0xE0;
   GPIO_PORTB_DIR_R=0xFF;
	 GPIO_PORTB_DEN_R|=0xFF; 
   sys_delay(50);
   LCD_command(0x30);
   sys_delay(20);
   LCD_command(0x30);
   sys_delay(20);
   LCD_command(0x30); // screen wake up
	 sys_delay(20);
   LCD_command(0x38); // set 8-bit data, 2-line, 5x7 font
   sys_delay(10);		 
   LCD_command(0x06); // move cursor right
	 sys_delay(10);
   LCD_command(0x01); // clear screen, move cursor to home 
   sys_delay(10); 
   LCD_command(0x0F); // turn on display, cursor blinking
		sys_delay(10); 
}
 
 int main(){
	 unsigned char data_s=0x41;
     LCD_init();
     while(1){
     LCD_command(1); //clear display
     LCD_command(0x80);//cursor location
     sys_delay(500);
		 LCD_data(data_s);
		 LCD_command(0x06);//move cursor
			sys_delay(500); 
			 
			 
    
     }
 }