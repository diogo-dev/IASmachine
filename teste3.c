#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum{
    vermelho,
    verde,
    amarelo
}sinal;


int converter(char *string)
{
    int x = atoi(string);
    return x;
}

void sinalVermelho(sinal *sinal)
{
    *sinal = vermelho;
}

void sinalAmarelo(sinal *sinal)
{
    *sinal = amarelo;
}

void sinalVerde(sinal *sinal)
{
    *sinal = verde;
}

int main()
{
    char string[5] = {"10"};
    int x = converter(string);
    printf("%d\n\n", x);

    double z;
    float w;
    printf("Quantidade de bytes de z: %ld\n\n", sizeof(z));
    printf("Quantidade de bytes de w: %ld\n\n", sizeof(w));

    sinal sinal;
    printf("Sinal: %d\n\n", sinal);
    sinalVermelho(&sinal);
    printf("Sinal vermelho: %d\n\n", sinal);
    sinalVerde(&sinal);
    printf("Sinal verde: %d\n\n", sinal);
    sinalAmarelo(&sinal);
    printf("Sinal amarelo: %d\n\n", sinal);

    return 0;
}