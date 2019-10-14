#ifndef MATRIZ_ESPARSA_H

    #define MATRIZ_ESPARSA_H

    typedef struct stElemento elemento;
    typedef struct stMatrizEsparsa matrizEsparsa;

    matrizEsparsa *gerarMatriz (int , int );
    int addElemento(matrizEsparsa *, int , int , double );
    double pegarElemento(matrizEsparsa *, int , int );
    void imprimirMatriz(matrizEsparsa  *);

#endif
