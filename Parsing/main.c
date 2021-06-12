#include <stdio.h>
#include <stdlib.h>
#include<string.h>
//GPRMC,212638.00,A,4567.347,N,03114.39714328795497,E,0.733,,090621,,,A*7F
int main()
{
    char GPS [100];
    scanf("%s",GPS);
    char check[6];
    memcpy(check, &GPS[0], 5);
    check[5] = '\0';
    int index1=18,index2=18,index3,index4;
    int latitude_length,longitude_length;
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
            char latitude[latitude_length];
            memcpy(latitude, &GPS[index1], latitude_length);
            latitude[latitude_length] = '\0';
            printf("%s\n",latitude);

            char longitude[longitude_length];
            memcpy(longitude, &GPS[index3], longitude_length);
            longitude[longitude_length] = '\0';
            printf("%s\n",longitude);

        }
    }
    return 0;
}
