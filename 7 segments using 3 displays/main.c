#include "stdint.h"
#include "tm4c123gh6pm.h"

void init (){
      SYSCTL_RCGCGPIO_R |= 0x02;   //activate port B  we can decide the port later 
      while ((SYSCTL_PRGPIO_R &= 0x02)==0) {};
      GPIO_PORTB_DIR_R |= 0XFF;
      GPIO_PORTB_DEN_R |= 0XFF;
      GPIO_PORTB_AMSEL_R &= 0;
      GPIO_PORTB_AFSEL_R &= 0;
      GPIO_PORTB_PCTL_R &= 0;
			GPIO_PORTB_DATA_R = 0xFF;
				
			SYSCTL_RCGCGPIO_R |= 0x01;   //activate port A we can decide the port later 
      while ((SYSCTL_PRGPIO_R &= 0x01)==0) {};
      GPIO_PORTA_DIR_R |= 0XFF;
      GPIO_PORTA_DEN_R |= 0XFF;
      GPIO_PORTA_AMSEL_R &= 0;
      GPIO_PORTA_AFSEL_R &= 0;
      GPIO_PORTA_PCTL_R &= 0;
				
			
}
uint16_t num_array[10] = { 0x3F,0x06,0x5B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F };

//void Num_Write(int num){
	
  //GPIO_PORTB_DATA_R = num_array[num];
//}
//int i = 0;

void display(int u, int t, int h);
int main (){
  init();

	while(1) {
  display (2, 3, 4);
	display (3, 4, 5);
	display (4, 5, 6);
	}
	
}
  int x = 10;
	int y = 10;
	int z = 10;
void display (int u, int t, int h){
	
	  uint32_t count;
    count = 500/(62.5*1e-6);
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = count-1;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 5;
	
    while((NVIC_ST_CTRL_R&0x10000) == 0){
			
		GPIO_PORTB_DATA_R = num_array[u];
		GPIO_PORTA_DATA_R = ~0x10;
		for (x=0; x < 1000; x++){}
			
		GPIO_PORTB_DATA_R = num_array[t];
		GPIO_PORTA_DATA_R = ~0x20;
		for (x=0; x < 1000; x++){}
			
		GPIO_PORTB_DATA_R = num_array[h];
		GPIO_PORTA_DATA_R = ~0x40;
		for (x=0; x < 1000; x++){}
		};
}
