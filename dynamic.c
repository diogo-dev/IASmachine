/*      Trabalho de Arquitetura 1
Desenvolver um simulador do computador IAS

Alunos: Diogo Felipe Soares da Silva    RA:124771
        Arthur Henrique Bando Ueda      RA:129406
        Gustavo Alves Glatz             RA:128592 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 40
    
double potencia(int base, int expoente) {
    if(expoente == 0)
        return 1;
    else
        return (base * potencia(base, expoente-1));
}

long long int binary_decimal(char *palavra)
{
    long long int dec = 0;
    int i = 0;
    int s;
    s = strlen(palavra);
    while( s-- ) {
        if( palavra[s] == '0' || palavra[s] == '1' ) {
            dec = dec + potencia(2, i++) * (palavra[s] - '0');
        }
    };
    return dec;
}

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

void finding_opcode(char *str_lida, unsigned char *opcode) 
{

    const char delimitador_inicial[2] = "(";
    const char delimitador_final[2] = ")";
    const char delimitador_jump[2] = ",";
    char str_lida_backup[BUFFER_SIZE];
    char str_lida_backup2[BUFFER_SIZE];
    char str_lida_stor[BUFFER_SIZE];

    char *token;
    char *token_stor;

    strcpy(str_lida_backup, str_lida);
    strcpy(str_lida_stor, str_lida);
    strcpy(str_lida_backup2,str_lida);
    token = strtok(str_lida,delimitador_inicial);

    if (strcmp(str_lida, "LOAD-MQ") == 0) 
    {
        *opcode = 0b00001010;
    }
    else if (strcmp(str_lida, "LOAD-MQ,M") == 0) 
    {
        *opcode = 0b00001001;
    }
    else if (strcmp(str_lida, "STOR-M") == 0) 
    {
        token = strtok(str_lida_backup,delimitador_jump);
        if(strcmp(token,str_lida_backup2) == 0)
        {
            *opcode = 0b00100001;
        }
        else 
        {
            token_stor = strtok(str_lida_stor, delimitador_jump);
            token_stor = strtok(NULL, delimitador_final);
            if(strcmp(token_stor,"8:19") == 0) 
            {
                *opcode = 0b00010010;
            }
            else if(strcmp(token_stor,"28:39") == 0)
            {
                *opcode = 0b00010011;
            }
        }
    }
    else if (strcmp(str_lida, "LOAD-M") == 0) 
    {
        *opcode = 0b00000001;
    }
    else if (strcmp(str_lida, "LOAD--M") == 0) 
    {
        *opcode = 0b00000010;
    }
    else if (strcmp(str_lida, "LOAD-|M") == 0) 
    {
        *opcode = 0b00000011;
    }
    else if (strcmp(str_lida, "LOAD--|M") == 0) 
    {
        *opcode = 0b00000100;
    }
    else if (strcmp(str_lida, "JUMP-M") == 0) 
    {
        token = strtok(str_lida_backup, delimitador_jump);
        token = strtok(NULL, delimitador_final);
        if(strcmp(token,"0:19") == 0) 
        {
            *opcode = 0b00001101;
        }
        else if(strcmp(token,"20:39") == 0)
        {
            *opcode = 0b00001110;
        }
    }
    else if (strcmp(str_lida, "JUMP+-M") == 0) 
    {
        token = strtok(str_lida_backup, delimitador_jump);
        token = strtok(NULL, delimitador_final);
        if(strcmp(token,"0:19") == 0) 
        {
            *opcode = 0b00001111;
        }
        else if(strcmp(token,"20:39") == 0) 
        {
            *opcode = 0b00010000;
        }
    }
    else if (strcmp(str_lida, "ADD-M") == 0) 
    {
        *opcode = 0b00000101;
    }
    else if (strcmp(str_lida, "ADD-|M") == 0)
    {
        *opcode = 0b00000111;
    }
    else if (strcmp(str_lida, "SUB-M") == 0) 
    {
        strcpy(opcode, "00000110");
    }
    else if (strcmp(str_lida, "SUB-|M") == 0) 
    {
        *opcode = 0b00001000;
    }
    else if (strcmp(str_lida, "MUL-M") == 0) 
    {
        *opcode = 0b00001011;
    }
    else if (strcmp(str_lida, "DIV-M") == 0) 
    {
        *opcode = 0b00001100;
    }
    else if (strcmp(str_lida, "LSH") == 0) 
    {
        *opcode = 0b00010100;
    }
    else if (strcmp(str_lida, "RSH") == 0) 
    {
       *opcode = 0b00010101;
    }
    else if (strcmp(str_lida, "EXIT") == 0) 
    {
        *opcode = 0b11111111;
    }
    else
    { 
        *opcode = 0b00000000;
    }
}

int main(int argc, char *argv[])
{
     char **memory = (char **)malloc(4096 * sizeof(char *));

    FILE *arq;
    FILE *arquivo_saida;

    char buffer[BUFFER_SIZE];
    char linha_lida[BUFFER_SIZE];
    char linha_lida2[BUFFER_SIZE];
    char linha_lida_backup[BUFFER_SIZE];
    char linha_lida2_backup[BUFFER_SIZE];

    
    if (argc == 5 && (strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-m") == 0))
    {
        if ((arq = fopen(argv[2], "r")) == NULL)
        {
            printf("Erro ao abrir o aquivo!");
            exit(1);
        }

        int i = 0;
        while (fgets(linha_lida, BUFFER_SIZE, arq) != NULL)
            {
                char value = linha_lida[0];
                if (value >= 48 && value <= 57 || value == 45) // Para os dados
                {
                    linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
                    memory[i] = (char *)malloc(5 * sizeof(char));
                    strcpy(memory[i], linha_lida);
                    i++;
                }
                else if((fgets(linha_lida2, BUFFER_SIZE, arq) != NULL))
                {
                    //Isso vai acontecer caso a segunda instrucao lida exista.
                    long int palavra = 0;

                    int address1;
                    int address2;
                    char str1[BUFFER_SIZE] = "0";
                    char str2[BUFFER_SIZE] = "0";
                    
                    unsigned char opcode1;
                    unsigned char opcode2;

                    linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
                    linha_lida2[strlen(linha_lida2) - 1] = '\0'; // retirando o '\n' da string lida

                    strcpy(linha_lida_backup,linha_lida);
                    strcpy(linha_lida2_backup,linha_lida2);

                    //instrução da esquerda
                    finding_opcode(linha_lida_backup, &opcode1);
                    address1 = finding_address(linha_lida, str1);
                    
                    //instrução da direita
                    finding_opcode(linha_lida2_backup, &opcode2);
                    address2 = finding_address(linha_lida2, str2);

                    //palavra final
                    printf("%d",i);
                    palavra = opcode1 << 12;
                    palavra = palavra | address1;
                    palavra = palavra << 8;
                    palavra = palavra | opcode2;
                    palavra = palavra << 12;
                    palavra = palavra | address2;
                    
                    //Carregamento da palavra final para a memoria
                    memory[i] = (char *)malloc(5 * sizeof(char));
                    memory[i] = palavra;
                    i++;
                }
            }
        }

    //ao rodar o programa esta dando erro de falha de segmentacao
    //nao conseguimos arrumar o problema
    //quando terminarmos, enviaremos um email para o senhor
    if ((arquivo_saida = fopen(argv[4], "w")) == NULL)
    {
            printf("Erro ao abrir o aquivo!");
            exit(1);
    }

    for (int i = 0; i < 4096; i++)
    {
        fputs(memory[i], arquivo_saida);
    }

    free(memory);
    fclose(arquivo_saida);
    fclose(arq);

    return 0;
}