/*      Trabalho de Arquitetura 1
Desenvolver um simulador do computador IAS

Alunos: Diogo Felipe Soares da Silva    RA:124771
        Arthur Henrique Bando Ueda      RA:129406
        Gustavo Alves Glatz             RA:128592 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 40
#define UNSIGNED_CHAR_SIZE 255 //0b11111111
#define BITWISE_12 4095 //0b111111111111
#define BITWISE_20 1048575 //0b11111111111111111111
#define BITWISE_40 1099511627775 //0b1111111111111111111111111111111111111111

typedef struct IAS
{
    //Program Control Unit
    int MAR; //MAR(12)
    int IR; //IR(8)
    int IBR; //IBR(20)
    int PC; //PC(12)

    //Arithmetic-logic unit (ALU)
    long AC; //AC(40)
    long MQ; //MQ(40)
    long MBR; //MBR(40)
}banco_de_registradores;

void resetar_registradores(banco_de_registradores *br)
{
    br->MAR = 0;
    br->IR = 0;
    br->IBR = 0;
    br->PC = 0;

    br->AC = 0;
    br->MQ = 0;
    br->MBR = 0;
}

typedef enum{
    NENHUM,
    LOAD_MQ,
    LOAD_MQ_MX,
    STOR_MX,
    LOAD_MX,
    LOAD_MINUS_MX,
    LOAD_PIPE_MX,
    LOAD_MINUS_PIPE_MX,
    JUMP_MX_0_19,
    JUMP_MX_20_39,
    JUMPC_MX_0_19,
    JUMPC_MX_20_39,
    ADD_MX,
    ADD_PIPE_MX,
    SUB_MX,
    SUB_PIPE_MX,
    MUL_MX,
    DIV_MX,
    LSH,
    RSH,
    STOR_MX_8_19,
    STOR_MX_28_39,
    EXIT
}instrucoesIAS;

typedef enum{
    soma,
    subtracao,
    multiplicacao,
    divisao,
    shift_direita,
    shift_esquerda
}operacoesULA;

typedef struct{
    long a;
    long b;
    operacoesULA operacao;
    long resultado;
}compodenteULA;

typedef enum{
    direita, // 0
    esquerda // 1
}ladoInstrucao;


/*
    Vamos utilizar de flags no UC para conseguirmos mandar para a execucao qual instrucao sera realizada
    
    | flag_decoder = 0 Nao vai realizar a busca na memoria. E seguimos pelo lado esquerdo do fluxograma na parte de fetch cycle
    | flag_decoder = 1 Vai realizar uma busca na memoria. E seguimos pelo lado direito do fluxograma na parte de fetch cycle
    
    | flag = 0 // LOAD MQ
    | flag = 1 // LOAD MQ, M(X)
    | flag = 2 // STOR M(X)
    | flag = 3 // LOAD M(X)
    | flag = 4 // LOAD -M(X)
    | flag = 5 // LOAD |M(X)|
    | flag = 6 // LOAD -|M(X)|
    | flag = 7 // JUMP M(X, 0:19)
    | flag = 8 // JUMP M(X, 20:39)
    | flag = 9 // JUMP +M(X, 0:19)
    | flag = 10 // JUMP +M(X, 20:39)
    | flag = 11 // ADD M(X)
    | flag = 12 // ADD M|(X)|
    | flag = 13 // SUB M(X)
    | flag = 14 // SUB |M(X)|
    | flag = 15 // MUL M(X)
    | flag = 16 // DIV M(X)
    | flag = 17 // LSH
    | flag = 18 // RSH
    | flag = 19 // STOR M(X,8:19)
    | flag = 20 // STOR M(X, 28:39)

    busca de operandos
    flag = 1 // LOAD MQ, M(X)
    flag = 3 // LOAD M(X)
    flag = 4 // LOAD -M(X)
    flag = 5 // LOAD |M(X)|
    flag = 6 // LOAD -|M(X)|
    flag = 11 // ADD M(X)
    flag = 12 // ADD M|(X)|
    flag = 13 // SUB M(X)
    flag = 14 // SUB |M(X)|
    flag = 15 // MUL M(X)
    flag = 16 // DIV M(X)
*/

