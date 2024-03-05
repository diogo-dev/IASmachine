#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum{
    NENHUM,
    LOAD_MQ,
    LOAD_MQ_MX,
    STOR_MX,
    LOAD_MX,
    LOAD_MINUS_MX,
    LOAD_PIPE_MX,
    LOAD_MINUS_PIPE_MX,
    JUMP_MX_0_19,
    JUMP_MX_20_39,
    JUMPC_MX_0_19,
    JUMPC_MX_20_39,
    ADD_MX,
    ADD_PIPE_MX,
    SUB_MX,
    SUB_PIPE_MX,
    MUL_MX,
    DIV_MX,
    LSH,
    RSH,
    STOR_MX_8_19,
    STOR_MX_28_39,
    EXIT
}instrucoesIAS;

void atribuir_X(int *x, instrucoesIAS i)
{
    if (i == LOAD_MINUS_MX || i == LOAD_MQ || i == LOAD_MINUS_PIPE_MX || i == LOAD_MQ_MX)
    {
        *x = 1;
    }
    else if (i == JUMP_MX_0_19 || i == JUMP_MX_20_39 || i == JUMPC_MX_0_19 || i == JUMPC_MX_20_39)
    {
        *x = 2;
    }
    else if (i == ADD_MX || i == ADD_PIPE_MX)
    {
        *x = 3;
    }
    else if (i == SUB_MX || i == SUB_PIPE_MX)
    {
        *x = 4;
    }
    else{
        *x = 5;
    }
}

int main()
{
    int x;
    int y;

    instrucoesIAS i;
    i = SUB_MX;
    atribuir_X(&x, i);
    printf("%d\n\n", x);

    x = 0b1111;

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