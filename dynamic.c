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


void display_memory_data(char **memory)
{
    printf("---------Display memory---------\nData:\n");
    for (int i = 0; i < 500; i++)
    {
        printf("%s\n", memory[i]);
    }
    printf("---------End Display Data---------\n");
}

int main()
{
    char **memory = (char **)malloc(4096 * sizeof(char *));
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
        linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
        memory[i] = (char *)malloc(5 * sizeof(char));
        strcpy(memory[i], linha_lida);
        i++; // equivalente a memory += 5;
    }
    display_memory_data(memory);

    free(memory);
    fclose(arq);

    return 0;
}