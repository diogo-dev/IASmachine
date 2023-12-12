#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *x = (int *) malloc(10 * sizeof(int));
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
    }
    else{
        printf("Nao foi possivel alocar a memoria");
    }

    free(x);

    return 0;
}