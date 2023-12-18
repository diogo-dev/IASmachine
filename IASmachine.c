/*      Trabalho de Arquitetura 1
Desenvolver um simulador do computador IAS

Alunos: Diogo Felipe Soares da Silva    RA:124771
        Arthur Henrique Bando Ueda      RA:129406
        Gustavo Alves Glatz             RA:128592
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 40

void finding_address(char *str_lida, char *address)
{
        int i = 0;
        int j = 0;
        int k = 0;

        while (i < strlen(str_lida) && str_lida[i] != ')')
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
        if (strcmp(address, "-") == 0)
                strcpy(address, "000000000000");
}

void finding_opcode(char *str_lida, char *opcode)
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
        strcpy(str_lida_backup2, str_lida);
        token = strtok(str_lida, delimitador_inicial);

        if (strcmp(str_lida, "LOAD-MQ") == 0)
        {
                strcpy(opcode, "00001010");
        }
        else if (strcmp(str_lida, "LOAD-MQ,M") == 0)
        {
                strcpy(opcode, "00001001");
        }
        else if (strcmp(str_lida, "STOR-M") == 0)
        {
                token = strtok(str_lida_backup, delimitador_jump);
                if (strcmp(token, str_lida_backup2) == 0)
                {
                        strcpy(opcode, "00100001");
                }
                else
                {
                        token_stor = strtok(str_lida_stor, delimitador_jump);
                        token_stor = strtok(NULL, delimitador_final);
                        if (strcmp(token_stor, "8:19") == 0)
                        {
                                strcpy(opcode, "00010010");
                        }
                        else if (strcmp(token_stor, "28:39") == 0)
                        {
                                strcpy(opcode, "00010011");
                        }
                }
        }
        else if (strcmp(str_lida, "LOAD-M") == 0)
        {
                strcpy(opcode, "00000001");
        }
        else if (strcmp(str_lida, "LOAD--M") == 0)
        {
                strcpy(opcode, "00000010");
        }
        else if (strcmp(str_lida, "LOAD-|M") == 0)
        {
                strcpy(opcode, "00000011");
        }
        else if (strcmp(str_lida, "LOAD--|M") == 0)
        {
                strcpy(opcode, "00000100");
        }
        else if (strcmp(str_lida, "JUMP-M") == 0)
        {
                token = strtok(str_lida_backup, delimitador_jump);
                printf("%s\n", token);
                token = strtok(NULL, delimitador_final);
                printf("%s\n", token);
                if (strcmp(token, "0:19") == 0)
                {
                        strcpy(opcode, "00001101");
                }
                else if (strcmp(token, "20:39") == 0)
                {
                        strcpy(opcode, "00001110");
                }
        }
        else if (strcmp(str_lida, "JUMP-+M") == 0)
        {
                token = strtok(str_lida_backup, delimitador_jump);
                printf("%s\n", token);
                token = strtok(NULL, delimitador_final);
                printf("%s\n", token);
                if (strcmp(token, "0:19") == 0)
                {
                        strcpy(opcode, "00001111");
                }
                else if (strcmp(token, "20:39") == 0)
                {
                        strcpy(opcode, "00010000");
                }
        }
        else if (strcmp(str_lida, "ADD-M") == 0)
        {
                strcpy(opcode, "00000101");
        }
        else if (strcmp(str_lida, "ADD-|M") == 0)
        {
                strcpy(opcode, "00000111");
        }
        else if (strcmp(str_lida, "SUB-M") == 0)
        {
                strcpy(opcode, "00000110");
        }
        else if (strcmp(str_lida, "SUB-|M") == 0)
        {
                strcpy(opcode, "00001000");
        }
        else if (strcmp(str_lida, "MUL-M") == 0)
        {
                strcpy(opcode, "00001011");
        }
        else if (strcmp(str_lida, "DIV-M") == 0)
        {
                strcpy(opcode, "00001100");
        }
        else if (strcmp(str_lida, "LSH") == 0)
        {
                strcpy(opcode, "00010100");
        }
        else if (strcmp(str_lida, "RSH") == 0)
        {
                strcpy(opcode, "00010101");
        }
        else if (strcmp(str_lida, "STOR-M") == 0)
        {
                token = strtok(str_lida_backup, delimitador_jump);
                printf("%s\n", token);
                token = strtok(NULL, delimitador_final);
                printf("%s\n", token);
                if (strcmp(token, "0:19") == 0)
                {
                        strcpy(opcode, "00010010");
                }
                else if (strcmp(token, "20:39") == 0)
                {
                        strcpy(opcode, "00010011");
                }
        }
        else if (strcmp(str_lida, "EXIT") == 0)
        {
                strcpy(opcode, "11111111");
        }
}

void display_memory_data(char *memory)
{
        printf("---------Display memory---------\nData:\n");
        for (int i = 0; i < 500; i++)
        {
                printf("%s\n", *memory);
                memory += 5;
        }
        printf("---------End Display Data---------\n");
}

int main()
{
        char *memory = (char *) malloc(4096 * 5 * sizeof(char));
        char *aux; 
        aux = memory; // fazendo aux receber o endereco de inicio da memoria
        char linha_lida[BUFFER_SIZE];
        FILE *arq;

        if ((arq = fopen("texto.txt", "r")) == NULL)
        {
                printf("Erro ao abrir o aquivo!");
                exit(1);
        }

        int i = 0;
        while (fgets(linha_lida, BUFFER_SIZE, arq) != NULL)
        {
                if (i < 500)
                {
                        linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
                        // strcpy(*memory, linha_lida);
                        memory += 5;
                }
        }
        memory = aux; //fazer memory apontar para o inicio da memoria alocada
        display_memory_data(memory);

        free(memory);
        fclose(arq);

        return 0;
}