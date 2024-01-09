#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 40

int finding_address(char *str_lida, char *address)
{
    int i = 0;
    int j = 0;
    int k = 0;

    while (i < strlen(str_lida) && str_lida[i] != ')' )
    {
        if (str_lida[i] == '(')
        {
            k = i;
            while (str_lida[k + 1] != ')' && str_lida[k + 1] != ',')
            {
                address[j] = str_lida[k + 1];
                k++;
                j++;
            }
            i = k;
        }
        i++;
    }
    if (strcmp(address, "0") == 0)
    {
        return 0;
    }
    else
    {
        int i = atoi(address);
        return i;
    }
}

int main(int argc, char *argv[])
{
    FILE *arq;
    char str1[BUFFER_SIZE] = "0";
    char str2[BUFFER_SIZE] = "0";
    char linha_lida[BUFFER_SIZE];
    char linha_lida2[BUFFER_SIZE];
    int address1;
    int address2;
    long int palavra = 0;
    unsigned char op1 = 0b00001010; //10
    unsigned char op2 = 0b00000010; // 2

    //0000101000000000000000000010000000001010
    //42949681162

    if (argc == 5 && (strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-m") == 0))
    {
        //executar os comandos
        if ((arq = fopen(argv[2], "r")) == NULL)
        {
            printf("Erro ao abrir o aquivo!");
            exit(1);
        }

        int i = 0;
        while (fgets(linha_lida, BUFFER_SIZE, arq) != NULL)
        {
            linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida

            address1 = finding_address(linha_lida, str1);
            printf("Address1: %d\n", address1);
            if((fgets(linha_lida2, BUFFER_SIZE, arq) != NULL))
            {
                address2 = finding_address(linha_lida2, str2);
                printf("Address2: %d\n", address2);
                palavra = op1 << 12;
                printf("palavra: %ld\n", palavra);
                palavra = palavra | address1;
                printf("palavra: %ld\n", palavra);
                palavra = palavra << 8;
                printf("palavra: %ld\n", palavra);
                palavra = palavra | op2;
                printf("palavra: %ld\n", palavra);
                palavra = palavra << 12;
                printf("palavra: %ld\n", palavra);
                palavra = palavra | address2;
                printf("palavra: %ld\n", palavra);
            }
            i++;
        }

    }
    else{
        printf("Erro na linha de comando!");
    }

    return 0;
}