/* void fetch_cycle(banco_de_registradores *br, unsigned char *memory)
{
    //Is next instruction in IBR ?
    if(br->IBR != 0)
    {
        //No memory access required
        int temp1 = br->IBR; //armazena o seu valor em uma variavel temp
        //IR <- IBR(0:7)
        br->IBR = br->IBR >> 12;
        br->IR = br->IR | br->IBR;
        //MAR <- IBR(8:19)
        br->IBR = temp1; //restaura o seu valor de 20bits
        br->MAR = br->IBR & BITWISE_12;
        //PC <- PC + 1
        memory = br->PC;
        memory = memory + 5;
        br->PC = memory;
    }
    else
    {
        //Memory access required
        //MAR <- PC
        br->MAR = br->PC;
        //MBR <- M(MAR)
        memory = br->MAR;
        while(j < 5)
        {
            br->MBR = br->MBR << 8;
            br->MBR = br->MBR | *memory;
            memory++;
            j++;
        }
        //Left instruction required ? usar flag
        int temp3;
        int temp4;
        temp3 = br->MBR & BITWISE_20;
        if(temp3 == 0)
        {
            // instrucao da direita nao existe
            br->MBR = br->MBR >> 20;
            temp4 = br->MBR;
            br->MBR = br->MBR >> 12;
            br->IR = br->IR | br->MBR;
            
            br->MBR = temp4;
            br->MAR = br->MBR & BITWISE_12;
            //PC <- PC + 1
            memory = br->PC;
            memory = memory + 5;
            br->PC = memory;
        }
        else
        {
            //instrucao da direita exite
            br->IBR = br->MBR & BITWISE_20;
            br->MBR = br->MBR >> 20;
            temp4 = br->MBR;
            br->MBR = br->MBR >> 12;
            br->IR = br->IR | br->MBR;
            
            br->MBR = temp4;
            br->MAR = br->MBR & BITWISE_12;
        }

    }
}*/

void UC(banco_de_registradores *br, instrucoesIAS *instrucaoIAS) 
{ 
    // Na unidade de controle, nos apenas marcamos que instrucao(opcode) estamos realizando utilizando uma flag
    // para que assim seja possivel realizar a busca dos operandos e a execucao para aquela instrucao em especifico

    if(br->IR == 0b00001010) { 
        *instrucaoIAS = LOAD_MQ;
    }
    else if(br-> IR == 0b00001001) { 
        *instrucaoIAS = LOAD_MQ_MX;
    }
    else if(br-> IR == 0b00100001 ) { 
        *instrucaoIAS = STOR_MX;
    }
    else if(br-> IR == 0b00000001) { 
        *instrucaoIAS = LOAD_MX;
    }
    else if(br-> IR == 0b00000010) { 
        *instrucaoIAS = LOAD_MINUS_MX;
    }
    else if(br-> IR == 0b00000011) { 
        *instrucaoIAS = LOAD_PIPE_MX;
    }
    else if(br-> IR == 0b00000100) { 
        *instrucaoIAS = LOAD_MINUS_PIPE_MX;
    }
    else if(br-> IR == 0b00001101) { 
        *instrucaoIAS = JUMP_MX_0_19;
    }
    else if(br-> IR == 0b00001110) { 
        *instrucaoIAS = JUMP_MX_20_39;
    }
    else if(br-> IR == 0b00001111) { 
        *instrucaoIAS = JUMPC_MX_0_19;
    }
    else if(br-> IR == 0b00010000) { 
        *instrucaoIAS = JUMPC_MX_20_39;
    }
    else if(br-> IR == 0b0000101) { 
        *instrucaoIAS = ADD_MX;
    }
    else if(br-> IR == 0b00000111) { 
        *instrucaoIAS = ADD_PIPE_MX;
    }
    else if(br-> IR == 0b0000110) { 
        *instrucaoIAS = SUB_MX;
    }
    else if(br-> IR == 0b00001000) { 
        *instrucaoIAS = SUB_PIPE_MX;
    }
    else if(br-> IR == 0b00001011) { 
        *instrucaoIAS = MUL_MX;
    }
    else if(br-> IR == 0b00001100) { 
        *instrucaoIAS = DIV_MX;
    }
    else if(br-> IR == 0b00010100) { 
        *instrucaoIAS = LSH;
    }
    else if(br-> IR == 0b00010101) { 
        *instrucaoIAS = RSH;
    }
    else if(br-> IR == 0b00010010) { 
        *instrucaoIAS = STOR_MX_8_19;
    }
    else if(br-> IR == 0b00010011) { 
        *instrucaoIAS = STOR_MX_28_39;
    }
    else if(br -> IR == 0b11111111) { 
        *instrucaoIAS = EXIT;
    }
    else{
        *instrucaoIAS = NENHUM;
    }
}

