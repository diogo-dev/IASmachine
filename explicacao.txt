Explicação dos exercícios feitos em código da máquina IAS:

fatorial:

M(1): Valor inicial N, depois N-1, N-2,...
M(2): Contém o valor 1
M(10): Contém o valor 0
M(5): Ontem o resultado será armazenado

LOAD MQ,M(1)
LOAD MQ
STOR M(5) 	// colocar em M(5) o valor inicial de M(1) = N
SUB M(2)  	//varificar se N vale 1
JUMP +M(508,20:39)  //Se sim, faz o jump, senão continua
LOAD M(2)	
SUB M(1)  	// Subtrair valor atual de M(1) com 1, N-1, (N-1)-1, ... Que seria decrementar N
STOR M(1) 	// armazenar esse novo valor em M(1)
LOAD MQ,M(5) 
MUL M(1)  	//multipicar o que está em M(5) pelo valor atual de M(1)
LOAD MQ
STOR M(5) 	// guardar o valor dessa multiplicação em M(5)
LOAD M(1) 
SUB M(2)  	// Varificar se valor atual de N é 1
JUMP M(503,0:19)
SUB M(10) 	// verifica se o Valor de N é 0
JUMP +M(510,20:39)
ADD M(11) 	// Se N = 0, adiciona 1, senão acaba o programa
STOR M(5)
EXIT		//fim
 

determinante: b² - 4.a.c

M(1): a
M(2): b
M(3): c
M(4): 4
M(5): resultado
M(7): b²

LOAD MQ,M(2) // MQ <- b
MUL M(2)     // MQ <- b²
LOAD MQ      // AC <- b²
STOR M(7)    // colocar em M(7)
LOAD MQ,M(4) // MQ <- 4
MUL M(1)     // MQ <- 4.a
MUL M(3)     // MQ <- 4.a.c
LOAD MQ      // AC <- 4.a.c
SUB M(7)     //AC <- b² - 4.a.c
STOR M(5)    // coloca o resultado em M(5)
EXIT 	     // fim

trapesio: (B+b).h/2
clocks: 4 - 6 - 9 - 13 - 14(bolha) - 16 - 19 - 21 - 24 - 28 - 29 - 30

M(1): B
M(2): b
M(3): (B+b)
M(4): h
M(5): resultado

LOAD M(1)	// AC <- B
ADD M(2)	// AC<- B+b
STOR M(3)	// armazenar em M(3)
LOAD MQ,M(3)	// MQ <- B+b
MUL M(4)	// MQ <- (B+b).h
LOAD MQ		// AC <- (B+b).h
RSH		// AC <- (B+b).h/2
STOR M(5)	// Armazenar em M(5)
EXIT		// fim

Ordenação bolha:

M(1) até M(5): valores a serem ordenados
M(8): enderenço do primeiro elemento (valor inicial 2)
M(9): endereço do segundo elemento   (valor inicial 1)
M(10): valor 1 (guardar o valor do endereço inicial do 1 elemento e usado para operações de incremento)
M(11): valor 2 (guardar o valor do endereço inicial do 2 elemento)
M(12): valor -1
M(13): valor -1 //flag de troca, caso ocorra alguma troca seu valor vai para 0, senão continua em -1
M(20): valor -5


LOAD M(1)
SUB M(2)
JUMP +M(506,20:39)
LOAD M(1) --------------------------------------------------------
LOAD MQ,M(2)
STOR M(2)
LOAD MQ		trocar os valores caso o 1 elemento > 2 elemento
STOR M(1)
LOAD M(13)
ADD M(10)
STOR M(13) -------------------------------------------------------
LOAD M(9)
ADD M(20)
JUMP +M(515,20:39) // verifica se o 2 elemento tem endereço com valor 5. Se tiver pula, senão continua
LOAD M(8)---------------------------------------------------------
LOAD MQ,M(9)
ADD M(10)
STOR M(8)	Incrementar os enderenços dos elementos 1 e 2
LOAD MQ
ADD M(10)
STOR M(9)---------------------------------------------------------
STOR M(501,28:39)
STOR M(503,8:19)
STOR M(503,28:39)
LOAD M(8)	Modificar os endereços dos elementos 1 e 2 para seus endereços atualizados
STOR M(501,8:19)
STOR M(502,28:39)
STOR M(504,28:39)------------------------------------------------
JUMP M(501,0:19) //voltar para o inicio e repetir as comparações
LOAD M(13) // varificar se a flag de troca foi modificada
JUMP +M(517,0:19) // caso ela tenha sido modificada, faz-se o jump. Senão, acaba o programa
EXIT // fim
LOAD M(10)-------------------------------------------------------
STOR M(8)
LOAD M(11)
STOR M(9)	Resultaurar os valores dos endereços dos 2 elementos
LOAD M(12)
STOR M(13)
LOAD M(9) ---------------------------------------------------------
STOR M(501,28:39)
STOR M(503,8:19)
STOR M(503,28:39)
LOAD M(8)	Resultaurar os valores dos endereços dos 2 elementos
STOR M(501,8:19)
STOR M(502,28:39)
STOR M(504,28:39)--------------------------------------------------
JUMP M(501,0:19) // voltar ao inicio

Busca Binária:

M(1) até M(11): Vetor com valores ordenados
M(13): Valor que queremos buscar 
M(14): Início (valor 1)
M(15): Final (valor 11)
M(16): Meio ( (inicio+fim) / 2 ) e contém a resposta final do algoritmo (o índice do número no vetor ou -1).
M(17): valor 1
M(18): Armazena valor temporário
M(21): valor -1 

