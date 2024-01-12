#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *x = (int *) malloc(10 * sizeof(int));
    int *temp = x; //armazena o ponto inicial de da memoria/vetor
    //char *m = (char *) malloc(4096 * 5 * sizeof(char));

    if(x)
    {
        printf("Memoria alocada com sucesso\n");

        printf("\nx: %x", x);
        printf("\nx: %d", *x);
        *x = 50;
        printf("\nx: %d", *x);
        x += 1;
        printf("\nx: %x", x);
        printf("\nx: %d", *x);
        *x = 100;
        printf("\nx: %d", *x);
        x -= 1;
        printf("\nx: %x", x);
        printf("\nx: %d", *x);
        x += 1;
        printf("\nx: %x", x);
        printf("\nx: %d", *x);
        x = temp;
        printf("\nx: %x", x);
        printf("\nx: %d\n", *x);
    }
    else{
        printf("Nao foi possivel alocar a memoria");
    }

    free(x);

    return 0;
}