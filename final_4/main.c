#include "stdint.h"
#include <stdlib.h>
#include <string.h>
#include "tm4c123gh6pm.h"

#define PA01 0x03
#define PD67 0xC0
#define maxlen 80

unsigned char latitude[] = { 0 };
unsigned char longitude[] = { 0 };

void init_UART0() {
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

    UART0_CTL_R &= ~UART_CTL_UARTEN;
    UART0_IBRD_R = 104;
    UART0_FBRD_R = 11;
    UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
    UART0_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

    GPIO_PORTA_AFSEL_R |= PA01;
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R &= ~0XFF) | (GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX);
    GPIO_PORTA_AMSEL_R &= ~PA01;
    GPIO_PORTA_DEN_R |= PA01;
}

void init_UART2() //UART2_INIT OF D6 "RX" & D7 "TX"
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R2;
    while ((SYSCTL_PRGPIO_R & SYSCTL_RCGCGPIO_R3) == 0);
    while ((SYSCTL_PRUART_R & SYSCTL_PRUART_R2) == 0);

    GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;
    GPIO_PORTD_CR_R = 0x80;

    UART2_CTL_R &= ~UART_CTL_UARTEN;
    UART2_IBRD_R = 104;
    UART2_FBRD_R = 11;
    UART2_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN); //0X00000070 MUST AFTER BRD CONFIGURE
    UART2_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN); //RE ENABLE THE UART MODULE

    GPIO_PORTD_AFSEL_R |= PD67;
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0X00FFFFFF) | (GPIO_PCTL_PD6_U2RX | GPIO_PCTL_PD7_U2TX);
    GPIO_PORTD_AMSEL_R &= ~PD67;
    GPIO_PORTD_DEN_R |= PD67;
}

char Read_data() {
    while ((UART2_FR_R & UART_FR_RXFE) == UART_FR_RXFE);
    return UART2_DR_R & 0xFF;
}

void Write_data(unsigned char data) {
    while ((UART0_FR_R & UART_FR_TXFF) == UART_FR_TXFF);
    UART0_DR_R = ((char)data);
}

void Read_command(unsigned char* str, uint8_t mexlen) {
    uint8_t i;
    char c;
    for (i = 0; i < maxlen; i++) {
        c = Read_data();
        if (c == '$')
            break;

        str[i] = c;
    }
    str[i] = '\0';
}

void print_command(unsigned char str[]) {
    uint8_t i = 0;
    Write_data('\n');
    while (str[i] != '\0') {
        Write_data(str[i]);
        i++;
    }
}

void GPRMC(unsigned char command[])
{

    uint8_t i, k = 1;
    const unsigned char check[6] = "GPRMC";

    for (i = 0; i < 5; i++) {
        if (!(command[i] == check[i])) {
            k = 0;
            break;
        }
    }
    if (!(command[16] == 'A')) {
        k = 0;
    }


    if (k) {
        print_command(command);

        i = 0;
        for (; i < 10; i++) {
            latitude[i] = command[18 + i];
        }
        latitude[i] = '\0';
        print_command(latitude);


        i = 0;
        for (; i < 11; i++) {
            longitude[i] = command[31 + i];
        }
        longitude[i] = '\0';
        print_command(longitude);
    }
}

double ascii_number(const char dimension[]) {
    char** example = NULL;
    return strtod(dimension, example);
}

int main() {
    unsigned char command[100] = { 0 };
    //char c;
    init_UART0();
    init_UART2();

    while (1)
    {
        //c = Read_data();
        //Write_data(c);
        Read_command(command, 100);
        //print_command(command);
        GPRMC(command);
        //print_command(longitude);
        //print_command(latitude);
    }
}

