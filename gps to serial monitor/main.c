#include "stdint.h"
#include "tm4c123gh6pm.h"

#define PA01 0x03
#define PD67 0xC0
#define maxlen 100

void init_UART0(){
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
    
    UART0_CTL_R &= ~UART_CTL_UARTEN;
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R |= (UART_CTL_RXE|UART_CTL_TXE|UART_CTL_UARTEN);

    GPIO_PORTA_AFSEL_R |= PA01;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R &= ~0XFF) | (GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX);
    GPIO_PORTA_AMSEL_R &= ~PA01;  
    GPIO_PORTA_DEN_R |= PA01; 
}

void init_UART2() //UART2_INIT OF D6 "RX" & D7 "TX"
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R2;
    while((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R3) == 0);
    while((SYSCTL_PRUART_R & SYSCTL_PRUART_R2)==0);
    
    GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTD_CR_R = 0x80;

    UART2_CTL_R &= ~UART_CTL_UARTEN;
    UART2_IBRD_R = 104;
    UART2_FBRD_R = 11;
    UART2_LCRH_R = ( UART_LCRH_WLEN_8 | UART_LCRH_FEN); //0X00000070 MUST AFTER BRD CONFIGURE 
    UART2_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN); //RE ENABLE THE UART MODULE 

    GPIO_PORTD_AFSEL_R |= PD67;
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0X00FFFFFF) | ( GPIO_PCTL_PD6_U2RX | GPIO_PCTL_PD7_U2TX );
    GPIO_PORTD_AMSEL_R &= ~PD67;
    GPIO_PORTD_DEN_R |= PD67;
}

char Read_data (){
    while((UART2_FR_R & UART_FR_RXFE) == UART_FR_RXFE);
    return UART2_DR_R & 0xFF;
}

void Write_data(char data){
    while ((UART0_FR_R & UART_FR_TXFF) == UART_FR_TXFF);
    UART0_DR_R = ((char)data);
}

void Read_command(char* str , uint8_t mexlen){
    uint8_t i;
    char c;
    for(i = 0; i < maxlen; i++){
        c = Read_data();
        if(c == '$') 
          break;
        
        str[i] = c;
    }
     str[i] = '$';
}

void print_command(char str[]){
  uint8_t i = 0;
  Write_data('\n');
  while (str[i] != '$'){
    Write_data(str[i]);
    i++;
  }
}

int main(){
    char command [maxlen] = {0};
    char c;
    init_UART0();
    init_UART2();

    while (1)
    {
            // c = Read_data();
            // Write_data(c);
            Read_command(command, maxlen);
           print_command(command);
    }
}

