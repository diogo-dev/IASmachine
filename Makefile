all:
	gcc IASmain.c IASfuncoes.c -g
	./a.out -p exercicios/bolha.ias.txt -l 51 -m resultados/saidaBolha.ias.txt
	./a.out -p exercicios/fatorial.ias.txt -l 51 -m resultados/saidaFatorial.ias.txt
	./a.out -p exercicios/buscaBinaria.ias.txt -l 51 -m resultados/saidaBuscaBinaria.ias.txt
	./a.out -p exercicios/determinante.ias.txt -l 21 -m resultados/saidaDeterminante.ias.txt
	./a.out -p exercicios/areaTrapesio.ias.txt -l 11 -m resultados/saidaTrapesio.ias.txt
compilar:
	gcc IASmain.c IASfuncoes.c -g

bolha:
	./a.out -p exercicios/bolha.ias.txt -l 51 -m resultados/saidaBolha.ias.txt

fatorial:
	./a.out -p exercicios/fatorial.ias.txt -l 51 -m resultados/saidaFatorial.ias.txt

buscaBinaria:
	./a.out -p exercicios/buscaBinaria.ias.txt -l 51 -m resultados/saidaBuscaBinaria.ias.txt

determinante:
	./a.out -p exercicios/determinante.ias.txt -l 21 -m resultados/saidaDeterminante.ias.txt

trapesio:
	./a.out -p exercicios/areaTrapesio.ias.txt -l 11 -m resultados/saidaTrapesio.ias.txt