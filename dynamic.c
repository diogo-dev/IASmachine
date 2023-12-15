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
// para simular a memoria do IAS devemos alocar um bloco de memoria, como se fosse um vetor
// char *memory = (char *) malloc(4096 * 5 * sizeof(char));
    
void finding_addres(char *str_lida, char *addres){
    const char delimitador1[2] = ")";
    const char delimitador2[2] = "(";
    char *token1;
    char *token2;
    
    int aux;
    for(int i=0; i<=strlen(str_lida); i++){
        if(str_lida[i] != '('){
            aux++;
        }
    }

    if(aux == strlen(str_lida)){
        token1 = strtok(str_lida, delimitador1);
        while(token1 != NULL){
            token2 = strtok(token1, delimitador2);
            while(token2 != NULL){
                strcpy(addres, token2);
                token2 = strtok(NULL, delimitador2);
                token1 = strtok(NULL, delimitador1);
            }
        }
    }
    else{
        strcpy(addres, "000000000000");
    }
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
    for (int i = 500; i < 502; i++)
    {
        printf("%s\n", memory[i]);
    }
}

void finding_instruction(char *str_lida, char *opcode)
{
    if (strcmp(str_lida, "LOAD-MQ") == 0) {
        strcpy(opcode, "00001010");
    }
    else if (strcmp(str_lida, "LOAD-MQ,M(X)") == 0) {
        strcpy(opcode, "00001001");
    }
    else if (strcmp(str_lida, "STOR-M(X)") == 0) {
        strcpy(opcode, "00100001");
    }
    else if (strcmp(str_lida, "LOAD-M(X)") == 0) {
        strcpy(opcode, "00000001");
    }
    else if (strcmp(str_lida, "LOAD--M(X)") == 0) {
        strcpy(opcode, "00000010");
    }
    else if (strcmp(str_lida, "LOAD-|M(X)|") == 0) {
        strcpy(opcode, "00000011");
    }    
    else if (strcmp(str_lida, "LOAD--|M(X)|") == 0) {
        strcpy(opcode, "00000100");
    }
    else if (strcmp(str_lida, "JUMP-M(X,0:19)") == 0) {
        strcpy(opcode, "00001101");
    }
    else if (strcmp(str_lida, "JUMP-M(X,20:39)") == 0) {
        strcpy(opcode, "00001110");
    }
    else if (strcmp(str_lida, "JUMP-+M(X,0:19)") == 0) {
        strcpy(opcode, "00001111");
    }
    else if (strcmp(str_lida, "JUMP-+M(X,20:39)") == 0) {
        strcpy(opcode, "00010000");
    }
    else if (strcmp(str_lida, "ADD-M(X)") == 0) {
        strcpy(opcode, "00000101");
    }
    else if (strcmp(str_lida, "ADD-|M(X)|") == 0) {
        strcpy(opcode, "00000111");
    }
    else if (strcmp(str_lida, "SUB-M(X)") == 0) {
        strcpy(opcode, "00000110");
    }
    else if (strcmp(str_lida, "SUB-|M(X)|") == 0) {
        strcpy(opcode, "00001000");
    }
    else if (strcmp(str_lida, "MUL-M(X)") == 0) {
        strcpy(opcode, "00001011");
    }
    else if (strcmp(str_lida, "DIV-M(X)") == 0) {
        strcpy(opcode, "00001100");
    }
    else if (strcmp(str_lida, "LSH") == 0) {
        strcpy(opcode, "00010100");
    }
    else if (strcmp(str_lida, "RSH") == 0) {
        strcpy(opcode, "00010101");
    }
    else if (strcmp(str_lida, "STOR-M(X,8:19)") == 0) {
        strcpy(opcode, "00010010");
    }

    else if (strcmp(str_lida, "STOR-M(X,28:39)") == 0) {
        strcpy(opcode, "00010011");
    }
    else if (strcmp(str_lida, "EXIT") == 0) {
        strcpy(opcode, "11111111");
    }
}

int main()
{
    char **memory = (char **)malloc(4096 * sizeof(char *));
    char linha_lida[BUFFER_SIZE];
    FILE *arq;
    char *opcode[9];

    if ((arq = fopen("texto.txt", "r")) == NULL)
    {
        printf("Erro ao abrir o aquivo!");
        exit(1);
    }

    char address[12];
    int i=0;
    while (fgets(linha_lida, BUFFER_SIZE, arq) != NULL)
    {
        if (i < 500)
        {
            linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
            memory[i] = (char *)malloc(5 * sizeof(char));
            strcpy(memory[i], linha_lida);
            i++;
        }
        else
        {
            linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
            finding_addres(linha_lida, address);
            printf("%s\n", address);
        }
    }

    //display_memory_data(memory);
    //display_memory_instructions(memory);

    free(memory);
    fclose(arq);

    return 0;
}