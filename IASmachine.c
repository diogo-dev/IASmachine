/*      Trabalho de Arquitetura 1
Desenvolver um simulador do computador IAS

Alunos: Diogo Felipe Soares da Silva    RA:124771
        Arthur Henrique Bando Ueda      RA:129406
        Gustavo Alves Glatz             RA:128592
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//para simular a memoria do IAS devemos alocar um bloco de memoria, como se fosse um vetor
//char *memory = (char *) malloc(4096 * 5 * sizeof(char));


void read_line(char str[], int size, FILE *arq)
{
        int i = 0;
        char c = fgetc(arq);
        while(c != '\n')
        {
               if (i < size-1)
               {
                       str[i] = c;
                       i++;
               }
               c = fgetc(arq);
        }
        str[i] = '\0';
}

void display_memory_data(char *memory)
{
        printf("---------Display memory---------\nData:\n");
        for (int i = 0; i < 500; i++)
        {
                printf("%c\n", *memory);
                memory += 5;
        }
        printf("---------End Display Data---------\n");
}

int main()
{
        char *memory = (char *) malloc(4096 * 5 * sizeof(char));
        char *aux; 
        aux = memory;
        char linha_lida[10];
        FILE *arq;

        if ((arq = fopen("texto.txt", "r")) == NULL)
        {
                printf("Erro ao abrir o aquivo!");
                exit(1);
        }

        //mudar a condicao para a funcao read_line → colocar ela com retorno int afim de testar a condicao
        while (fgets(linha_lida, 10, arq) != NULL)
        {
                *memory = linha_lida[0];
                for (int i = 1; i < 10; i++)
                {
                        //*memory = linha_lida[i]; nao da para fazer assim
                        //strcat(*memory, linha_lida[i]); nao da tbm
                }
                memory += 5;
        }
        memory = aux; //fazer memory apontar para o inicio da memoria alocada
        display_memory_data(memory);

        free(memory);
        fclose(arq);

        return 0;
}