void ULA(banco_de_registradores *br, operacoesULA operacao, long operando_memoria)
{
    __int128_t palavra_80bits,temp = 0;

    if (operacao == soma)
    {
        br->AC = br->AC + operando_memoria;
    }
    else if (operacao == subtracao)
    {
        br->AC = operando_memoria - br->AC;
    }
    else if (operacao == multiplicacao)
    {
        palavra_80bits = operando_memoria * br->MQ;
        //colocar os bits mais significativos em AC e os menos significativos em MQ
        temp = palavra_80bits;
        palavra_80bits = palavra_80bits >> 40;
        br->AC = br->AC | palavra_80bits;
        palavra_80bits = temp;
        br->MQ = palavra_80bits & BITWISE_40;
        //Para juntar AC com MQ a fim de ver o resultado da multiplicacao,
        //esse resultado tende de ser maior que o numero (em decimal): 549755813888 = 0b1000000000000000000000000000000000000000
        //Vamos so checar MQ
    }
    else if (operacao == divisao)
    {
        palavra_80bits = br->AC / operando_memoria;
        // Put the quotient in MQ and the remainder in AC
        // ver video de divisao de binario para entender
    }
    else if (operacao == shift_esquerda)
    {
        br->AC = br->AC << 1; //Multiplicar por 2
    }
    else
    {
        br->AC = br->AC >> 1; //Dividir por 2
    }
}

void decodificacao(banco_de_registradores *br, int flag_escrita, instrucoesIAS *instrucaoIAS)  
{
    if(flag_escrita == 0)
    { 
        // setar o IR
        br->IR = 0;
        int temp1 = br->IBR; //armazena o seu valor em uma variavel temp
        //IR <- IBR(0:7)
        br->IBR = br->IBR >> 12;
        br->IR = br->IR | br->IBR;
        //MAR <- IBR(8:19)
        br->IBR = temp1; //restaura o seu valor de 20bits
        br->MAR = br->IBR & BITWISE_12;
        //PC <- PC + 1
        br->PC = br->PC + 1;
        //setar IBR
        br->IBR = 0;
    }
    else if(flag_escrita == 1) 
    { 
        //Left instruction required ? usar flag
        int temp3;
        int temp4;
        temp3 = br->MBR & BITWISE_20;
        if(temp3 == 0)
        {
            // instrucao da direita nao existe
            br->MBR = br->MBR >> 20;
            temp4 = br->MBR;
            br->MBR = br->MBR >> 12;
            br->IR = br->IR | br->MBR;
            
            br->MBR = temp4;
            br->MAR = br->MBR & BITWISE_12;
            //PC <- PC + 1
            br->PC = br->PC + 1;
            //setar MBR
            br->MBR = 0;
        }
        else
        {
            //instrucao da direita exite
            br->IBR = br->MBR & BITWISE_20;
            br->MBR = br->MBR >> 20;
            temp4 = br->MBR;
            br->MBR = br->MBR >> 12;
            br->IR = br->IR | br->MBR;
            
            br->MBR = temp4;
            br->MAR = br->MBR & BITWISE_12;
            //setar MBR
            br->MBR = 0;
        }
    }

    UC(br, instrucaoIAS);
}

