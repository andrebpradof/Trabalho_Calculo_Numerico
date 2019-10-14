#include <stdio.h>
#include <stdlib.h>
#include "matrizEsparsa.h" 

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


struct stMatrizEsparsa {
    // ponteiros para os vetores de ponteiros de elementos
    elemento **linha;
    elemento **coluna;
    
    // número de linhas e colunas
    int numLinha;
    int numColuna;
};

double pegarElemento(matrizEsparsa *matriz, int linha, int coluna){
    linha-= 1;
    coluna-= 1;

    if ( (linha < matriz->numLinha) && (linha >= 0) && (coluna < matriz->numColuna) && (coluna >=0) ){ // Verifica a validade da posição enviada
        elemento *auxElemento = matriz->linha[linha]; // ponteiro para que o nó da cabeça não se perca

        while (auxElemento->elemento_direita != NULL && auxElemento->elemento_direita->coluna <= coluna) //Busca onde deve inserir o novo nó
            auxElemento = auxElemento->elemento_direita; //Percorrendo a lista

        if (auxElemento->coluna == coluna) // Se encontrar
            return auxElemento->valor; // Retorna o valor encontrado        
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
