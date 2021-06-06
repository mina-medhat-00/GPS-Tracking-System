#include "stdint.h"
#include "tm4c123gh6pm.h"
#include "math.h"
#define R 6371000
#define pi 3.141592654
void init (){
	SYSCTL_RCGCGPIO_R |= 0x20;
	while ((SYSCTL_PRGPIO_R&0x20) == 0)
{}
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x01;   //enable pf321
	GPIO_PORTF_DIR_R |= 0x0E;  //set direction pf04 input, pf123 output
	GPIO_PORTF_DEN_R |= 0x1F;
	GPIO_PORTF_AMSEL_R = ~0x1F;
	GPIO_PORTF_AFSEL_R = ~0x1F;
	GPIO_PORTF_PCTL_R = ~0x1F;
	GPIO_PORTF_PUR_R |= 0x11;
	GPIO_PORTF_DATA_R &= ~0x0E; //initialize output pins to be off
}
double distance(long double x)
{
    static long double z = 0;
    z+= x;
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
    x = rad(lon2 - lon1) * cos(rad(lat1 + lat2) / 2);
    y = rad(lat1 - lat2);
    d = sqrt(x * x + y * y) * R;
    return d;
}

int main (){
	init();
	
	while(1){
		if((GPIO_PORTF_DATA_R & 0x11) == 0x10){
			GPIO_PORTF_DATA_R &= ~0x0C;
			GPIO_PORTF_DATA_R |= 0x02;
		}
		else if ((GPIO_PORTF_DATA_R & 0x11) == 0x01){
			GPIO_PORTF_DATA_R &= ~0x0A;
			GPIO_PORTF_DATA_R |= 0x04;
		}
		else if ((GPIO_PORTF_DATA_R & 0x11) == 0x00){
			GPIO_PORTF_DATA_R &= ~0x06;
			GPIO_PORTF_DATA_R |= 0x08;
		}
		else {
			GPIO_PORTF_DATA_R &= ~0x0E;
		}
	}
}
