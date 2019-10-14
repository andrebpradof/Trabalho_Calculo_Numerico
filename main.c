#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "matrizEsparsa.h"

#define tam 200 //tamanho da matriz

//Função que calcula o módulo de um número (double)
double modulo(double numero){
	if(numero < 0){
		numero = numero * (- 1);
	}
	return numero;
}

int main(){
	// ariáveis que armazenam o tam do vetor e o número máximo de iterações
	int itmax = 2*tam;

	//Erro Epsilon = 10^(-10)
	double e = pow(10,-10);

	matrizEsparsa *matriz;
	//Cria a matriz utilizando a função feita na biblioteca matriz_esparsa.c
	matriz = gerarMatriz(tam,tam);

	int i;
	//Seta a matriz como indicado no enunciado
	for(i=1; i<= tam; i++){
		addElemento(matriz,i,i,5);
		addElemento(matriz,i,i+1,-1);
		addElemento(matriz,i+1,i,-1);
		addElemento(matriz,i,i+3,-1);
		addElemento(matriz,i+3,i,-1); 
	}

	float vetor_b[tam];
	//Seta vetor b como determinado no enunciado
	for(int j = 1; j <= tam; j++){
		vetor_b[j-1] = 0;
		for(int m = 1; m <= tam; m++){
			vetor_b[j-1] += pegarElemento(matriz,j,m);
		}
	}

	//Cria o vetor x(0) e seta como nulo
	double vetor_x[tam];
	for(int l = 0; l < tam; l++){
		vetor_x[l] = 0.0;
	}
	//Inicializa variáveis e vetor auxiliares
	int j;
	int k = 0;
	double erro = 10; //Erro começa com 10 (núemro qualquer maior que zero)para a primeira iteração do while
	double vetor_aux[tam];
	double aux1;
	double aux2;

	//Condição de parada do while: quando o número de iterações exceder o número máxido determinado ou
	//quando o erro ||x(k+1)-x(k)|| < Epsilon
	while((k < itmax) && (erro > e)){
		//vetor_aux recebe x(k), para que x(k+1) possa ser armazenado em vetor_x e possa ser possível compará-los
		for(int p = 0; p < tam; p++){
			vetor_aux[p] = vetor_x[p];
		}
		//Seta vetor_x utilizando o método de Gauss-Seidel
		for(i = 1; i <= tam; i++){
			vetor_x[i-1] = (vetor_b[i-1]/pegarElemento(matriz,i,i));
			for(int j = 1; j <= tam; j++){
				if(i!= j){
					vetor_x[i-1] -= (pegarElemento(matriz, i, j)*vetor_x[j-1]/pegarElemento(matriz,i,i));
				}
			}
		}
		//Inicializa variáveis auxiliares com zero para posterior comparação com números positivos
		aux2 = vetor_aux[0];
		aux1 = vetor_x[0];

		//Cálculo ||x(k=1)-x(k)||
		for(int r = 0; r < tam; r++){
			if(modulo(vetor_aux[r]) > aux2){
				aux2 = vetor_aux[r];
			}
			if(modulo(vetor_x[r]) > aux1){
				aux1 = vetor_x[r];
			}
		}
		erro = modulo(aux1 - aux2);
		k++;
	}
	//Imprime os resultados na tela
	printf(">> Met. interativo de sistemas lineares - Met. Gauss-Seidel<<\n");
	printf(">> Calculo numerico <<\n\n");

	printf(">> Erro: %.10lf\n", erro);
	printf(">> Numero de iteracoes: %d\n", k);
	printf(">> Vetor x: \n\n");
	for(int l = 0; l < tam; l++){
		printf("%.2lf ", vetor_x[l]);
	}
	printf("\n\n");
}