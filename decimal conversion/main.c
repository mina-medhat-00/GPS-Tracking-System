#include "stdio.h"
#include "stdint.h"
double decimel(double lat)
{
	double j = lat / 100;
	int degree = j;
	double minitues = (j - (double)degree) * 100;
	double degree2 = minitues / 60;
	double degree_final = (double)degree + degree2;
	return degree_final;
}

int main()
{
	double w=0,p=0;
	scanf("%lf",&w);
	p=decimel(w);
	printf("%lf\n", p);
}

