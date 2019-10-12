#ifndef MATRIZ_ESPARSA_H

    #define MATRIZ_ESPARSA_H

    typedef struct celula CELULA;

    typedef struct matriz_esparsa MATRIZ_ESPARSA;

    MATRIZ_ESPARSA *criar_matriz (int , int );

    int set_matriz(MATRIZ_ESPARSA *, int , int , double );

    double get_matriz(MATRIZ_ESPARSA *, int , int );

    void print_matriz(MATRIZ_ESPARSA  *);

	MATRIZ_ESPARSA  *somar_matriz( MATRIZ_ESPARSA *m1,  MATRIZ_ESPARSA *m2);
	MATRIZ_ESPARSA  * transposta_matriz(MATRIZ_ESPARSA  *matriz);
	MATRIZ_ESPARSA  *multiplicar_matriz( MATRIZ_ESPARSA *m1, MATRIZ_ESPARSA *m2);
	MATRIZ_ESPARSA *ler_matriz(char name[20]);
	//void resume_matriz(MATRIZ_ESPARSA *matriz);

#endif
