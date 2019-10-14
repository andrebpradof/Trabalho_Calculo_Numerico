#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define n 50 //nanho da matriz
#define itmax n*5 // número máximo de interações
#define e 0.00000001 //Erro Epsilon = 10^(-8)

typedef struct stElemento elemento;
struct stElemento{

    // valor armazenado na célula
    float valor;

    // "coordenadas" da célula (linha e coluna da matriz)
    int linha;
    int coluna;

    // ponteiros para as céulas a direita e a abaixo
    elemento *elemento_abaixo;
    elemento *elemento_direita; 
};


typedef struct{
    // ponteiros para os vetores de ponteiros de elementos
    elemento **linha;
    elemento **coluna;
    
    // número de linhas e colunas
    int numLinha;
    int numColuna;
}matrizEsparsa;

double pegarElemento(matrizEsparsa *matriz, int linha, int coluna){
    linha-= 1;
    coluna-= 1;

    if ( (linha < matriz->numLinha) && (linha >= 0) && (coluna < matriz->numColuna) && (coluna >=0) ){ // Verifica a validade da posição enviada
        elemento *auxElemento = matriz->linha[linha]; // ponteiro para que o nó da cabeça não se perca

        while (auxElemento->elemento_direita != NULL && auxElemento->elemento_direita->coluna <= coluna){ //Busca onde deve inserir o novo nó
            auxElemento = auxElemento->elemento_direita; //Percorrendo a lista
		}

        if (auxElemento->coluna == coluna){ // Se encontrar
            return auxElemento->valor; // Retorna o valor encontrado        
		}
    }
    return 0; // Se não encontrar, retorna zero
}



int addElemento(matrizEsparsa *matriz, int linha, int coluna, double valor){
    // Matrizes em C consideram o 0 como a primeira linha e coluna, mas para melhor interpretação do usuário, a linha e a coluna começarão em 1
    linha -= 1;
    coluna -= 1;

    if ( (linha < matriz->numLinha) && (linha >= 0) && (coluna < matriz->numColuna) && (coluna >=0) ){ // Verifica se a posição dada não é inválida
    
        // Elemento a ser inserida
        elemento *elemento_set = (elemento *) malloc (sizeof(elemento));
        elemento_set->linha=linha;
        elemento_set->coluna=coluna;
        elemento_set->valor=valor;

        elemento *aux = matriz->linha[linha]; // ponteiro auxiliar para manipulação da matriz
        while ( (aux->elemento_direita != NULL) && (aux->elemento_direita->coluna < coluna) ){
            aux = aux->elemento_direita; // se não estiver vazia, procura a posição onde a célula deve ser inserida                
        }if (aux->elemento_direita == NULL){
            elemento_set->elemento_direita = NULL; 
            aux->elemento_direita = elemento_set;
        }else{
            elemento_set->elemento_direita = aux->elemento_direita; 
            aux->elemento_direita = elemento_set;
        }
            

        aux = matriz->coluna[coluna];
        while ( (aux->elemento_abaixo != NULL) && (aux->elemento_abaixo->linha < linha) ){
            aux = aux->elemento_abaixo; // se não estiver vazia, procura a posição onde a célula deve ser inserida                
        }if (aux->elemento_abaixo == NULL){
            elemento_set->elemento_abaixo = NULL; 
            aux->elemento_abaixo = elemento_set;
        }else{
            elemento_set->elemento_abaixo = aux->elemento_abaixo; 
            aux->elemento_abaixo = elemento_set;
        }

        return 1;
    }
    return 0;
}

matrizEsparsa *gerarMatriz (int numLinhas, int numColunas){
    matrizEsparsa *matriz = (matrizEsparsa *) malloc(sizeof(matrizEsparsa));

    matriz->numLinha = numLinhas;
    matriz->numColuna = numColunas;
    matriz->linha = (elemento **) malloc ( (sizeof (elemento *)) * numLinhas); // array de ponteiros para célula para as linhas
    matriz->coluna = (elemento **) malloc ( (sizeof (elemento *)) * numColunas); // array de ponteiros para célula para as colunas

    for (int cont=  0; cont < numLinhas; cont++){
        matriz->linha[cont] = (elemento *) malloc (sizeof(elemento));
        matriz->linha[cont]->elemento_direita = NULL;
    }
    for (int cont = 0; cont < numColunas; cont++){
        matriz->coluna[cont] = (elemento *) malloc (sizeof(elemento));
        matriz->coluna[cont]->elemento_abaixo = NULL;
    }

    return matriz;
}


