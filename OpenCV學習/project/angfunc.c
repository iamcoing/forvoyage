#include <stdio.h>
#include <math.h>

void print(double num)
{
    int a = 31, b = 60;
    float c = 17.625;
    printf("%f, %d\n", (float)a / (float)b, (int)c);
}

int main()
{
    __uint8_t arr[] = {15, 22, 64, 55, 78, 99};
    float theta;
    for(int i = 0; i < sizeof(arr)/ sizeof(arr[0]) - 1 ; i++)
        for(int j = sizeof(arr)/sizeof(arr[0]) - 1; j > 0 ;j--){
            theta = atan( (float)arr[i] / (float)arr[j] );
            printf("theta: %d\n", (int)theta);
        }
    print(theta);
    return 0;
}