all:
	gcc IAS.c -g
	./a.out -p bolha.ias.txt -l 501 -m saidaBolha.ias.txt
	./a.out -p fatorial.ias.txt -l 501 -m saidaFatorial.ias.txt
	./a.out -p buscaBinaria.ias.txt -l 501 -m saidaBuscaBinaria.ias.txt
	./a.out -p matMult.ias.txt -l 501 -m saidaMatMult.ias.txt
	./a.out -p determinante.ias.txt -l 501 -m saidaDeterminante.ias.txt
	./a.out -p areaTrapesio.ias.txt -l 501 -m saidaTrapesio.ias.txt
	./a.out -p testeLOAD.ias.txt -l 501 -m saidaLOAD.ias.txt
	./a.out -p testeULA.ias.txt -l 501 -m saidaULA.ias.txt
	./a.out -p testeJUMP.ias.txt -l 501 -m saidaJUMP.ias.txt
	./a.out -p testeSTOR.ias.txt -l 501 -m saidaSTOR.ias.txt

compilar:
	gcc IAS.c -g

bolha:
	./a.out -p bolha.ias.txt -l 501 -m saidaBolha.ias.txt

fatorial:
	./a.out -p fatorial.ias.txt -l 501 -m saidaFatorial.ias.txt

buscaBinaria:
	./a.out -p buscaBinaria.ias.txt -l 501 -m saidaBuscaBinaria.ias.txt

matMult:
	./a.out -p matMult.ias.txt -l 501 -m saidaMatMult.ias.txt

determinante:
	./a.out -p determinante.ias.txt -l 501 -m saidaDeterminante.ias.txt

trapesio:
	./a.out -p areaTrapesio.ias.txt -l 501 -m saidaTrapesio.ias.txt

load:
	./a.out -p testeLOAD.ias.txt -l 501 -m saidaLOAD.ias.txt

ula:
	./a.out -p testeULA.ias.txt -l 501 -m saidaULA.ias.txt

jump:
	./a.out -p testeJUMP.ias.txt -l 501 -m saidaJUMP.ias.txt

stor:
	./a.out -p testeSTOR.ias.txt -l 501 -m saidaSTOR.ias.txt

pipelineCompilar:
	gcc IASpipeline.c -o pipeline

pipelineLOAD:
	./pipeline -p testeLOAD.ias.txt -l 501 -m saidaPipelineLOAD.ias.txt

pipelineULA:
	./pipeline -p testeULA.ias.txt -l 501 -m saidaPipelineULA.ias.txt

pipelineJUMP:
	./pipeline -p testeJUMP.ias.txt -l 501 -m saidaPipelineJUMP.ias.txt

pipelineFatorial:
	./pipeline -p fatorial.ias.txt -l 501 -m saidaPipelineFatorial.ias.txt

pipelineDeterminante:
	./pipeline -p determinante.ias.txt -l 501 -m saidaPipelineDeterminante.ias.txt

pipelineTrapesio:
	./pipeline -p areaTrapesio.ias.txt -l 501 -m saidaPipelineTrapesio.ias.txt