LOAD M(14)
ADD M(15)			Calculando o valor do meio
RSH
STOR M(16)----------------------------------------------------------------------------		 
STOR M(503,28:39)
LOAD M(1)	Descobrindo se vetor[meio] > ou < que numero escolhido para ser buscado
SUB M(13)
JUMP +M(510,0:19) --------------------------------------------------------------------
LOAD M(17)			SE VETOR[MEIO] FOR MENOR
SUB M(16)	
STOR M(15)		Calcula o valor para o novo fim, ou seja, fim = meio-1	
LOAD M(14)		
ADD M(17)	Calcula início - fim para saber se o programa chegou ao fim ou não
SUB M(15)	SE INICIO - FIM > 0 voltamos para o início do programa
JUMP +M(501,0:19)----------------------------------------------------------------------
LOAD M(21)			SE INICIO - FIM < 0
STOR M(16)	Atribuímos -1 para M(16) e finalizamos o programa
EXIT-----------------------------------------------------------------------------------
LOAD M(16)			SE VETOR[MEIO] FOR MAIOR OU IGUAL
STOR M(511,8:19)	
LOAD M(1)	Para podermos descobrir se o valor do índice meio é igual ou não ao
ADD M(17)    valor que queremos buscar, calculamos o seguinte: M(13) - vetor[meio]+1.
STOR M(18)   E logo em seguida utilizamos o JUMP condicional: Se for < 0 encontramos
LOAD M(18)   o valor que buscamos e podemos encerrar o programa. Se for > 0 continuamos
SUB M(13)    o programa.
JUMP +M(514,20:39)---------------------------------------------------------------------
EXIT
LOAD M(16)-----------------------------------------------------------------------------
ADD M(17)			SE O VALOR NÃO É O QUE BUSCAMOS
STOR M(14)
LOAD M(14)	Calculamos o novo valor para o início, ou seja, início = meio+1
ADD M(17)    E calcula início - fim novamente para saber se o programa chegou ao fim
SUB M(15)    SE INICIO - FIM > 0 voltamos para o início do programa	
JUMP +M(501,0:19)-----------------------------------------------------------------------
LOAD M(21)				SE INICIO - FIM < 0
STOR M(16)		Atribuímos -1 para M(16) e finalizamos o programa
EXIT-----------------------------------------------------------------------------------

Multiplicação de Matriz:

M(1) a M(4): Valores da matriz A
M(6) a M(9): Valores da matriz B
M(11): 2 - N - Tamanho da matriz
M(12): 1 - N-1 - Contador para cada linha da matriz
M(13): 1 - N-1 - Contador final do algoritmo
M(14): 1 - Posição atual da matriz A
M(15): 6 - Posição atual da matriz B
M(16): 18 - Posição atual da matriz A*B
M(18) a M(21): Valores da matriz A*B
M(23): 1 - Constante
M(24): 1 - N-1 - Constante com tamanho da matriz - 1
M(25) 6 - Constante contendo a posição inicial da matriz B
M(26): -1 - Constante

LOAD M(14)
STOR M(504,8:19)
LOAD M(15)
STOR M(504,28:39)	
LOAD M(16) 		Calculando A[posA]*B[PosB] = AB[PosAB]	
STOR M(505,28:39)
LOAD MQ,M(1)
MUL M(1)
LOAD MQ
STOR M(1)--------------------------------------------------------------------------
LOAD M(14)
ADD M(23)
STOR M(14)	Atualizando as váriaveis das posições A e B para realizar a segunda 
LOAD M(15)	parte da multiplicação
ADD M(11)
STOR M(15)--------------------------------------------------------------------------
LOAD M(14)
STOR M(512,28:39)
LOAD M(15) 
STOR M(513,8:19)
LOAD M(16) 		Calculando a segunda parte da multiplicação de matriz
STOR M(514,8:19)	e somando ao valor que já está em AB[PosAB] 
STOR M(514,28:39)
LOAD MQ,M(1)
MUL M(1)
LOAD MQ
ADD M(1)
STOR M(1)----------------------------------------------------------------------------
LOAD M(13)
ADD M(26)
STOR M(13)
LOAD M(14)	Atualizando as váriaveis para podermos utilizá-las nas próximas 
ADD M(26)	iterações e decrementando o contador para sabermos quando a
STOR M(14)	linha da matriz finalizar e podermos ir para a próxima
LOAD M(15)
ADD M(26)
STOR M(15)
LOAD M(16)	Jump condicional para caso o contador linha seja menor que 0 seguimos 
ADD M(23)	para a próxima instrução, caso contrário volta para o começo do 
STOR M(16)	programa e refaz tudo.
LOAD M(13)
JUMP +M(501,0:19)---------------------------------------------------------------------
LOAD M(12)			SE CONTADOR < 0
ADD M(26)		
STOR M(12)	Decrementamos o contador Matriz e resetamos o contador linha para ser 
LOAD M(13)	usado novamente e preparamos as variáveis de posição para a próxima 
ADD M(11)	linha da matriz AB
STOR M(13)
LOAD M(14)
ADD M(11)
STOR M(14)
LOAD M(25)	Jump condicional para caso o contador linha seja menor que 0 o 
STOR M(15)	o programa finaliza, caso contrário volta para o começo do programa
LOAD M(12)	e refaz tudo
JUMP +M(501,0:19)----------------------------------------------------------------------
EXIT
