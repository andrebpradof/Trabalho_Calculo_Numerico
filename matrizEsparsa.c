#include <stdio.h>
#include <stdlib.h>

typedef struct stCelula celula;
typedef struct stMatrizEsparsa matrizEsparsa;
struct stCelula{

    // valor armazenado na célula
    float valor;

    // "coordenadas" da célula (linha e coluna da matriz)
    int linha;
    int coluna;

    // ponteiros para as céulas a direita e a abaixo
    celula *celula_abaixo;
    celula *celula_direita; 
};


struct stMatrizEsparsa {
    // ponteiros para os vetores de ponteiros de celulas
    celula **linha;
    celula **coluna;
    
    // número de linhas e colunas
    int numLinha;
    int numColuna;
};

double pegarMatriz(matrizEsparsa *matriz, int linha, int coluna){
    linha-= 1;
    coluna-= 1;

    if ( (linha < matriz->numLinha) && (linha >= 0) && (coluna < matriz->numColuna) && (coluna >=0) ){ // Verifica a validade da posição enviada
        celula *auxCelula = matriz->linha[linha]; // ponteiro para que o nó da cabeça não se perca

        while (auxCelula->celula_direita != NULL && auxCelula->celula_direita->coluna <= coluna) //Busca onde deve inserir o novo nó
            auxCelula = auxCelula->celula_direita; //Percorrendo a lista

        if (auxCelula->coluna == coluna) // Se encontrar
            return auxCelula->valor; // Retorna o valor encontrado        
    }
    return 0; // Se não encontrar, retorna zero
}



int addMatriz (matrizEsparsa *matriz, int linha, int coluna, double valor){
    // Matrizes em C consideram o 0 como a primeira linha e coluna, mas para melhor interpretação do usuário, a linha e a coluna começarão em 1
    linha -= 1;
    coluna -= 1;

    if ( (linha < matriz->numLinha) && (linha >= 0) && (coluna < matriz->numColuna) && (coluna >=0) ){ // Verifica se a posição dada não é inválida
    
        // Celula a ser inserida
        celula *celula_set = (celula *) malloc (sizeof(celula));
        celula_set->linha=linha;
        celula_set->coluna=coluna;
        celula_set->valor=valor;

        celula *aux = matriz->linha[linha]; // ponteiro auxiliar para manipulação da matriz
        while ( (aux->celula_direita != NULL) && (aux->celula_direita->coluna < coluna) ){
            aux = aux->celula_direita; // se não estiver vazia, procura a posição onde a célula deve ser inserida                
        }if (aux->celula_direita == NULL){
            celula_set->celula_direita = NULL; 
            aux->celula_direita = celula_set;
        }else{
            celula_set->celula_direita = aux->celula_direita; 
            aux->celula_direita = celula_set;
        }
            

        aux = matriz->coluna[coluna];
        while ( (aux->celula_abaixo != NULL) && (aux->celula_abaixo->linha < linha) ){
            aux = aux->celula_abaixo; // se não estiver vazia, procura a posição onde a célula deve ser inserida                
        }if (aux->celula_abaixo == NULL){
            celula_set->celula_abaixo = NULL; 
            aux->celula_abaixo = celula_set;
        }else{
            celula_set->celula_abaixo = aux->celula_abaixo; 
            aux->celula_abaixo = celula_set;
        }

        return 1;
    }
    return 0;
}

matrizEsparsa *gerarMatriz (int numLinhas, int numColunas){
    matrizEsparsa *matriz = (matrizEsparsa *) malloc(sizeof(matrizEsparsa));

    matriz->numLinha = numLinhas;
    matriz->numColuna = numColunas;
    matriz->linha = (celula **) malloc ( (sizeof (celula *)) * numLinhas); // array de ponteiros para célula para as linhas
    matriz->coluna = (celula **) malloc ( (sizeof (celula *)) * numColunas); // array de ponteiros para célula para as colunas

    for (int cont=  0; cont < numLinhas; cont++){
        matriz->linha[cont] = (celula *) malloc (sizeof(celula));
        matriz->linha[cont]->celula_direita = NULL;
    }
    for (int cont = 0; cont < numColunas; cont++){
        matriz->coluna[cont] = (celula *) malloc (sizeof(celula));
        matriz->coluna[cont]->celula_abaixo = NULL;
    }

    return matriz;
}


void print_matriz(matrizEsparsa *matriz){
    for (int j = 1; j<=matriz->numLinha; j++){
        for (int i = 1; i<=matriz->numColuna; i++){
            printf("%.2lf  ", get_matriz(matriz,j,i));
        }
        printf("\n");
    }
}
