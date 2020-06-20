#include <stdio.h>
#include <stdlib.h>
#include "pcv.h"

int **matriz(FILE *fp, int n)
{
    int i, j;
    int **m = (int **)malloc(n * sizeof(int *));

    for (i = 0; i < n; i++)
        m[i] = (int *)malloc(n * sizeof(int));

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (fscanf(fp, "%d", &m[i][j]) != 1)
            {
                fprintf(stderr, "invalid input for a[%d][%d]\n", i, j);
                fclose(fp);
                exit(1);
            }
            /*printf(" %d ", m[i][j]);*/
        }
        /* printf("\n");*/
    }

    return m;
}

int quebra_vet(int *vet, int pos)
{

    /*caso a posicao n exista no vetor */
    if (pos >= vet[0])
    {
        printf("\nErro:Posicao inexistente\n");
        return -1;
    }
    /*caso o vetor esteja vazio*/
    else if (vet[0] == 1)
    {
        printf("\nErro: Vetor vazio\n");
    }

    int i, elem = vet[pos + 1];

    /*shiftar todos elemento do vetor até aquela posicao*/
    for (i = pos + 1; i < (vet[0] - 1); i++)
        vet[i] = vet[i + 1];

    /*atualizar o tam*/
    vet[0]--;

    return elem;
}

int *copiar(int *vet)
{
    int i;
    int *vet_copy = (int *)malloc(vet[0] * sizeof(int));

    for (i = 0; i < vet[0]; i++)
        vet_copy[i] = vet[i];

    return vet_copy;
}

int *pcv(int **matriz_adj, int *nos_seg, int no_atual)
{

    int *retorno;
    /* chegou a o ultimo nó */
    if (nos_seg[0] == 1)
    {

        /* cria um vetor que sera pcv*/
        retorno = (int *)malloc(4 * sizeof(int));

        /* atualiza o tamanho do vetor */
        retorno[0] = 4;

        /* atualizar o custo */
        retorno[1] = matriz_adj[no_atual][nos_seg[1]];
        retorno[1] += matriz_adj[nos_seg[1]][0];

        /* adiciona o no final */
        retorno[2] = 0;
        /* adiciona o no atual */
        retorno[3] = no_atual;
    }
    else
    {

        int j;

        /* guarda o noh futuro que sera visitado */
        int prox_no_atual;
        /* mantem uma copia*/
        /* 4 1 2 3*/
        int *no_seg_copy = copiar(nos_seg);
        /* retorno de menor caminho anterior*/
        int *retorno_aux;
        /*soma dos custos*/
        int menor_custo = 9999999;
        /*custo para ir do no atual para o prox_no*/
        int custo;

        /* percorrerah todos os nos possiveis */
        /* nos_seg = TAM|NOS_SEGS*/
        for (j = 1; j < nos_seg[0]; j++)
        {

            /* retorna o elemento que sera o proximo noh e o retira do vetor */
            /*3 1 3*/
            prox_no_atual = quebra_vet(nos_seg, j - 1);

            if (prox_no_atual == -1)
                printf("Erro: quebra de vet");

            /* recebe o caminho de menor custo*/
            retorno_aux = pcv(matriz_adj, nos_seg, prox_no_atual);

            custo = matriz_adj[no_atual][prox_no_atual];

            /* verifica se caminho recebido possui o menor custo*/
            if (menor_custo > retorno_aux[1] + custo)
            {
                retorno = retorno_aux;
                menor_custo = retorno[1] + custo;
            }
            /*3 2 3*/
            /* faz uma copia do nos_seg original*/
            nos_seg = copiar(no_seg_copy);
            /*4 1 2 3 */
        }

        /* aumenta o tamanho do vetor*/
        retorno = (int *)realloc(retorno, (retorno[0] + 1) * sizeof(int));
        /*adicionar o custo */
        retorno[1] = menor_custo;
        /*adiciona o no atual*/
        retorno[retorno[0]] = no_atual;
        /*atualiza o tamanho do vetor*/
        retorno[0]++;
    }
    return retorno;
}