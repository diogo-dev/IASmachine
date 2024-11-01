all:
	gcc IAS.c -g
	./a.out -p bolha.ias.txt -l 51 -m saidaBolha.ias.txt
	./a.out -p fatorial.ias.txt -l 51 -m saidaFatorial.ias.txt
	./a.out -p buscaBinaria.ias.txt -l 51 -m saidaBuscaBinaria.ias.txt
	./a.out -p determinante.ias.txt -l 21 -m saidaDeterminante.ias.txt
	./a.out -p areaTrapesio.ias.txt -l 11 -m saidaTrapesio.ias.txt

compilar:
	gcc IAS.c -g

bolha:
	./a.out -p bolha.ias.txt -l 51 -m saidaBolha.ias.txt

fatorial:
	./a.out -p fatorial.ias.txt -l 51 -m saidaFatorial.ias.txt

buscaBinaria:
	./a.out -p buscaBinaria.ias.txt -l 51 -m saidaBuscaBinaria.ias.txt

determinante:
	./a.out -p determinante.ias.txt -l 21 -m saidaDeterminante.ias.txt

trapesio:
	./a.out -p areaTrapesio.ias.txt -l 11 -m saidaTrapesio.ias.txt