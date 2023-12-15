#include <stdio.h>
#include <string.h>

int main()
{
    char str[50] = "ola meus amigos da glob";
    char x = 'o';

    strncat(str, &x, 1);

    printf("%s\n", str);

    return 0;
}