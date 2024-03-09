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

typedef enum{
    soma,
    subtracao,
    multiplicacao,
    divisao,
    shift_direita,
    shift_esquerda
}operacoesULA;

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
    desnecessario,
    necessario,
    nenhum
}accesso_memoria; 

void manter_registradores(banco_de_registradores *br)
{
    br->MAR = br->MAR;
    br->IR = br->IR;
    br->IBR = br->IBR;
    br->PC = br->PC;

    br->AC = br->AC;
    br->MQ = br->MQ;
    br->MBR = br->MBR;
}

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

void printar_registradores(banco_de_registradores *br)
{
    printf("MAR: %d\n", br->MAR);
    printf("IR: %d\n", br->IR);
    printf("IBR: %d\n", br->IBR);
    printf("PC: %d\n", br->PC);

    printf("AC: %ld\n", br->AC);
    printf("MQ: %ld\n", br->MQ);
    printf("MBR: %ld\n\n", br->MBR);
}

void UC(banco_de_registradores *br, instrucoesIAS *instrucaoIAS) 
{ 
    // Na unidade de controle, nos apenas marcamos que instrucao(opcode) estamos realizando utilizando uma flag
    // para que assim seja possivel realizar a busca dos operandos e a execucao para aquela instrucao em especifico

    if(br->IR == 0b00001010) { 
        *instrucaoIAS = LOAD_MQ;
    }
    else if(br->IR == 0b00001001) { 
        *instrucaoIAS = LOAD_MQ_MX;
    }
    else if(br->IR == 0b00100001 ) { 
        *instrucaoIAS = STOR_MX;
    }
    else if(br->IR == 0b00000001) { 
        *instrucaoIAS = LOAD_MX;
    }
    else if(br->IR == 0b00000010) { 
        *instrucaoIAS = LOAD_MINUS_MX;
    }
    else if(br->IR == 0b00000011) { 
        *instrucaoIAS = LOAD_PIPE_MX;
    }
    else if(br->IR == 0b00000100) { 
        *instrucaoIAS = LOAD_MINUS_PIPE_MX;
    }
    else if(br->IR == 0b00001101) { 
        *instrucaoIAS = JUMP_MX_0_19;
    }
    else if(br->IR == 0b00001110) { 
        *instrucaoIAS = JUMP_MX_20_39;
    }
    else if(br->IR == 0b00001111) { 
        *instrucaoIAS = JUMPC_MX_0_19;
    }
    else if(br->IR == 0b00010000) { 
        *instrucaoIAS = JUMPC_MX_20_39;
    }
    else if(br->IR == 0b0000101) { 
        *instrucaoIAS = ADD_MX;
    }
    else if(br->IR == 0b00000111) { 
        *instrucaoIAS = ADD_PIPE_MX;
    }
    else if(br->IR == 0b0000110) { 
        *instrucaoIAS = SUB_MX;
    }
    else if(br->IR == 0b00001000) { 
        *instrucaoIAS = SUB_PIPE_MX;
    }
    else if(br->IR == 0b00001011) { 
        *instrucaoIAS = MUL_MX;
    }
    else if(br->IR == 0b00001100) { 
        *instrucaoIAS = DIV_MX;
    }
    else if(br->IR == 0b00010100) { 
        *instrucaoIAS = LSH;
    }
    else if(br->IR == 0b00010101) { 
        *instrucaoIAS = RSH;
    }
    else if(br->IR == 0b00010010) { 
        *instrucaoIAS = STOR_MX_8_19;
    }
    else if(br->IR == 0b00010011) { 
        *instrucaoIAS = STOR_MX_28_39;
    }
    else if(br->IR == 0b11111111) { 
        *instrucaoIAS = EXIT;
    }
    else{
        *instrucaoIAS = NENHUM;
    }
}

