#include "stdint.h"
#include "tm4c123gh6pm.h"
#define PA01 0x03
#define PE01 0x03
#define maxlen 50

void initUART0()
{
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
		
    UART0_CTL_R &= ~UART_CTL_UARTEN;
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    UART0_LCRH_R |= (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R |= (UART_CTL_RXE|UART_CTL_TXE|UART_CTL_UARTEN);

    GPIO_PORTA_AFSEL_R |= PA01;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R &= ~0XFF) | (GPIO_PCTL_PA0_U0RX|GPIO_PCTL_PA1_U0TX);
    GPIO_PORTA_DEN_R |= PA01;
    GPIO_PORTA_AMSEL_R &= ~PA01;
}

void initUART7 (){
		
		SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R7;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
		

    UART7_CTL_R &= ~UART_CTL_UARTEN;
    UART7_IBRD_R = 104;
    UART7_FBRD_R = 11
    UART7_LCRH_R = ( UART_LCRH_WLEN_8 | UART_LCRH_FEN); //0X00000070 MUST AFTER BRD CONFIGURE 
    UART7_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN); //RE ENABLE THE UART MODULE 

    GPIO_PORTE_AFSEL_R &= PE01;
    GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R & ~0XFF) | (GPIO_PCTL_PE0_U7RX|GPIO_PCTL_PE1_U7TX);
    GPIO_PORTE_DEN_R |= PE01;
    GPIO_PORTE_AMSEL_R &= ~PE01;
}

char Read_data ()
{
    while((UART7_FR_R & UART_FR_RXFE) == UART_FR_RXFE);
    return (uint8_t) UART7_DR_R & 0xFF;
}
void Write_data(uint8_t data){
    while ((UART0_FR_R & UART_FR_TXFF) == UART_FR_TXFF);
    UART0_DR_R = data;
}

void Read_command(char* str, uint8_t mexlen){
    uint8_t i;
    char c;
    for(i=0; i<50; i++){
        c = Read_data();
        if(c == '\n' || c == '\r') break;
                else str[i] = c;
        Write_data(c);
    }
}

void print_command(char *str){
    uint8_t i = 0;
    while (str[i] != '\0'){
        Write_data(str[i]);
        i++;
    }
}

int main (){
    char command [maxlen] = {0};
    uint32_t i;
    initUART0();
    initUART7();

    while(1){
        Read_command(command, maxlen);
        print_command(command);
        for(i = 0; i < 3100; i++);
    }
}