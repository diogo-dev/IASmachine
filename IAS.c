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
#define BITWISE_SIZE 4095 //0b111111111111

typedef struct IAS
{
    //Program Control Unit
    int MAR = 0; //MAR(12)
    int IR = 0; //IR(8)
    int IBR = 0; //IBR(20)
    int PC = 0; //PC(12)

    //Arithmetic-logic unit (ALU)
    long AC = 0; //AC(40)
    long MQ = 0; //MQ(40)
    long MBR = 0; //MBR(40)
}banco_de_registradores;

//implementar o fluxograma meio que parecido com o do repositorio do github c++
//O UC vai ser usar essas funções do fluxograma (são as condições no fluxo -- triangulos)
//a ULA ainda nao sei direito

void fetch_cycle(banco_de_registradores *br, unsigned char *memory)
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
        br->MAR = br->IBR & BITWISE_SIZE;
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
        //Left instruction required ?
        

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

int main(int argc, char *argv[])
{
    unsigned char *memory = (unsigned char *)malloc(4096 * 5 * sizeof(char *));
    unsigned char *inicio_memory; 

    inicio_memory = memory; //variavel inicio_memory recebe o endereco do inicio da memoria

    iniciar_memory(memory, inicio_memory); // iniciar a memoria com valor 0

    FILE *arq;
    FILE *arquivo_saida;

    char linha_lida[BUFFER_SIZE];
    char linha_lida2[BUFFER_SIZE];
    char linha_lida_backup[BUFFER_SIZE];
    char linha_lida2_backup[BUFFER_SIZE];

    
    if (argc == 5 && (strcmp(argv[1], "-p") == 0) && (strcmp(argv[3], "-m") == 0))
    {
        printf("Argumentos válidos!\n");
    }
    else
    {
        printf("Erro: Abra o arquivo como o seguinte exemplo:\n");
        printf("./nomeExecutavel -p arqEntrada.ias.txt -m arqSaida.ias.txt\n");
        exit(1);
    }

    if ((arq = fopen(argv[2], "r")) == NULL)
    {
        printf("Erro ao abrir o aquivo!");
        exit(1);
    }

    int i = 0;
    while (fgets(linha_lida, BUFFER_SIZE, arq) != NULL)
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
        else if((fgets(linha_lida2, BUFFER_SIZE, arq) != NULL))
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

    if ((arquivo_saida = fopen(argv[4], "w")) == NULL)
    {
            printf("Erro ao abrir o aquivo!");
            exit(1);
    }

    imprime_memory(arquivo_saida, memory, inicio_memory);

    memory = inicio_memory;
    free(memory);
    fclose(arquivo_saida);
    fclose(arq);
    printf("Fim do programa, sucesso!\n");

    return 0;
}