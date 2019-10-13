#ifndef MATRIZ_ESPARSA_H

    #define MATRIZ_ESPARSA_H

    typedef struct stCelula celula;

    typedef struct stMatrizEsparsa matrizEsparsa;

    matrizEsparsa *gerarMatriz (int , int );

    int addMatriz(matrizEsparsa *, int , int , double );

    double pegarMmatriz(matrizEsparsa *, int , int );

    void imprimirMatriz(matrizEsparsa  *);

#endif
