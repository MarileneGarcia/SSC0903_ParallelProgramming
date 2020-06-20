#ifndef PCV_H

#define PCV_H

/* le a matriz no arquivo fp*/
int *matriz(FILE *, int);

void print_vetor(int *, int);

int **fenix_matriz(int *, int);

void print_matriz(int **, int);

/* retira um elemento do vetor e o retorna */
int quebra_vet(int *, int);

/* retorna uma copia de um vetor*/
int *copiar(int *);

/* retorna o menor caminho*/
int *pcv(int **, int *, int);

#endif