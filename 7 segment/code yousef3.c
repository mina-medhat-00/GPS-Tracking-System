#include<stdio.h>
int main()
{
    int I,H,T,T0,U0;
    printf("enter the value ");
    scanf("%d",&I);
    H=I/100;   // what we need for hundred
    T0=I-100*H;
    T=T0/10;   // ten
    U0=T0-10*T;
    printf("the first value is H=%d , the second value is T=%d , the third value is U0=%d",H,T,U0);

}
