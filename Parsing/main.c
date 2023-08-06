#include <stdio.h>
#include <stdlib.h>
#include<string.h>

void GPGGA()
{
    char GPS [100];
    char check[6];
    double latitude_d,longitude_d;
    char *p1,*p2;
    int index1=16,index2=16,index3,index4;
    int latitude_length,longitude_length;
    scanf("%s",GPS);
    memcpy(check, &GPS[0], 5);
    check[5] = '\0';
    if(strcmp(check,"GPGGA")==0)
    {

        for(int i=0; i<=strlen(GPS); i++)
        {
            while(GPS[index2]!=',')
            {
                index2++;
            }
            index2--;
            latitude_length=(index2-index1)+1;

            index3=index2+4;
            index4=index2+4;
            while(GPS[index4]!=',')
            {
                index4++;
            }
            index4--;
            longitude_length=(index4-index3)+1;
            }
        char latitude[15];
        memcpy(latitude, &GPS[index1], latitude_length);
        latitude[latitude_length+1] = '\0';
        printf("%s\n",latitude);

        char longitude[15];
        memcpy(longitude, &GPS[index3], longitude_length);
        longitude[longitude_length+1] = '\0';
        printf("%s\n",longitude);
        latitude_d=strtod(latitude,&p1);
        longitude_d=strtod(longitude,&p2);
        printf("%f\n",latitude_d);
        printf("%f\n",longitude_d);
    }
}

void GPRMC()
{
    char GPS [100];
    char check[6];
    scanf("%s",GPS);
    int index1=18,index2=18,index3,index4;
    int latitude_length,longitude_length;
    double latitude_d,longitude_d;
    char *p1,*p2;
    memcpy(check, &GPS[0], 5);
    check[5] = '\0';
    if(strcmp(check,"GPRMC")==0)
    {
        if(GPS[16]=='A')
        {
        for(int i=0; i<=strlen(GPS); i++)
        {
            while(GPS[index2]!=',')
            {
                index2++;
            }
            index2--;
            latitude_length=(index2-index1)+1;

            index3=index2+4;
            index4=index2+4;
            while(GPS[index4]!=',')
            {
                index4++;
            }
            index4--;
            longitude_length=(index4-index3)+1;
        }
        char latitude[15];
        memcpy(latitude, &GPS[index1], latitude_length);
        latitude[latitude_length+1] = '\0';
        printf("%s\n",latitude);

        char longitude[15];
        memcpy(longitude, &GPS[index3], longitude_length);
        longitude[longitude_length+1] = '\0';
        printf("%s\n",longitude);

        latitude_d=strtod(latitude,&p1);
        longitude_d=strtod(longitude,&p2);
        printf("%f\n",latitude_d);
        printf("%f\n",longitude_d);
        }
    }
}

int main()
{
//    GPGGA();
//    GPRMC();
    return 0;
}
