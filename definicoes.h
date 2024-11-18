#ifndef DEFINICOES_H
#define DEFINICOES_H

// definicoes das estruturas e enumerações

typedef struct IAS
{
  // Program Control Unit
  int MAR; // MAR(12)
  int IR;  // IR(8)
  int IBR; // IBR(20)
  int PC;  // PC(12)

  // Arithmetic-logic unit (ALU)
  long AC;  // AC(40)
  long MQ;  // MQ(40)
  long MBR; // MBR(40)
} banco_de_registradores;

typedef enum
{
  NENHUM,             // opcode: inexistente
  LOAD_MQ,            // opcode: 10
  LOAD_MQ_MX,         // opcode: 9
  STOR_MX,            // opcode: 33
  LOAD_MX,            // opcode: 1
  LOAD_MINUS_MX,      // opcode: 2
  LOAD_PIPE_MX,       // opcode: 3
  LOAD_MINUS_PIPE_MX, // opcode: 4
  JUMP_MX_0_19,       // opcode: 13
  JUMP_MX_20_39,      // opcode: 14
  JUMPC_MX_0_19,      // opcode: 15
  JUMPC_MX_20_39,     // opcode: 16
  ADD_MX,             // opcode: 5
  ADD_PIPE_MX,        // opcode: 7
  SUB_MX,             // opcode: 6
  SUB_PIPE_MX,        // opcode: 8
  MUL_MX,             // opcode: 11
  DIV_MX,             // opcode: 12
  LSH,                // opcode: 20
  RSH,                // opcode: 21
  STOR_MX_8_19,       // opcode: 18
  STOR_MX_28_39,      // opcode: 19
  EXIT                // opcode: 255
} instrucoesIAS;

typedef enum
{
  soma,
  subtracao,
  multiplicacao,
  divisao,
  shift_direita,
  shift_esquerda
} operacoesULA;

typedef enum
{
  desnecessario, // 0
  necessario     // 1
} accesso_memoria;

// definições das funções/métodos

void resetar_registradores(banco_de_registradores *br);
void printar_registradores(banco_de_registradores *br);

void UC(banco_de_registradores *br, instrucoesIAS *instrucaoIAS);
void ULA(banco_de_registradores *br, operacoesULA operacao, long operando_memoria, __int128_t *resultado);
void barramento_memoria_reg(banco_de_registradores *br, unsigned char *memory, unsigned char *inicio_memory);

void ciclo_das_instrucoes(banco_de_registradores br, unsigned char *memory, unsigned char *inicio_memory);
void busca(banco_de_registradores *br, unsigned char *memory, accesso_memoria *acesso, unsigned char *inicio_memory);
void decodificacao(banco_de_registradores *br, accesso_memoria acesso, instrucoesIAS *instrucaoIAS);
void busca_de_operandos(banco_de_registradores *br, unsigned char *memory, instrucoesIAS instrucaoIAS, long *operando, unsigned char *inicio_memory);
void execucao(banco_de_registradores *br, unsigned char *memory, instrucoesIAS instrucaoIAS, long operando, __int128_t *resultado, unsigned char *inicio_memory);
void escrita_dos_resultados(banco_de_registradores *br, unsigned char *memory, instrucoesIAS instrucaoIAS, long operando, long resultado, unsigned char *inicio_memory);

int encontrando_endereco(char *str_lida, char *address);
void encontrando_opcode(char *str_lida, unsigned char *opcode);

void carregar_memoria(FILE *arqEntrada, unsigned char *memory);
void iniciar_memoria(unsigned char *memory, unsigned char *iniciar_memory);
void imprime_memoria(FILE *arqSaida, unsigned char *memory, unsigned char *inicio_memory);
void escrevendo_na_memoria(banco_de_registradores *br, unsigned char *memory, unsigned char *inicio_memory, __int128_t dado);

long obter_operando(banco_de_registradores *br, unsigned char *memory, unsigned char *inicio_memory);
int eh_negativo(long *operando);

#endif