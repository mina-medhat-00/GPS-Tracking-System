#include "stdint.h"
#include <stdlib.h>
#include <string.h>
#include "tm4c123gh6pm.h"
#include "math.h"
#include <TM4C123.h>

#define pi 3.141592654
#define R 6371000
#define PA01 0x03
#define PD67 0xC0
#define maxlen 80
#define delay1 100
#define PA567 0xFF
#define PB 0xFF



unsigned char latitude[] = { 0 };
unsigned char longitude[] = { 0 };
int num_array[10] = { 0x3F,0x06,0x05B,0X4F,0X66,0X6D,0X7D,0X07,0X7F,0X6F };

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

double decimel(double lat)
{
    double j = lat / 100;
    int degree = j;
    double minitues = (j - (double)degree) * 100;
    double degree2 = minitues / 60;
    double degree_final = (double)degree + degree2;
    return degree_final;
}

void sys_delay(uint32_t time)
{
    //enter time in milli second
    uint32_t count;
    count = time / (62.5 * 1e-6);
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = count - 1;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 5;
    while ((NVIC_ST_CTRL_R & 0x10000) == 0) {};

}

void lot_delay(int n)
{
    int i = 0;
    for (; i < n; i++) {
        sys_delay(1000);
    }
}


double distance(double r)
{
    static double z = 0;
    z += r;
    return z;
}
double rad(double w)
{
    double v = 0;
    v = w * pi / 180;
    return v;
}

double gps_distance(double lat1, double lon1, double lat2, double lon2) {
    double x, y, d;
    x = (lon2 - lon1) * cos(rad(lat1 + lat2) / 2);
    y = lat1 + lat2;
    d = sqrt(x * x + y * y) * R;
    return d;
}

void init_display()

{
    SYSCTL_RCGCGPIO_R |= 0x02;   //activate port B  we can decide the port later 
    while ((SYSCTL_PRGPIO_R &= 0x02) == 0) {};
    GPIO_PORTB_DIR_R |= 0XFF;
    GPIO_PORTB_DEN_R |= 0XFF;
    GPIO_PORTB_AMSEL_R &= 0;
    GPIO_PORTB_AFSEL_R &= 0;
    GPIO_PORTB_PCTL_R &= 0;
    GPIO_PORTB_DATA_R = 0xFF;

    SYSCTL_RCGCGPIO_R |= 0x01;   //activate port A we can decide the port later 
    while ((SYSCTL_PRGPIO_R &= 0x01) == 0) {};
    GPIO_PORTA_DIR_R |= 0XFF;
    GPIO_PORTA_DEN_R |= 0XFF;
    GPIO_PORTA_AMSEL_R &= 0;
    GPIO_PORTA_AFSEL_R &= 0;
    GPIO_PORTA_PCTL_R &= 0;

}
void display_7segment(uint8_t u, uint8_t t, uint8_t h) {
    uint16_t x = 0;
    uint32_t count;
    count = 500 / (62.5 * 1e-6);
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_RELOAD_R = count - 1;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_CTRL_R = 5;

    while ((NVIC_ST_CTRL_R & 0x10000) == 0) {

        GPIO_PORTB_DATA_R = num_array[u];
        GPIO_PORTA_DATA_R = ~0x10;
        for (x = 0; x < 1000; x++) {}

        GPIO_PORTB_DATA_R = num_array[t];
        GPIO_PORTA_DATA_R = ~0x20;
        for (x = 0; x < 1000; x++) {}

        GPIO_PORTB_DATA_R = num_array[h];
        GPIO_PORTA_DATA_R = ~0x40;
        for (x = 0; x < 1000; x++) {}
    };
}

void delay_micro(uint8_t n)
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
    sys_delay(5);
    GPIO_PORTA_DATA_R |= 0x80; //port A bit 5, 7    RS=1   enable=1
    sys_delay(5);
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
    sys_delay(5);
    LCD_command(0x30);
    sys_delay(50);
    LCD_command(0x38); // set 8-bit data, 2-line, 5x7 font
    sys_delay(5);
    LCD_command(0x06); // move cursor right
    sys_delay(5);
    LCD_command(0x0F); // turn on display, cursor blinking
    sys_delay(5);
    LCD_command(0x01);
    sys_delay(5);
}



int main() {
    unsigned char command[100] = { 0 };
    double longi1 = 0;
    double lat1 = 0;
    double longi2 = 0;
    double lat2 = 0;
    double distances = 0;
    double final_distance = 0;
    uint8_t x = 1;
    uint8_t H = 0;
    uint8_t T = 0;
    uint8_t T0 = 0;
    uint8_t U0 = 0;
    uint8_t i = 0;
    //char c;
    init_UART0();
    init_UART2();
    init_display();
    LCD_init();
    SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));

    while (1)
    {
        double longi = 0;
        double lat = 0;
        unsigned char c[3] = "0";
        //c = Read_data();
        //Write_data(c);
        Read_command(command, maxlen);
        //print_command(command);
        GPRMC(command);
        //print_command(longitude);
        //print_command(latitude);

        longi = ascii_number((const char*)longitude);
        lat = ascii_number((const char*)latitude);
        longi = decimel(longi);
        lat = decimel(lat);
        if (x) {
            lat1 = rad(lat);
            longi1 = rad(longi);
            x = !x;
        }
        else {
            lat2 = lat;
            longi2 = rad(longi);
            x = !x;
        }
        distances = gps_distance(lat1, longi1, lat2, longi2);
        final_distance = distance(distances);
        H = final_distance / 100;   // what we need for hundred
        T0 = final_distance - 100 * H;
        T = T0 / 10;   // ten
        U0 = T0 - 10 * T;
        display_7segment(H, T, U0);
        LCD_command(0x01); //clear display
        LCD_command(0x80); //cursor location
        c[0] = '0' + H;
        c[1] = '0' + T;
        c[2] = '0' + U0;
        while (c[i] != '\0') {
            LCD_data(c[i]);
            i++;
            sys_delay(100);
        }
        sys_delay(500);


    }
}

