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

int main()
{
        char c;
        FILE *arq;

        if ((arq = fopen("texto.txt", "r")) == NULL)
        {
                printf("Erro ao abrir o aquivo!");
                exit(1);
        }

        while((c = fgetc(arq)) != EOF)
        {
                fputc(c, stdout);
        }

        fclose(arq);

        return 0;
}