void barramento_memoria_reg(banco_de_registradores *br, unsigned char *memory, unsigned char *inicio_memory)
{
    //Memory access required
    //MAR <- PC
    br->MAR = br->PC;  
    //MBR <- M(MAR)
    memory = inicio_memory;
    memory = memory + 5 * (br->MAR - 1); //Explicacao do -1: Instrucoes comeca na linha 501, vou incrementar memory 500 vezes
    int j = 0;
    while(j < 5)
    {
        br->MBR = br->MBR << 8;
        br->MBR = br->MBR | *memory;
        memory++;
        j++;
    }
}

void busca(banco_de_registradores * br, unsigned char * memory, int *flag_escrita, unsigned char *inicio_memory)
{ 

    int j = 0;
    if(br->IBR != 0) 
    { 
        //Memory access not required
        //Marcamos com uma flag para sabermos o que fazer na decodificacao
        *flag_escrita = 0; 
    }
    else 
    { 
        *flag_escrita = 1;
        //Marcamos com uma flag para sabermos o que fazer na decodificacao
        barramento_memoria_reg(br, memory, inicio_memory);
    }
}

void barramento_reg_memoria(banco_de_registradores br, unsigned char *memory)
{
    //escrita dos resultados
}

int finding_address(char *str_lida, char *address)
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
    if (strcmp(address, "0") == 0)
    {
        return 0;
    }
    else
    {
        int i = atoi(address);
        return i;
    }
}

void finding_opcode(char *str_lida, unsigned char *opcode) 
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
    strcpy(str_lida_backup2,str_lida);
    token = strtok(str_lida,delimitador_inicial);

    if (strcmp(str_lida, "LOAD-MQ") == 0) 
    {
        *opcode = 0b00001010;
    }
    else if (strcmp(str_lida, "LOAD-MQ,M") == 0) 
    {
        *opcode = 0b00001001;
    }
    else if (strcmp(str_lida, "STOR-M") == 0) 
    {
        token = strtok(str_lida_backup,delimitador_jump);
        if(strcmp(token,str_lida_backup2) == 0)
        {
            *opcode = 0b00100001;
        }
        else 
        {
            token_stor = strtok(str_lida_stor, delimitador_jump);
            token_stor = strtok(NULL, delimitador_final);
            if(strcmp(token_stor,"8:19") == 0) 
            {
                *opcode = 0b00010010;
            }
            else if(strcmp(token_stor,"28:39") == 0)
            {
                *opcode = 0b00010011;
            }
        }
    }
    else if (strcmp(str_lida, "LOAD-M") == 0) 
    {
        *opcode = 0b00000001;
    }
    else if (strcmp(str_lida, "LOAD--M") == 0) 
    {
        *opcode = 0b00000010;
    }
    else if (strcmp(str_lida, "LOAD-|M") == 0) 
    {
        *opcode = 0b00000011;
    }
    else if (strcmp(str_lida, "LOAD--|M") == 0) 
    {
        *opcode = 0b00000100;
    }
    else if (strcmp(str_lida, "JUMP-M") == 0) 
    {
        token = strtok(str_lida_backup, delimitador_jump);
        token = strtok(NULL, delimitador_final);
        if(strcmp(token,"0:19") == 0) 
        {
            *opcode = 0b00001101;
        }
        else if(strcmp(token,"20:39") == 0)
        {
            *opcode = 0b00001110;
        }
    }
    else if (strcmp(str_lida, "JUMP+-M") == 0) 
    {
        token = strtok(str_lida_backup, delimitador_jump);
        token = strtok(NULL, delimitador_final);
        if(strcmp(token,"0:19") == 0) 
        {
            *opcode = 0b00001111;
        }
        else if(strcmp(token,"20:39") == 0) 
        {
            *opcode = 0b00010000;
        }
    }
    else if (strcmp(str_lida, "ADD-M") == 0) 
    {
        *opcode = 0b00000101;
    }
    else if (strcmp(str_lida, "ADD-|M") == 0)
    {
        *opcode = 0b00000111;
    }
    else if (strcmp(str_lida, "SUB-M") == 0) 
    {
        *opcode =  0b00000110;
    }
    else if (strcmp(str_lida, "SUB-|M") == 0) 
    {
        *opcode = 0b00001000;
    }
    else if (strcmp(str_lida, "MUL-M") == 0) 
    {
        *opcode = 0b00001011;
    }
    else if (strcmp(str_lida, "DIV-M") == 0) 
    {
        *opcode = 0b00001100;
    }
    else if (strcmp(str_lida, "LSH") == 0) 
    {
        *opcode = 0b00010100;
    }
    else if (strcmp(str_lida, "RSH") == 0) 
    {
       *opcode = 0b00010101;
    }
    else if (strcmp(str_lida, "EXIT") == 0) 
    {
        *opcode = 0b11111111;
    }
    else
    { 
        *opcode = 0b00000000;
    }
}

