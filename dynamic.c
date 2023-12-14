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

char finding_addres(char *str_lida, char* no_addres){
    for(int i=0; i=strlen(str_lida); i++){
        if(str_lida[i] == '('){
            return str_lida[i+1];
        }
    } 
    *no_addres = "000000000000";
    return 0;
}


long int decoder(long int number)
{
    int number_size = 0;
    long int temp_number = number, to_return = 0, power_2 = 1;

    // ------------ Transformando um inteiro em um vetor de inteiro --------------

    while (temp_number != 0)
    {

        temp_number /= 10;
        number_size++;
    }

    long int numberArray[number_size];
    int temp = (number_size - 1);
    temp_number = number;

    while (temp_number != 0)
    {

        numberArray[temp] = temp_number % 10;
        temp_number /= 10;
        temp--;
    }

    // ------------ Transformando um inteiro em um vetor de inteiro --------------

    // Realizar transformacao do vetor binario em um inteiro decimal

    for (int i = number_size; i >= 0; i--)
    {
        to_return += numberArray[i] * power_2;
        power_2 *= 2;
    }

    return to_return;
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

    int i = 0;
    while (fgets(linha_lida, BUFFER_SIZE, arq) != NULL)
    {
        linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
        memory[i] = (char *)malloc(5 * sizeof(char));
        strcpy(memory[i], linha_lida);
        i++; // equivalente a memory += 5;
    }

    display_memory_data(memory);
    display_memory_instructions(memory);
    printf("%s\n", opcode);
    finding_instruction(memory[500], opcode);
    printf("%s\n", opcode);
    finding_instruction(memory[501], opcode);
    printf("%s\n", opcode);

    //char *trash;
    //printf("%d", (strtol(opcode, &trash, 2)));

    free(memory);
    fclose(arq);

    return 0;
}