#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int x;
    int y;
    int z;

    x = 0b10101010111111111110;
    y = x & 0b111111111111;
    printf("y: %d\n\n", y);
    x >>= 12;
    printf("x: %d\n\n", x);
    z = x;
    printf("z: %d\n\n", z);

    x >>= 1;
    printf("%d\n\n", x);

    x <<= 1;
    printf("%d\n\n", x);
    
    //y = vale15(x) & 0b11;
    printf("y:%d\n\n", y); // y = 3 (ok)

    //y = vale15(x) >> 1;
    printf("y:%d\n\n", y); // y = 7 (ok)

    y = x >> 3;
    printf("y:%d\n\n", y);

    y = x >> 4;
    printf("y:%d\n\n", y);

    x = x & 0b011;
    if (x == 15 || 3)
    {
        printf("%d\n\n", x);
    }

    return 0;
}