#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define e 0.00000001 // epsilon = 10^(-8)

int n; // definido o tamanho 
int itmax; // definido o número máximo de interações

typedef struct stElemento elemento;
struct stElemento{

    // valor armazenado pela matriz
    float valor;

    // linhas e colunas da matriz
    int linha;
    int coluna;

    // ponteiros para as céulas a direita e a abaixo
    elemento *elemento_abaixo;
    elemento *elemento_direita; 
};

//struct da matriz esparsa
typedef struct{
    // ponteiros para os vetores ponteiros de elementos
    elemento **linha;
    elemento **coluna;
    
    // número de linhas e colunas
    int numLinha;
    int numColuna;
}matrizEsparsa;

// pega o elemento 
double pegarElemento(matrizEsparsa *matriz, int linha, int coluna){
    //Começa de 0 e não de 1, melhor para o usuário
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
    //Começa de 0 e não de 1, melhor para o usuário
    linha -= 1;
    coluna -= 1;

    if ( (linha < matriz->numLinha) && (linha >= 0) && (coluna < matriz->numColuna) && (coluna >=0) ){ // Verifica se a posição dada não é inválida
    
        // Elemento a ser inserido
        elemento *elemento_set = (elemento *) malloc (sizeof(elemento));
		elemento_set->valor=valor;
		elemento_set->coluna=coluna;
        elemento_set->linha=linha;

        elemento *aux = matriz->linha[linha]; // auxliar para não perder o ponteiro inicial de matriz
        while ( (aux->elemento_direita != NULL) && (aux->elemento_direita->coluna < coluna) ){
            aux = aux->elemento_direita;                 
        }
		if (aux->elemento_direita != NULL){ //Ajusta os ponteiros
            elemento_set->elemento_direita = aux->elemento_direita; 
            aux->elemento_direita = elemento_set;
        }else{
            elemento_set->elemento_direita = NULL; 
            aux->elemento_direita = elemento_set;
        }
            

        aux = matriz->coluna[coluna];
        while ( (aux->elemento_abaixo != NULL) && (aux->elemento_abaixo->linha < linha) ){
            aux = aux->elemento_abaixo; // percorre a lista para emcontrar a posição onde deve ser inserido o novo elemento                
        }
		if (aux->elemento_abaixo != NULL){ //Ajusta os ponteiros
            elemento_set->elemento_abaixo = aux->elemento_abaixo; 
            aux->elemento_abaixo = elemento_set;
        }else{ //Se for null, insere no final
            elemento_set->elemento_abaixo = NULL; 
            aux->elemento_abaixo = elemento_set;
        }
		
        return 1;
    }
    return 0;
}

matrizEsparsa *gerarMatriz (int numLinhas, int numColunas){ //Cria a matriz esparsa
    matrizEsparsa *matriz = (matrizEsparsa *) malloc(sizeof(matrizEsparsa));

	//setando
	matriz->numColuna = numColunas; 
    matriz->numLinha = numLinhas;

	matriz->coluna = (elemento **) malloc ( (sizeof (elemento *)) * numColunas); // array de ponteiros para célula para as colunas
    matriz->linha = (elemento **) malloc ( (sizeof (elemento *)) * numLinhas); // array de ponteiros para célula para as linhas

	int cont = 0;
	while(cont < numColunas){ //Aloca as colunas
        matriz->coluna[cont] = (elemento *) malloc (sizeof(elemento));
        matriz->coluna[cont]->elemento_abaixo = NULL;
		cont++;
    }

	cont = 0;
    while(cont < numLinhas){ //Aloca as linhas
        matriz->linha[cont] = (elemento *) malloc (sizeof(elemento));
        matriz->linha[cont]->elemento_direita = NULL;
		cont++;
    }

    return matriz;
}

void setaParametro(int i, int mux){ // Seta n e itmax
	n = i;
	itmax = mux*n;
}
//Função que calcula o módulo de um número (double)
double modulo(double numero){
	if(numero < 0){
		numero = numero * (- 1);
	}
	return numero;
}

// Insere os valores na matriz, como indica o enunciado
void setarMatriz(matrizEsparsa *matriz){
	for(int i=1; i<= n; i++){
		addElemento(matriz,i,i,5);
		addElemento(matriz,i,i+1,-1);
		addElemento(matriz,i+1,i,-1);
		addElemento(matriz,i,i+3,-1);
		addElemento(matriz,i+3,i,-1); 
	}
}
//Seta o vetor b como indica o enunciado (letra a)
void setaVetorB(matrizEsparsa *matriz,float *vetorB){ 
	for(int j = 1; j <= n; j++){
		vetorB[j-1] = 0;
		for(int m = 1; m <= n; m++){
			vetorB[j-1] += pegarElemento(matriz,j,m);
		}
	}
}

