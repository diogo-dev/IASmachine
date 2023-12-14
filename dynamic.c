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
}

int main()
{
    char **memory = (char **)malloc(4096 * sizeof(char *));
    char linha_lida[BUFFER_SIZE];
    FILE *arq;
    char *opcode;

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
    /*printf("%d\n", *opcode);
    finding_instruction(memory[500], opcode);
    printf("%d\n", *opcode);
    finding_instruction(memory[501], opcode);
    printf("%d\n", *opcode);*/

    free(memory);
    fclose(arq);

    return 0;
}