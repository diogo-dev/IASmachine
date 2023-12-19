/*      Trabalho de Arquitetura 1
Desenvolver um simulador do computador IAS

Alunos: Diogo Felipe Soares da Silva    RA:124771
        Arthur Henrique Bando Ueda      RA:129406
        Gustavo Alves Glatz             RA:128592 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 40
// para simular a memoria do IAS devemos alocar um bloco de memoria, como se fosse um vetor
    
double potencia(int base, int expoente){
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

void finding_address(char *str_lida, char *address)
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
    if (strcmp(address, "-") == 0)
        strcpy(address, "000000000000");
}

void display_memory_data(char **memory)
{
    printf("---------Display memory data---------\n");
    for (int i = 0; i < 500; i++)
    {
        printf("%s\n", memory[i]);
    }
    printf("---------End Display Data---------\n");
}

void display_memory_instructions(char **memory)
{
    printf("---------Display memory instructions---------\n");
    for (int i = 500; i < 3596; i++)
    {
        printf("%s\n", memory[i]);
    }
}

void finding_opcode(char *str_lida, char *opcode) {

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

    if (strcmp(str_lida, "LOAD-MQ") == 0) {
        strcpy(opcode, "00001010");
    }
    else if (strcmp(str_lida, "LOAD-MQ,M") == 0) {
        strcpy(opcode, "00001001");
    }
    else if (strcmp(str_lida, "STOR-M") == 0) {
        token = strtok(str_lida_backup,delimitador_jump);
        if(strcmp(token,str_lida_backup2) == 0){
            strcpy(opcode,"00100001");
        }
        else {
            token_stor = strtok(str_lida_stor, delimitador_jump);
            token_stor = strtok(NULL, delimitador_final);
            if(strcmp(token_stor,"8:19") == 0) {
                strcpy(opcode, "00010010");
            }
            else if(strcmp(token_stor,"28:39") == 0){
                strcpy(opcode, "00010011");
            }
        }
    }
    else if (strcmp(str_lida, "LOAD-M") == 0) {
        strcpy(opcode, "00000001");
    }
    else if (strcmp(str_lida, "LOAD--M") == 0) {
        strcpy(opcode, "00000010");
    }
    else if (strcmp(str_lida, "LOAD-|M") == 0) {
        strcpy(opcode, "00000011");
    }
    else if (strcmp(str_lida, "LOAD--|M") == 0) {
        strcpy(opcode, "00000100");
    }
    else if (strcmp(str_lida, "JUMP-M") == 0) {
        token = strtok(str_lida_backup, delimitador_jump);
        token = strtok(NULL, delimitador_final);
        if(strcmp(token,"0:19") == 0) {
            strcpy(opcode, "00001101");
        }
        else if(strcmp(token,"20:39") == 0){
            strcpy(opcode, "00001110");
        }
    }
    else if (strcmp(str_lida, "JUMP-+M") == 0) {
        token = strtok(str_lida_backup, delimitador_jump);
        token = strtok(NULL, delimitador_final);
        if(strcmp(token,"0:19") == 0) {
            strcpy(opcode, "00001111");
        }
        else if(strcmp(token,"20:39") == 0){
            strcpy(opcode, "00010000");
        }
    }
    else if (strcmp(str_lida, "ADD-M") == 0) {
        strcpy(opcode, "00000101");
    }
    else if (strcmp(str_lida, "ADD-|M") == 0) {
        strcpy(opcode, "00000111");
    }
    else if (strcmp(str_lida, "SUB-M") == 0) {
        strcpy(opcode, "00000110");
    }
    else if (strcmp(str_lida, "SUB-|M") == 0) {
        strcpy(opcode, "00001000");
    }
    else if (strcmp(str_lida, "MUL-M") == 0) {
        strcpy(opcode, "00001011");
    }
    else if (strcmp(str_lida, "DIV-M") == 0) {
        strcpy(opcode, "00001100");
    }
    else if (strcmp(str_lida, "LSH") == 0) {
        strcpy(opcode, "00010100");
    }
    else if (strcmp(str_lida, "RSH") == 0) {
        strcpy(opcode, "00010101");
    }
    else if (strcmp(str_lida, "EXIT") == 0) {
        strcpy(opcode, "11111111");
    }
}

int main()
{
    FILE *arq;
    char **memory = (char **)malloc(4096 * sizeof(char *));

    char linha_lida[BUFFER_SIZE];
    char linha_lida2[BUFFER_SIZE];
    char linha_lida_backup[BUFFER_SIZE];
    char linha_lida2_backup[BUFFER_SIZE];

    char *trash;
    char word_string[BUFFER_SIZE];
    
    if ((arq = fopen("texto.txt", "r")) == NULL)
    {
        printf("Erro ao abrir o aquivo!");
        exit(1);
    }

    int i = 0;
    while (fgets(linha_lida, BUFFER_SIZE, arq) != NULL)
        {
            if (i < 500) // Para os dados
            {
                linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
                memory[i] = (char *)malloc(5 * sizeof(char));
                strcpy(memory[i], linha_lida);
                i++;
            }
            else if((fgets(linha_lida2, BUFFER_SIZE, arq) != NULL))
            {
                //Isso vai acontecer caso a segunda instrucao lida exista.
                printf("-----------------------------------------------------------\n");

                char address1[BUFFER_SIZE] = "-";
                char address2[BUFFER_SIZE] = "-";
                
                char opcode1[BUFFER_SIZE];
                char opcode2[BUFFER_SIZE];

                linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
                linha_lida2[strlen(linha_lida2) - 1] = '\0'; // retirando o '\n' da string lida

                strcpy(linha_lida_backup,linha_lida);
                strcpy(linha_lida2_backup,linha_lida2);

                //instrução da esquerda
                finding_opcode(linha_lida_backup,opcode1);
                finding_address(linha_lida, address1);
                strcat(opcode1,address1);
                
                //instrução da direita
                finding_opcode(linha_lida2_backup,opcode2);
                finding_address(linha_lida2, address2);
                strcat(opcode2,address2);

                //palavra final
                strcat(opcode1,opcode2);

                //Carregamento da palavra final para a memoria
                sprintf(word_string,"%lli", binary_decimal(opcode1));
                memory[i] = (char *)malloc(5 * sizeof(char));
                strcpy(memory[i],word_string);
                i++;
            }
            else 
            {
                //Caso nao tenha a instrucao da direita para ser colocada na memoria, iremos apenas pegar a da esquerda.
                printf("--------------------------------------------------------------\n");

                char address1[BUFFER_SIZE] = "-";
                char opcode1[BUFFER_SIZE];

                char empty_instruction[BUFFER_SIZE];
                strcpy(empty_instruction, "00000000000000000000");
                
                linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida

                strcpy(linha_lida_backup,linha_lida);

                //instrução da esquerda (única)
                finding_opcode(linha_lida_backup,opcode1);
                finding_address(linha_lida, address1);
                strcat(opcode1,address1);

                //Carregamento da palavra final (instrução esquerda + "00000000000000000000") para a memoria
                strcat(opcode1,empty_instruction);
                sprintf(word_string,"%lli", binary_decimal(opcode1));
                memory[i] = (char *)malloc(5 * sizeof(char));
                strcpy(memory[i],word_string);
                i++;

            }
        }
    display_memory_data(memory);
    display_memory_instructions(memory);

    free(memory);
    fclose(arq);

    return 0;
}