void imprime_memory(FILE *arqSaida, unsigned char *memory, unsigned char *inicio_memory)
{
    memory = inicio_memory;
    for (int i = 0; i < 4096; i++) 
    {
        long int palavra_temp = 0;
        char string_temp[BUFFER_SIZE];
        int j = 0;

        while(j < 5)
        {
            palavra_temp = palavra_temp << 8;
            palavra_temp = palavra_temp | *memory;
            memory++;
            j++;
        }

        //converter palavra_temp em string_temp
        sprintf(string_temp, "%ld", palavra_temp);
        //escrever string_temp no arquivo de saida
        fputs(string_temp, arqSaida);
        fputc('\n', arqSaida);
    }
}

void iniciar_memory(unsigned char *memory, unsigned char *iniciar_memory)
{
    for(int i = 0; i < (4096 * 5); i++)
    {
        *memory = 0;
        memory++;
    }
    memory = iniciar_memory;
}

void carregar_memoria(FILE *arqEntrada, unsigned char *memory)
{
    char linha_lida[BUFFER_SIZE];
    char linha_lida2[BUFFER_SIZE];
    char linha_lida_backup[BUFFER_SIZE];
    char linha_lida2_backup[BUFFER_SIZE];

    int i = 0;
    while (fgets(linha_lida, BUFFER_SIZE, arqEntrada) != NULL)
    {
        char value = linha_lida[0];
        if ((value >= 48 && value <= 57) || value == 45)
        {
            // Para os dados
            linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
            if(value == 45)
            {
                //Quando o dado for negativo
                int j = 0;
                char dado_positivo_string[sizeof(linha_lida)];
                //Retirando o "-" do dado
                for (int i = 1; i < sizeof(linha_lida); i++)
                {
                    dado_positivo_string[j] = linha_lida[i];
                    j++;
                }
                unsigned long int dado;
                unsigned long int dado_final = 0;
                //Transformando em um inteiro
                dado = atoi(dado_positivo_string);
                //colocando 1 no bit mais significativo
                dado_final = dado_final | 1;
                dado_final = dado_final << 39;
                dado_final |= dado;
                //mandando para a memoria
                unsigned char palavra_8bits;
                for(int i = 4; i >= 0; i--)
                {
                    palavra_8bits = (dado_final >> (8*i)) & UNSIGNED_CHAR_SIZE; 
                    *memory = palavra_8bits;
                    memory++;
                }
            }
            else 
            {
                //Quando o dado nao for negativo
                long int dado;
                //Transformando em um inteiro
                dado = atoi(linha_lida);
                //Mandando para a memoria
                unsigned char palavra_8bits;
                for(int i = 4; i >= 0; i--)
                {
                    palavra_8bits = (dado >> (8*i)) & UNSIGNED_CHAR_SIZE;
                    *memory = palavra_8bits;
                    memory++;
                }
            }
        }
        else if((fgets(linha_lida2, BUFFER_SIZE, arqEntrada) != NULL))
        {
            //Isso vai acontecer caso a segunda instrucao lida exista.
            long int palavra = 0;

            int address1;
            int address2;

            char str1[BUFFER_SIZE] = "0";
            char str2[BUFFER_SIZE] = "0";
                    
            unsigned char opcode1;
            unsigned char opcode2;

            if (linha_lida[strlen(linha_lida) - 1] == '\n')
            {
                linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
            }
            if (linha_lida2[strlen(linha_lida2) - 1] == '\n')
            {
                 linha_lida2[strlen(linha_lida2) - 1] = '\0'; // retirando o '\n' da string lida
            }
            
            strcpy(linha_lida_backup,linha_lida);
            strcpy(linha_lida2_backup,linha_lida2);

            //instrução da esquerda
            finding_opcode(linha_lida_backup, &opcode1);
            address1 = finding_address(linha_lida, str1);
                    
            //instrução da direita
            finding_opcode(linha_lida2_backup, &opcode2);
            address2 = finding_address(linha_lida2, str2);

            //palavra final
            palavra = opcode1 << 12;
            palavra = palavra | address1;
            palavra = palavra << 8;
            palavra = palavra | opcode2;
            palavra = palavra << 12;
            palavra = palavra | address2;
                    
            //Carregamento da palavra final para a memoria
            unsigned char palavra_8bits;
            for(int i = 4; i >= 0; i--)
            {
                palavra_8bits = (palavra >> (8*i)) & UNSIGNED_CHAR_SIZE;
                *memory = palavra_8bits;
                memory++;
            }
        }
        else
        {
            //caso exista somente a primeira instrucao na palavra
            long int palavra = 0;

            int address1;

            char str1[BUFFER_SIZE] = "0";

            unsigned char opcode1;

            if (linha_lida[strlen(linha_lida) - 1] == '\n')
            {
                linha_lida[strlen(linha_lida) - 1] = '\0'; // retirando o '\n' da string lida
            }

            strcpy(linha_lida_backup,linha_lida);

            //instrução da esquerda
            //lembrando que nesse caso nao ha instrucao da direita
            finding_opcode(linha_lida_backup, &opcode1);
            address1 = finding_address(linha_lida, str1);

            //palavra final
            palavra = opcode1 << 12;
            palavra = palavra | address1;
            palavra = palavra << 20;

            //Carregamento da palavra final para a memoria
            unsigned char palavra_8bits;
            for(int i = 4; i >= 0; i--)
            {
                palavra_8bits = (palavra >> (8*i)) & UNSIGNED_CHAR_SIZE;
                *memory = palavra_8bits;
                memory++;
            }
        }
        i++;
    }
}