//Seta o vetor b como indica o enunciado (Letra c)
void setaVetorBAltC(float *vetorB){
	for(int i = 1; i <= n; i++){
		vetorB[i-1] = (double)(1.0/i);
	}
}

//Seta o vetor x como sugere o enunciado
void setaVetorX(matrizEsparsa *matriz, double *vetorX){
	for(int l = 0; l < n; l++){
		vetorX[l] = 0.0;
	}
}

// Imprime o resutado
void imprimeResultado(float erro, int k, double *vet){
	printf(">> Erro: %.10lf\n", erro);
	printf(">> Numero de iteracoes: %d\n", k);
	printf(">> Vetor x: \n\n");
	printf("[ ");
	for(int l = 0; l < n; l++){
		printf("%.4lf , ", vet[l]);
	}
	printf(" ]");
	printf("\n\n");
}

// Calcula o módulo infinito
void moduloInfinito(double *vetorAux,double *vetorX, double *aux1, double *aux2){

	*aux2 = vetorAux[0];
	*aux1 = vetorX[0];

	for(int i = 0; i < n; i++){
		if(modulo(vetorAux[i]) > *aux2){
			*aux2 = vetorAux[i];
		}
		if(modulo(vetorX[i]) > *aux1){
			*aux1 = vetorX[i];
		}
	}
}

// Realiza os cálculos do método Gauss-Seidel
int metodoGaussSeidel(matrizEsparsa *matriz, float *vetorB, double *vetorX){
	//Inicializa variáveis e vetor auxiliares
	int j;
	int k = 0;
	double erro = 5; //Erro começa com 5 para a primeira iteração do while
	double vetorAux[n];
	double aux1;
	double aux2;


	//A condição de parada do while é quando o número de iterações ultrapassar o número máximo indicado
	// ou quando o erro ||x(k+1)-x(k)|| < Epsilon
	while((k < itmax) && (erro > e)){
		//vetorAux é utilizado nas comparações abaixo
		for(int p = 0; p < n; p++){
			vetorAux[p] = vetorX[p];
		}
		//Seta vetorX utilizando o método de Gauss-Seidel
		for(int i = 1; i <= n; i++){
			vetorX[i-1] = (vetorB[i-1]/pegarElemento(matriz,i,i));
			for(int j = 1; j <= n; j++){
				if(i!= j){
					vetorX[i-1] -= (pegarElemento(matriz, i, j)*vetorX[j-1]/pegarElemento(matriz,i,i));
				}
			}
		}

		//Calcula o módulo infinito
		moduloInfinito(vetorAux, vetorX, &aux1, &aux2);
		erro = modulo(aux1 - aux2);
		
		//Caso o método dirvija
		if ( (k+1) > itmax ){
			printf(">> METODO GAUSS-SEIDEL DIVERGIU <<\n");
			return 0;
		}

		k++;
	}

	//Imprime os resultados obtidos
	imprimeResultado(erro, k, vetorX);
}

// Função que executa os testes referentes aos parâmetros informados
void testeExercicio(int n, int i, int status){
	setaParametro(n,i); //Seta n e o multiplicador
	matrizEsparsa *matriz;
	
	//Cria a matriz esparsa
	matriz = gerarMatriz(n,n);

	//Declara os vetores B e X
	float vetorB[n];
	double vetorX[n];

	//Seta a matriz como sujerido no enunciado
	setarMatriz(matriz);

	//Seta vetor b como determinado no enunciado
	if(status == 1){
		setaVetorB(matriz,vetorB);
	}
	else{
		setaVetorBAltC(vetorB);
	}
	
	//Seta o vetor X
	setaVetorX(matriz,vetorX);
	
	//Realiza os cálculos referentes ao método de Gauss-Seidel
	metodoGaussSeidel(matriz,vetorB,vetorX);

	printf("\n----------------------------------------------------------------------------------------------------------------------------\n\n");
}
int main(){

	printf("\n\n");
	printf(">> Met. interativo de sistemas lineares - Met. Gauss-Seidel<<\n");
	printf(">> Calculo numerico <<\n\n\n");

	testeExercicio(50,5,1); // letra b para n = 50
	testeExercicio(100,5,1); // Letra b para n = 100
	testeExercicio(500,5,2); // Letra c para n = 500

	return 0;
}