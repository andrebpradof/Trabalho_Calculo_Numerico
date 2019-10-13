#include <stdio.h>
#include <stdlib.h>
#include "matrizEsparsa.h"

struct celula{
    // "coordenadas" da célula (linha e coluna da matriz)
    int linha;
    int coluna;

    // valor armazenado na célula
    float valor;

    // ponteiros para as céulas a direita e a abaixo
    CELULA *cel_direita; 
    CELULA *cel_abaixo;
};

struct matriz_esparsa {
    // ponteiros para os vetores de ponteiros de celulas
    CELULA **linha;
    CELULA **coluna;
    
    // número de linhas e colunas
    int nL;
    int nC;
};

MATRIZ_ESPARSA *criar_matriz (int n_linhas, int n_colunas){
    MATRIZ_ESPARSA *matriz_criada = (MATRIZ_ESPARSA *) malloc(sizeof(MATRIZ_ESPARSA));

    matriz_criada->nL = n_linhas;
    matriz_criada->nC = n_colunas;
    matriz_criada->linha = (CELULA **) malloc ( (sizeof (CELULA *)) * n_linhas); // array de ponteiros para célula para as linhas
    matriz_criada->coluna = (CELULA **) malloc ( (sizeof (CELULA *)) * n_colunas); // array de ponteiros para célula para as colunas

    int aux;
    for (aux=0; aux < n_linhas; aux++){
        matriz_criada->linha[aux] = (CELULA *) malloc (sizeof(CELULA));
        matriz_criada->linha[aux]->cel_direita = NULL;
    }for (aux=0; aux < n_colunas; aux++){
        matriz_criada->coluna[aux] = (CELULA *) malloc (sizeof(CELULA));
        matriz_criada->coluna[aux]->cel_abaixo = NULL;
    }

    return matriz_criada;
}

int set_matriz (MATRIZ_ESPARSA *matriz, int linha_set, int coluna_set, double valor_set){
    // Matrizes em C consideram o 0 como a primeira linha e coluna, mas para melhor interpretação do usuário, a linha e a coluna começarão em 1
    --linha_set;
    --coluna_set;

    if ( (linha_set < matriz->nL) && (linha_set >= 0) && (coluna_set < matriz->nC) && (coluna_set >=0) ){ // Verifica se a posição dada não é inválida
    
        // Celula a ser inserida
        CELULA *celula_set = (CELULA *) malloc (sizeof(CELULA));
        celula_set->linha=linha_set;
        celula_set->coluna=coluna_set;
        celula_set->valor=valor_set;

        CELULA *p_aux = matriz->linha[linha_set]; // ponteiro auxiliar para manipulação da matriz
        while ( (p_aux->cel_direita != NULL) && (p_aux->cel_direita->coluna < coluna_set) ){
            p_aux = p_aux->cel_direita; // se não estiver vazia, procura a posição onde a célula deve ser inserida                
        }if (p_aux->cel_direita == NULL){
            celula_set->cel_direita = NULL; 
            p_aux->cel_direita = celula_set;
        }else{
            celula_set->cel_direita = p_aux->cel_direita; 
            p_aux->cel_direita = celula_set;
        }
            

        p_aux = matriz->coluna[coluna_set];
        while ( (p_aux->cel_abaixo != NULL) && (p_aux->cel_abaixo->linha < linha_set) ){
            p_aux = p_aux->cel_abaixo; // se não estiver vazia, procura a posição onde a célula deve ser inserida                
        }if (p_aux->cel_abaixo == NULL){
            celula_set->cel_abaixo = NULL; 
            p_aux->cel_abaixo = celula_set;
        }else{
            celula_set->cel_abaixo = p_aux->cel_abaixo; 
            p_aux->cel_abaixo = celula_set;
        }

        return 1;
    }
    return 0;
}

double get_matriz(MATRIZ_ESPARSA *matriz, int linha_get, int coluna_get){
    --linha_get;
    --coluna_get;

    if ( (linha_get < matriz->nL) && (linha_get >= 0) && (coluna_get < matriz->nC) && (coluna_get >=0) ){ // Verifica se a posição dada não é inválida
        CELULA *celula_get = matriz->linha[linha_get]; // ponteiro auxiliar que recebe o endereço do nó cabeça

        while ( celula_get->cel_direita != NULL && celula_get->cel_direita->coluna <= coluna_get)
            celula_get = celula_get->cel_direita;

        if (celula_get->coluna == coluna_get)
            return celula_get->valor;        
    }
    return 0;
}

void print_matriz(MATRIZ_ESPARSA  *matriz){
    int i, j;
    for (j=1; j<=matriz->nL; j++){
        for (i=1; i<=matriz->nC; i++){
            printf("%.2lf  ", get_matriz(matriz,j,i));
        }
        printf("\n");
    }
}
