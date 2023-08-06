#include "stdint.h"
#include <stdlib.h>
#include <string.h>
#include "tm4c123gh6pm.h"

#define PA01 0x03
#define PD6 0x40
#define maxlen 50
#define PA567 0xE0
#define PB 0xFF


unsigned char latitude[15] = {0};
unsigned char longitude[15] = {0};
unsigned char command[maxlen] = {0};


void sys_delay(uint32_t n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3180; j++)
        {
        }
}

void delay_micro(uint32_t n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3; j++)
        {
        }
}

void LCD_command(uint8_t command)
{
    GPIO_PORTA_DATA_R = 0;
    GPIO_PORTB_DATA_R = command;
    GPIO_PORTA_DATA_R = 0x80; //port a bit 7 enable
    sys_delay(1);
    GPIO_PORTA_DATA_R = 0;
    if (command < 4)
        sys_delay(5);

    else
        delay_micro(37);

}

void LCD_data(uint8_t data)
{
    GPIO_PORTA_DATA_R = 0x20; //port a bit 5
    GPIO_PORTB_DATA_R = data;
    GPIO_PORTA_DATA_R |= 0x80; //port A bit 5, 7    RS=1   enable=1
    GPIO_PORTA_DATA_R = 0;

}

void LCD_init(void)
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; //ENABLE PORT A AND PORT B
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
    sys_delay(1);

    GPIO_PORTA_AFSEL_R &= 0;
    GPIO_PORTA_AMSEL_R &= 0;
    GPIO_PORTA_PCTL_R &= 0;

    GPIO_PORTB_AFSEL_R &= 0;
    GPIO_PORTB_AMSEL_R &= 0;
    GPIO_PORTB_PCTL_R &= 0;

    GPIO_PORTA_DIR_R |= PA567;
    GPIO_PORTA_DEN_R |= PA567;
    GPIO_PORTB_DIR_R |= PB;
    GPIO_PORTB_DEN_R |= PB;


    LCD_command(0x38); // set 8-bit data, 2-line, 5x7 font

    LCD_command(0x06); // move cursor right

    LCD_command(0x0F); // turn on display, cursor blinking

    LCD_command(0x01);

}

void init_UART0()
{
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

    while ((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R0) == 0)
        ;
    while ((SYSCTL_PRUART_R & SYSCTL_PRUART_R0) == 0)
        ;

    UART0_CTL_R &= ~UART_CTL_UARTEN;
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R |= (UART_CTL_TXE | UART_CTL_UARTEN);

    GPIO_PORTA_AFSEL_R |= PA01;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R &= ~0XFF) | GPIO_PCTL_PA1_U0TX;
    GPIO_PORTA_AMSEL_R &= ~PA01;
    GPIO_PORTA_DEN_R |= PA01;
}

void init_UART2() //UART2_INIT OF D6 "RX" & D7 "TX"
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R2;
    while ((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R3) == 0)
        ;
    while ((SYSCTL_PRUART_R & SYSCTL_PRUART_R2) == 0)
        ;

    UART2_CTL_R &= ~UART_CTL_UARTEN;
    UART2_IBRD_R = 104;
    UART2_FBRD_R = 11;
    UART2_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN); //0X00000070 MUST AFTER BRD CONFIGURE
    UART2_CTL_R |= (UART_CTL_RXE | UART_CTL_UARTEN);   //RE ENABLE THE UART MODULE

    GPIO_PORTD_AFSEL_R |= PD6;
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0XF0FFFFFF) | (GPIO_PCTL_PD6_U2RX);
    GPIO_PORTD_AMSEL_R &= ~PD6;
    GPIO_PORTD_DEN_R |= PD6;
}

unsigned char Read_data()
{
    while ((UART2_FR_R & UART_FR_RXFE) == UART_FR_RXFE)
        ;
    return UART2_DR_R & 0xFF;
}

void Write_data(unsigned char data)
{
    while ((UART0_FR_R & UART_FR_TXFF) == UART_FR_TXFF)
        ;
    UART0_DR_R = ((unsigned char)data);
}

void Read_command(unsigned char *str, unsigned char mexlen)
{
    uint8_t i;
    unsigned char c;
    for (i = 0; i < maxlen; i++)
    {
        c = Read_data();
        if (c == '$')
            break;

        str[i] = c;
    }
    str[i] = '$';
}

void print_command(unsigned char str[])
{
    uint8_t i = 0;
    Write_data('\n');
    while (str[i] != '$')
    {
        Write_data(str[i]);
        i++;
    }
}

void GPGGA()
{
	
  uint16_t i, k = 1;
	const unsigned char check[6] = "GPRMC\0";
	
	for (i = 0; i < 5; i++) {
		if (!(command[i] == check[i])) {
			k = 0;
			break;
		}
	}

	if (k) {
		for (i = 0; i < 11; i++) {
			latitude[i] = command[18 + i];
		}
		
		for (i = 0; i < 12; i++) {
			longitude[i] = command[31 + i];
		}
		
	}
	return;
}

int main()
{ 
	uint32_t i = 0;
	uint8_t c [] = "Hi there";
	uint8_t d [] ="";
  init_UART0();
  init_UART2();
	LCD_init();
				//c = Read_data();
        //Write_data(c);
        Read_command(command, maxlen);
        print_command(command);
        GPGGA();
        print_command(longitude);
        print_command(latitude);
    
    while (1)
    {
        LCD_command(0x01); //clear display
        LCD_command(0x80); //cursor location
        sys_delay(500);
        while (1)
        {
					LCD_command(0x01); //clear display
					LCD_command(0x80); //cursor location
					
					Read_command(command, maxlen);
          print_command(command);
          GPGGA();
					i=0;
					while(latitude[i] != '\0'){
					 LCD_data(latitude[i]);
					 i++;
					 sys_delay(100);
					}
					//LCD_command(0x01); //clear display
          LCD_command(0xC0); //cursor location
					i = 0;
					while (1)
        {
					while(longitude[i] != '\0'){
					 LCD_data(longitude[i]);
					 i++;
					 sys_delay(100);
					}
        }
    }
	}
    

    
}