void imprimirMatriz(matrizEsparsa *matriz){
    for (int j = 1; j<=matriz->numLinha; j++){
        for (int i = 1; i<=matriz->numColuna; i++){
            printf("%.2lf  ", pegarElemento(matriz,j,i));
        }
        printf("\n");
    }
}


//Função que calcula o módulo de um número (double)
double modulo(double numero){
	if(numero < 0){
		numero = numero * (- 1);
	}
	return numero;
}

int main(){

	printf(">> Met. interativo de sistemas lineares - Met. Gauss-Seidel<<\n");
	printf(">> Calculo numerico <<\n\n");

	matrizEsparsa *matriz;
	//Cria a matriz utilizando a função feita na biblioteca matriz_esparsa.c
	matriz = gerarMatriz(n,n);

	int i;
	//Seta a matriz como indicado no enunciado
	for(i=1; i<= n; i++){
		addElemento(matriz,i,i,5);
		addElemento(matriz,i,i+1,-1);
		addElemento(matriz,i+1,i,-1);
		addElemento(matriz,i,i+3,-1);
		addElemento(matriz,i+3,i,-1); 
	}

	float vetor_b[n];
	//Seta vetor b como determinado no enunciado
	for(int j = 1; j <= n; j++){
		vetor_b[j-1] = 0;
		for(int m = 1; m <= n; m++){
			vetor_b[j-1] += pegarElemento(matriz,j,m);
		}
	}

	//Cria o vetor x(0) e seta como nulo
	double vetor_x[n];
	for(int l = 0; l < n; l++){
		vetor_x[l] = 0.0;
	}
	//Inicializa variáveis e vetor auxiliares
	int j;
	int k = 0;
	double erro = 1; //Erro começa com 10 (núemro qualquer maior que zero)para a primeira iteração do while
	double vetor_aux[n];
	double aux1;
	double aux2;

	//Condição de parada do while: quando o número de iterações exceder o número máxido determinado ou
	//quando o erro ||x(k+1)-x(k)|| < Epsilon
	while((k < itmax) && (erro > e)){
		//vetor_aux recebe x(k), para que x(k+1) possa ser armazenado em vetor_x e possa ser possível compará-los
		for(int p = 0; p < n; p++){
			vetor_aux[p] = vetor_x[p];
		}
		//Seta vetor_x utilizando o método de Gauss-Seidel
		for(i = 1; i <= n; i++){
			vetor_x[i-1] = (vetor_b[i-1]/pegarElemento(matriz,i,i));
			for(int j = 1; j <= n; j++){
				if(i!= j){
					vetor_x[i-1] -= (pegarElemento(matriz, i, j)*vetor_x[j-1]/pegarElemento(matriz,i,i));
				}
			}
		}
		//Inicializa variáveis auxiliares com zero para posterior comparação com números positivos
		aux2 = vetor_aux[0];
		aux1 = vetor_x[0];

		//Cálculo ||x(k=1)-x(k)||
		for(int r = 0; r < n; r++){
			if(modulo(vetor_aux[r]) > aux2){
				aux2 = vetor_aux[r];
			}
			if(modulo(vetor_x[r]) > aux1){
				aux1 = vetor_x[r];
			}
		}
		erro = modulo(aux1 - aux2);
		k++;

		if ( (k+1) > itmax ){
			printf(">> METODO GAUSS-SEIDEL DIVERGIU <<\n");
			return 0;
		}
	}
	//Imprime os resultados na tela
	printf(">> Erro: %.10lf\n", erro);
	printf(">> Numero de iteracoes: %d\n", k);
	printf(">> Vetor x: \n\n");
	for(int l = 0; l < n; l++){
		printf("%.2lf ", vetor_x[l]);
	}
	printf("\n\n");

	return 0;
}