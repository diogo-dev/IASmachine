#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int converter(char *string)
{
    int x = atoi(string);
    return x;
}

int main()
{
    char string[5] = {"10"};
    int x = converter(string);
    printf("%d\n\n", x);

    return 0;
}