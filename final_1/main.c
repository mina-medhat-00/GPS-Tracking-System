#include "stdint.h"
#include <stdlib.h>
#include <string.h>
#include "tm4c123gh6pm.h"

#define PA01 0x03
#define PD6 0x40
#define maxlen 100


unsigned char latitude[15];
unsigned char longitude[15];
unsigned char command[maxlen] = {0};

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
    unsigned char i;
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
    unsigned char i = 0;
    Write_data('\n');
    while (str[i] != '$')
    {
        Write_data(str[i]);
        i++;
    }
}

void GPGGA(unsigned char command[])
{
    uint32_t i = 0, k = 0;
    unsigned char GPS[100];
    unsigned char check[6];
    unsigned char* p1, * p2;
    int index1 = 16, index2 = 16, index3, index4;
    int latitude_length, longitude_length;
    while (command[k] != '\0')
    {
        GPS[k] = command[k];
			  k++;
    }
    GPS[k+1] = '\0';
		print_command(GPS);
    memcpy(check, &GPS[0], 5);
    check[5] = '\0';
    if (strcmp(check, "GPGGA") == 0)
    {
        for (i = 0; i <= 100; i++)
        {
					//if( GPS[i] == '\0')
					//break;
					
            while (GPS[index2] != ',')
            {
                index2++;
            }
            index2--;
            latitude_length = (index2 - index1) + 1;

            index3 = index2 + 4;
            index4 = index2 + 4;
            while (GPS[index4] != ',')
            {
                index4++;
            }
            index4--;
            longitude_length = (index4 - index3) + 1;
        }
        memcpy(latitude, &GPS[index1], latitude_length);
        latitude[latitude_length + 1] = '\0';

        memcpy(longitude, &GPS[index3], longitude_length);
        longitude[longitude_length + 1] = '\0';
    }
}

int main()
{
    unsigned char c;
    init_UART0();
    init_UART2();

    while (1)
    {
        //c = Read_data();
        //Write_data(c);
        Read_command(command, maxlen);
        print_command(command);
        GPGGA(command);
        print_command(longitude);
        print_command(latitude);
    }
}
