#include <stdio.h>
#include <stdlib.h>
#include "matriz_esparsa.h"

//Função que calcula o módulo de um número (double)
double modulo(double numero){
	if(numero < 0){
		numero = numero * (- 1);
	}
	return numero;
}

int main(){
	//Variáveis que armazenam o tamanho do vetor e o número máximo de iterações
	int tamanho = 200;
	int itmax = 2*tamanho;

	//Erro Epsilon = 10^(-10)
	double e = 0.0000000001;

	MATRIZ_ESPARSA *matriz;
	//Cria a matriz utilizando a função feita na biblioteca matriz_esparsa.c
	matriz = criar_matriz(tamanho,tamanho);

	int i;
	//Seta a matriz como indicado no enunciado
	for(i=1; i<= tamanho; i++){
		set_matriz(matriz,i,i,4.5);
		set_matriz(matriz,i,i+1,-1);
		set_matriz(matriz,i+1,i,-1);
		set_matriz(matriz,i,i+3,-1);
		set_matriz(matriz,i+3,i,-1); 
	}

	float vetor_b[tamanho];
	//Seta vetor b como determinado no enunciado
	for(int j = 1; j <= tamanho; j++){
		vetor_b[j-1] = 0;
		for(int m = 1; m <= tamanho; m++){
			vetor_b[j-1] += get_matriz(matriz,j,m);
		}
	}

	//Cria o vetor x(0) e seta como nulo
	double vetor_x[tamanho];
	for(int l = 0; l < tamanho; l++){
		vetor_x[l] = 0.0;
	}
	//Inicializa variáveis  e vetor auxiliares
	int j;
	int k = 0;
	double erro = 10; //Erro começa com 10 (núemro qualquer maior que zero)para a primeira iteração do while
	double vetor_aux[tamanho];
	double auxiliar1;
	double auxiliar2;

	//Condição de parada do while: quando o número de iterações exceder o número máxido determinado ou
	//quando o erro ||x(k+1)-x(k)|| < Epsilon
	while((k < itmax) && (erro > e)){
		//vetor_aux recebe x(k), para que x(k+1) possa ser armazenado em vetor_x e possa ser possível compará-los
		for(int p = 0; p < tamanho; p++){
			vetor_aux[p] = vetor_x[p];
		}
		//Seta vetor_x utilizando o método de Gauss-Seidel
		for(i = 1; i <= tamanho; i++){
			vetor_x[i-1] = (vetor_b[i-1]/get_matriz(matriz,i,i));
			for(int j = 1; j <= tamanho; j++){
				if(i!= j){
					vetor_x[i-1] -= (get_matriz(matriz, i, j)*vetor_x[j-1]/get_matriz(matriz,i,i));
				}
			}
		}
		//Inicializa variáveis auxiliares com zero para posterior comparação com números positivos
		auxiliar2 = vetor_aux[0];
		auxiliar1 = vetor_x[0];

		//Cálculo ||x(k=1)-x(k)||
		for(int r = 0; r < tamanho; r++){
			if(modulo(vetor_aux[r]) > auxiliar2){
				auxiliar2 = vetor_aux[r];
			}
			if(modulo(vetor_x[r]) > auxiliar1){
				auxiliar1 = vetor_x[r];
			}
		}
		erro = modulo(auxiliar1 - auxiliar2);
		k++;
	}
	//Imprime o erro final, o número de iterações e o vetor final x
	printf("Erro final: %.10lf\nNúmero de iterações: %d\n", erro, k);
	printf("Vetor final: \n\n");
	for(int l = 0; l < tamanho; l++){
		printf("%.2lf ", vetor_x[l]);
	}
	printf("\n");
}