#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "definicoes.h"

int main(int argc, char *argv[])
{
  unsigned char *memory = (unsigned char *)malloc(4096 * 5 * sizeof(char *)); // alocando espaço para a memória de forma manual no heap
  unsigned char *inicio_memory;
  inicio_memory = memory; // variavel inicio_memory recebe o endereco do inicio da memoria

  iniciar_memoria(memory, inicio_memory); // iniciar a memoria com valor 0

  FILE *arquivo_entrada;
  FILE *arquivo_saida;

  if (argc == 7 && (strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-l") == 0) && (strcmp(argv[5], "-m") == 0))
  {
    printf("Argumentos válidos!\n\n");
  }
  else
  {
    printf("Erro: Abra o arquivo como o seguinte exemplo:\n");
    printf("./nomeExecutavel -p arqEntrada.ias.txt -l linhaDasInstrucoes -m arqSaida.ias.txt\n");
    exit(1);
  }

  if ((arquivo_entrada = fopen(argv[2], "r")) == NULL)
  {
    printf("Erro ao abrir o aquivo!");
    exit(1);
  }

  carregar_memoria(arquivo_entrada, memory);

  banco_de_registradores br;
  resetar_registradores(&br);
  br.PC = atoi(argv[4]); // receber endereco da linha de comando

  ciclo_das_instrucoes(br, memory, inicio_memory);

  if ((arquivo_saida = fopen(argv[6], "w")) == NULL)
  {
    printf("Erro ao abrir o aquivo!");
    exit(1);
  }

  imprime_memoria(arquivo_saida, memory, inicio_memory);

  memory = inicio_memory;
  free(memory);
  fclose(arquivo_saida);
  fclose(arquivo_entrada);
  printf("Fim do programa, sucesso!\n\n");

  return 0;
}