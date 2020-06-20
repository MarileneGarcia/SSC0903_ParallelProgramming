#pragma once

#ifndef PCV_H

#define PCV_H

/* le a matriz no arquivo fp*/
int **matriz(FILE *, int);

/* retira um elemento do vetor e o retorna */
int quebra_vet(int *, int);

/* retorna uma copia de um vetor*/
int *copiar(int *);

/* retorna o menor caminho*/
int *pcv(int **, int *, int);

#endif