void ULA(banco_de_registradores *br, operacoesULA operacao, long operando_memoria, __int128_t *resultado)
{
    long temp;

    if (operacao == soma)
    {
        *resultado = br->AC + operando_memoria;
    }
    else if (operacao == subtracao)
    {
        *resultado = operando_memoria - br->AC;
    }
    else if (operacao == multiplicacao)
    {
        *resultado = operando_memoria * br->MQ;
    }
    else if (operacao == divisao)
    {
        // Put the quotient in MQ and the remainder in AC
        *resultado = br->AC / operando_memoria;
        *resultado <<= 40;
        temp = br->AC % operando_memoria;
        temp = temp & BITWISE_40;  // caso temp seja negativo
        *resultado = *resultado | temp;
    }
    else if (operacao == shift_esquerda)
    {
        *resultado = br->AC << 1; //Multiplicar por 2
    }
    else
    {
        *resultado = br->AC >> 1; //Dividir por 2
    }
}

instrucoesIAS decodificacao(banco_de_registradores *br, accesso_memoria acesso)  
{
    instrucoesIAS instrucaoIAS;
    // setar o IR e MAR
    br->IR = 0;
    br->MAR = 0;
    if(acesso == desnecessario)
    { 
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
    else if(acesso == necessario) 
    { 
        //Left instruction required ?
        int temp3 = br->MBR & BITWISE_20;
        if(temp3 == 0)
        {
            // instrucao da direita nao existe
            br->MBR = br->MBR >> 20;
            //MAR <- endereco
            br->MAR = br->MBR & BITWISE_12;
            br->MBR = br->MBR >> 12;
            //IR <- opcode
            br->IR = br->MBR;
        
            //PC <- PC + 1
            br->PC = br->PC + 1;
            //setar MBR
            br->MBR = 0;
        }
        else
        {
            //instrucao da direita exite
            //IBR <- instrucao da direita
            br->IBR = br->MBR & BITWISE_20;
            br->MBR = br->MBR >> 20;
            //MAR <- endereco
            br->MAR = br->MBR & BITWISE_12;
            br->MBR = br->MBR >> 12;
            //IR <- opcode
            br->IR = br->MBR;
            
            //setar MBR
            br->MBR = 0;
        }
    }
    else
    {
        manter_registradores(br);
    }

    UC(br, &instrucaoIAS);
    return instrucaoIAS;
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

void busca(banco_de_registradores * br, unsigned char * memory, accesso_memoria *acesso, unsigned char *inicio_memory)
{ 
    if(br->IBR != 0) 
    { 
        //Memory access not required
        //Marcamos com uma flag para sabermos o que fazer na decodificacao
        *acesso = desnecessario; 
    }
    else 
    { 
        *acesso = necessario;
        //Marcamos com uma flag para sabermos o que fazer na decodificacao
        barramento_memoria_reg(br, memory, inicio_memory);
    }
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

    if (strcmp(str_lida, "LOAD MQ") == 0) 
    {
        *opcode = 0b00001010;
    }
    else if (strcmp(str_lida, "LOAD MQ,M") == 0) 
    {
        *opcode = 0b00001001;
    }
    else if (strcmp(str_lida, "STOR M") == 0) 
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
    else if (strcmp(str_lida, "LOAD M") == 0) 
    {
        *opcode = 0b00000001;
    }
    else if (strcmp(str_lida, "LOAD -M") == 0) 
    {
        *opcode = 0b00000010;
    }
    else if (strcmp(str_lida, "LOAD |M") == 0) 
    {
        *opcode = 0b00000011;
    }
    else if (strcmp(str_lida, "LOAD -|M") == 0) 
    {
        *opcode = 0b00000100;
    }
    else if (strcmp(str_lida, "JUMP M") == 0) 
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
    else if (strcmp(str_lida, "JUMP +M") == 0) 
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
    else if (strcmp(str_lida, "ADD M") == 0) 
    {
        *opcode = 0b00000101;
    }
    else if (strcmp(str_lida, "ADD |M") == 0)
    {
        *opcode = 0b00000111;
    }
    else if (strcmp(str_lida, "SUB M") == 0) 
    {
        *opcode =  0b00000110;
    }
    else if (strcmp(str_lida, "SUB |M") == 0) 
    {
        *opcode = 0b00001000;
    }
    else if (strcmp(str_lida, "MUL M") == 0) 
    {
        *opcode = 0b00001011;
    }
    else if (strcmp(str_lida, "DIV M") == 0) 
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
    memory = iniciar_memory; //volta o ponteiro da memoria para o inicio dela
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

long obter_operando(banco_de_registradores *br, unsigned char *memory, unsigned char *inicio_memory)
{
    //funcao parecida com o barramento, mas retorna a palavra que foi lida na memoria
    memory = inicio_memory;
    memory = memory + 5 * (br->MAR - 1); //endereco esta em MAR

    long palavra_temp = 0;
    int j = 0;

    while(j < 5)
    {
        palavra_temp = palavra_temp << 8;
        palavra_temp = palavra_temp | *memory;
        memory++;
        j++;
    }

    return palavra_temp;
}

int eh_negativo(long *operando)
{
    int bit_sinal = *operando >> 39;
    if (bit_sinal == 1)
        return 1; //true
    else
        return 0; //false
}

void busca_de_operandos(banco_de_registradores *br, unsigned char *memory, instrucoesIAS instrucaoIAS, long *operando, unsigned char *inicio_memory)
{
    if (instrucaoIAS == LOAD_MQ)
    {
        *operando = br->MQ; //O operando eh o que esta em MQ
    } 
    else if (instrucaoIAS == LOAD_MQ_MX || instrucaoIAS == LOAD_MX || instrucaoIAS == ADD_MX || 
    instrucaoIAS == SUB_MX || instrucaoIAS == MUL_MX || instrucaoIAS == DIV_MX)
    {
        *operando = obter_operando(br, memory, inicio_memory);
        if (eh_negativo(operando))
        {
            //colocar 0 no bit de sinal 
            *operando = *operando & 0b0111111111111111111111111111111111111111;
            *operando = -*operando;
        }
    } 
    else if (instrucaoIAS == STOR_MX || instrucaoIAS == LSH || instrucaoIAS == RSH)
    {
        *operando = br->AC; //O operando seria o que esta no AC
    } 
    else if (instrucaoIAS == LOAD_MINUS_MX)
    {
        *operando = obter_operando(br, memory, inicio_memory); 
        if (eh_negativo(operando))
        {
            //colocar 0 no bit de sinal
            *operando = *operando & 0b0111111111111111111111111111111111111111;
        }
        else
        {
            // aqui o colocar 1 seria converter o valor do operando para negativo
            *operando = -*operando;
        }
    }
    else if (instrucaoIAS == LOAD_PIPE_MX || instrucaoIAS == ADD_PIPE_MX || instrucaoIAS == SUB_PIPE_MX)
    {
        *operando = obter_operando(br, memory, inicio_memory);
        if (eh_negativo(operando))
        {
            //colocar 0 no bit de sinal
            *operando = *operando & 0b0111111111111111111111111111111111111111;
        }
    }
    else if (instrucaoIAS == LOAD_MINUS_PIPE_MX)
    {
        *operando = obter_operando(br, memory, inicio_memory);
        if (eh_negativo(operando))
        {
            //colocar 0 no bit de sinal
            *operando = *operando & 0b0111111111111111111111111111111111111111; // |M(X)|
        }
        *operando = -*operando; // -|M(X)|
    }
    else if (instrucaoIAS == JUMP_MX_0_19 || instrucaoIAS == JUMP_MX_20_39 
    || instrucaoIAS == JUMPC_MX_0_19 || instrucaoIAS == JUMPC_MX_20_39)
    {
        *operando = br->MAR; //O operando do jumpo eh somento o endereco que esta em MAR
    }
    else if (instrucaoIAS == STOR_MX_8_19 || instrucaoIAS == STOR_MX_28_39)
    {  
        int novo_endereco = br->AC & BITWISE_12;
        *operando = novo_endereco;
    }
    else //EXIT e NENHUM
    {
        *operando = 0; 
    }
}

void escrevendo_na_memoria(banco_de_registradores *br, unsigned char *memory, unsigned char *inicio_memory, __int128_t dado)
{
    memory = inicio_memory;
    memory = memory + 5 * (br->MAR - 1); //endereco esta em MAR

    unsigned char palavra_8bits;
    for(int i = 4; i >= 0; i--)
    {
        palavra_8bits = (dado >> (8*i)) & UNSIGNED_CHAR_SIZE; 
        *memory = palavra_8bits;
         memory++;
    }
}

void execucao(banco_de_registradores *br, unsigned char *memory, instrucoesIAS instrucaoIAS, long operando, __int128_t *resultado, unsigned char *inicio_memory)
{
    //variavel "resultado" resultante da execucao da instrucao que sera mandada para a faze: escrita dos resultados

    if (instrucaoIAS == LOAD_MQ || instrucaoIAS == LOAD_MX || instrucaoIAS == LOAD_MINUS_MX 
    || instrucaoIAS == LOAD_PIPE_MX || instrucaoIAS == LOAD_MINUS_PIPE_MX)
    {
        *resultado = operando;
    }
    else if (instrucaoIAS == LOAD_MQ_MX)
    {
        *resultado = operando;
    }
    else if (instrucaoIAS == STOR_MX)
    {
        if (operando < 0)
        {
            operando = -operando;
            *resultado = 1;
            *resultado = *resultado << 39;
            *resultado = *resultado | operando;
        }
        else
        {
            *resultado = operando;
        }
    }
    else if (instrucaoIAS == JUMP_MX_0_19)
    {
        // atualizar PC
        *resultado = operando;
        //setar IBR
    }
    else if (instrucaoIAS == JUMP_MX_20_39)
    {
        //atualizar PC
        *resultado = operando;
        // IBR <- instrucao da direita
        long instrucao_direita = obter_operando(br, memory, inicio_memory) & BITWISE_20;
        *resultado <<= 20;
        *resultado = *resultado | instrucao_direita;
    }
    else if (instrucaoIAS == JUMPC_MX_0_19)
    {
        if (br->AC >= 0)
        {
            // atualizar PC
            *resultado = operando;
            //setar IBR
        }
        else
        {
            *resultado = br->PC;
        }
    }
    else if (instrucaoIAS == JUMPC_MX_20_39)
    {
        if (br->AC >= 0)
        {
            //atualizar PC
            *resultado = operando;
            // IBR <- instrucao da direita
            long instrucao_direita = obter_operando(br, memory, inicio_memory) & BITWISE_20;
            *resultado <<= 20;
            *resultado = *resultado | instrucao_direita;
        }
        else
        {
            *resultado = br->PC;
        }
    }
    else if (instrucaoIAS == ADD_MX || instrucaoIAS == ADD_PIPE_MX)
    {
        //chamar ULA
        ULA(br, soma, operando, resultado);
    }
    else if (instrucaoIAS == SUB_MX || instrucaoIAS == SUB_PIPE_MX)
    {
        ULA(br, subtracao, operando, resultado);
    }
    else if (instrucaoIAS == MUL_MX)
    {
        ULA(br, multiplicacao, operando, resultado);
    }
    else if (instrucaoIAS == DIV_MX)
    {
        ULA(br, divisao, operando, resultado);
    }
    else if (instrucaoIAS == LSH)
    {
        ULA(br, shift_esquerda, operando, resultado);
    }
    else if (instrucaoIAS == RSH)
    {
        ULA(br, shift_direita, operando, resultado);
    }
    else if (instrucaoIAS == STOR_MX_8_19)
    {
        long palavra_temp = obter_operando(br, memory, inicio_memory);
        int temp1 = palavra_temp & BITWISE_20;
        palavra_temp = palavra_temp >> 32;
        int temp2 = palavra_temp;

        *resultado = temp2;
        *resultado <<= 12;
        *resultado |= operando;
        *resultado <<= 20;
        *resultado |= temp1;
    }
    else if (instrucaoIAS == STOR_MX_28_39)
    {
        *resultado = obter_operando(br, memory, inicio_memory) >> 12;
        *resultado <<= 12;
        *resultado |= operando;

        if (br->PC == br->MAR)
        {
            if (br->IBR != 0)
            {
                //atualizar IBR
                br->IBR = *resultado & BITWISE_20;
            }
        }
    }
    else if (instrucaoIAS == EXIT)
    {
        //algo
    }
    else //NENHUM
    {
        //algo
    }
}

void escrita_dos_resultados(banco_de_registradores *br, unsigned char *memory, instrucoesIAS instrucaoIAS, long resultado, unsigned char *inicio_memory)
{
    //variavel "resultado" resultante da execucao da instrucao que sera mandada para a faze: escrita dos resultados

    if (instrucaoIAS == LOAD_MQ || instrucaoIAS == LOAD_MX || instrucaoIAS == LOAD_MINUS_MX 
    || instrucaoIAS == LOAD_PIPE_MX || instrucaoIAS == LOAD_MINUS_PIPE_MX)
    {
        br->AC = resultado;
    }
    else if (instrucaoIAS == LOAD_MQ_MX)
    {
        br->MQ = resultado;
    }
    else if (instrucaoIAS == STOR_MX)
    {
        escrevendo_na_memoria(br, memory, inicio_memory, resultado);
    }
    else if (instrucaoIAS == JUMP_MX_0_19)
    {
        // atualizar PC
        br->PC = resultado;
        //setar IBR
        br->IBR = 0;
    }
    else if (instrucaoIAS == JUMPC_MX_0_19)
    {
        if (br->AC >= 0)
        {
            // atualizar PC
            br->PC = resultado;
            //setar IBR
            br->IBR = 0;
        }
    }
    else if (instrucaoIAS == JUMP_MX_20_39)
    {
        // IBR <- instrucao da direita
        br->IBR = resultado & BITWISE_20;
        //atualizar PC
        resultado >>= 20;
        br->PC = resultado;
    }
    else if (instrucaoIAS == JUMPC_MX_20_39)
    {
        if (br->AC >= 0)
        {
            // IBR <- instrucao da direita
            br->IBR = resultado & BITWISE_20;
            //atualizar PC
            resultado >>= 20;
            br->PC = resultado;
        }
    }
    else if (instrucaoIAS == ADD_MX || instrucaoIAS == ADD_PIPE_MX || instrucaoIAS == SUB_MX 
    || instrucaoIAS == SUB_PIPE_MX || instrucaoIAS == LSH || instrucaoIAS == RSH)
    {
        br->AC = resultado;
    }
    else if (instrucaoIAS == MUL_MX)
    {
        //colocar os bits mais significativos em AC e os menos significativos em MQ
        if (resultado < 0)
        {
            br->MQ = resultado;
        }
        else
        {
            br->MQ = resultado & BITWISE_40;
            resultado >>= 40;
            br->AC = resultado;
        }
        //Para juntar AC com MQ a fim de ver o resultado da multiplicacao,
        //esse resultado tende de ser maior que o numero (em decimal): 549755813888 = 0b1000000000000000000000000000000000000000
        //Vamos so checar MQ
    }
    else if (instrucaoIAS == DIV_MX)
    {
        if (resultado < 0)
        {
            long temp = resultado & BITWISE_40; //resto da divisao
            if(eh_negativo(&temp))
            {
                int temp2 = temp;
                temp2 = ~temp2;
                temp2++;
                br->AC = temp2; //resto da divisao
            }
            else
            {
                br->AC = temp; //resto da divisao
            }
            resultado >>= 40;
            br->MQ = resultado; // quociente da divisao
        }
        else
        {
            br->AC = resultado & BITWISE_40; //resto da divisao
            resultado >>= 40;
            br->MQ = resultado; // quociente da divisao
        }
    }
    else if (instrucaoIAS == STOR_MX_8_19 || instrucaoIAS == STOR_MX_28_39)
    {
        escrevendo_na_memoria(br, memory, inicio_memory, resultado);
    }
    else if (instrucaoIAS == EXIT)
    {
        //algo
    }
    else //NENHUM
    {
        //algo
    }
}

/*void ciclo_das_instrucoes(banco_de_registradores br, unsigned char *memory, unsigned char *inicio_memory)
{
    long operando; //retornado na BO
    __int128_t resultado; //retornado na EX e escrito na ER

    instrucoesIAS instrucaoIAS = NENHUM;
    accesso_memoria acesso;

    while(instrucaoIAS != EXIT)
    {
        busca(&br, memory, &acesso, inicio_memory);
        printf("Busca, acesso memoria: %d\n\n", acesso);
        printar_registradores(&br);

        decodificacao(&br, acesso, &instrucaoIAS);
        printf("Decodificacao, instrucao IAS: %d\n\n", instrucaoIAS);
        printar_registradores(&br);

        busca_de_operandos(&br, memory, instrucaoIAS, &operando, inicio_memory);
        printf("Busca de operando, operando: %ld\n\n", operando);

        execucao(&br, memory, instrucaoIAS, operando, &resultado, inicio_memory);
        printf("Execucao...\n\n");

        escrita_dos_resultados(&br, memory, instrucaoIAS, resultado, inicio_memory);
        printf("Escrita dos resultados:\n\n");
        printar_registradores(&br);
    }
}*/

void dependencia_RAW(long operando, __int128_t resultado, instrucoesIAS *instrucaoIAS)
{
    if (operando != resultado)
    {
        *instrucaoIAS = NENHUM;
    }
}

void pipeline(banco_de_registradores br, unsigned char *memory, unsigned char *inicio_memory)
{
    int clock = 1;
    int MAR_temp = 0;
    int PC_temp = 0;
    int IBR_temp = 0;
    long MQ_temp = 0;
    long AC_temp = 0;

    long operando = 0; //retornado na BO
    __int128_t resultado = 0; //retornado na EX e escrito na ER

    accesso_memoria acesso = nenhum;
    instrucoesIAS atual, passada1, passada2, passada3;
    atual = NENHUM; 
    passada1 = NENHUM; 
    passada2 = NENHUM; 
    passada3 = NENHUM;

    while(passada3 != EXIT)
    {
        printf("-----------------clock: %d-----------------\n", clock);
        passada3 = passada2;
        AC_temp = br.AC;
        MQ_temp = br.MQ;
        PC_temp = br.PC;
        IBR_temp = br.IBR;
        escrita_dos_resultados(&br, memory, passada3, resultado, inicio_memory);
        printf("Escrita dos resultados:\n\n");
        printar_registradores(&br);

        passada2 = passada1;
        if (passada2 == JUMPC_MX_0_19)
        {
            br.AC = AC_temp;
            br.PC = PC_temp;
        }
        else if (passada2 == JUMPC_MX_20_39)
        {
            br.AC = AC_temp;
            br.PC = PC_temp;
            br.MAR = MAR_temp;
        }
        else if (passada2 == STOR_MX_28_39)
        {
            br.PC = PC_temp;
            br.IBR = IBR_temp;
            br.MAR = MAR_temp;
        }

        execucao(&br, memory, passada2, operando, &resultado, inicio_memory);
        printf("Execucao...\n\n");

        passada1 = atual;
        br.MAR = MAR_temp;
        busca_de_operandos(&br, memory, passada1, &operando, inicio_memory);
        printf("Busca de operando, operando: %ld\n\n", operando);

        //dependencia_RAW();

        atual = decodificacao(&br, acesso);
        printf("Decodificacao, instrucao IAS: %d\n\n", atual);
        printar_registradores(&br);
        MAR_temp = br.MAR;

        busca(&br, memory, &acesso, inicio_memory);
        printf("Busca, acesso memoria: %d\n\n", acesso);
        printar_registradores(&br);
        clock++;
    }
}

int main(int argc, char *argv[])
{
    unsigned char *memory = (unsigned char *)malloc(4096 * 5 * sizeof(char *)); //memoria
    unsigned char *inicio_memory;
    inicio_memory = memory; //variavel inicio_memory recebe o endereco do inicio da memoria

    iniciar_memory(memory, inicio_memory); // iniciar a memoria com valor 0

    FILE *arquivo_entrada;
    FILE *arquivo_saida;

    
    /*if (argc == 6 && (strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-l") == 0))
    {
        printf("Argumentos válidos!\n\n");
    }
    else
    {
        printf("Erro: Abra o arquivo como o seguinte exemplo:\n");
        printf("./nomeExecutavel -p arqEntrada.ias.txt -m arqSaida.ias.txt\n");
        exit(1);
    }*/

    if ((arquivo_entrada = fopen("testeLOAD.txt", "r")) == NULL)
    {
        printf("Erro ao abrir o aquivo!");
        exit(1);
    }

    carregar_memoria(arquivo_entrada, memory);

    banco_de_registradores br;
    resetar_registradores(&br);
    //br.PC = atoi(argv[4]); //receber endereco da linha de comando
    br.PC = 501;
    
    pipeline(br, memory, inicio_memory);

    if ((arquivo_saida = fopen("saida.txt", "w")) == NULL)
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