int main(int argc, char *argv[])
{
    unsigned char *memory = (unsigned char *)malloc(4096 * 5 * sizeof(char *));
    unsigned char *inicio_memory; 

    inicio_memory = memory; //variavel inicio_memory recebe o endereco do inicio da memoria

    iniciar_memory(memory, inicio_memory); // iniciar a memoria com valor 0

    FILE *arquivo_entrada;
    FILE *arquivo_saida;

    
    if (argc == 5 && (strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-l") == 0))
    {
        printf("Argumentos válidos!\n\n");
    }
    else
    {
        printf("Erro: Abra o arquivo como o seguinte exemplo:\n");
        printf("./nomeExecutavel -p arqEntrada.ias.txt -m arqSaida.ias.txt\n");
        exit(1);
    }

    if ((arquivo_entrada = fopen(argv[2], "r")) == NULL)
    {
        printf("Erro ao abrir o aquivo!");
        exit(1);
    }

    carregar_memoria(arquivo_entrada, memory);

    if ((arquivo_saida = fopen(argv[4], "w")) == NULL)
    {
            printf("Erro ao abrir o aquivo!");
            exit(1);
    }

    imprime_memory(arquivo_saida, memory, inicio_memory);

    memory = inicio_memory;
    free(memory);
    fclose(arquivo_saida);
    fclose(arquivo_entrada);
    printf("Fim do programa, sucesso!\n\n");

    return 0;
}