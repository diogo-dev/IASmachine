#include <stdio.h>
#include <string.h>

void finding_address(char *str_lida, char *address, char *no_address)
{
    int i = 0;
    int j = 0;
    int k = 0;

    while (i < strlen(str_lida) && str_lida[i] != ')' && str_lida[i] != ',')
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
    if (strcmp(address, "-") == 0)
        strcpy(no_address, "000000000000");
}

int main()
{
    char str[20] = "LOAD-M(10123)";
    char str2[30] = "JUMP-M(123456,0:19)";
    char str3[10] = "EXIT";
    char address[13] = "-"; //caracter especial que verifica se na instrucao foi passado algo ao address ou nao
    char no_address[13];
    finding_address(str, address, no_address);
    printf("%s\n", address);
    printf("\n");
    finding_address(str2, address, no_address);
    printf("%s\n", address);
    printf("\n");
    strcpy(address, "-");
    finding_address(str3, address, no_address);
    printf("%s\n", address);
    printf("%s\n", no_address);

    return 0;
}