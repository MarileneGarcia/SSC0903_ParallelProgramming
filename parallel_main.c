#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "pcv.h"

int master_io(MPI_Comm, MPI_Comm);

int slave_io(MPI_Comm, MPI_Comm);

int main(int argc, char **argv)
{
    int rank, size;
    MPI_Comm new_comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //Cria um grupo para o master comunicar todos os escravos
    MPI_Comm_split(MPI_COMM_WORLD, 0, 0, &new_comm);

    if (rank == 0)
        master_io(MPI_COMM_WORLD, new_comm);
    else
        slave_io(MPI_COMM_WORLD, new_comm);

    MPI_Finalize();
    return 0;
}

/* This is the master */
int master_io(MPI_Comm master_comm, MPI_Comm comm)
{
    int i, j, n, size, rank;
    char buf[256];
    MPI_Status status;

    /*leitura da matriz de adjacencia*/
    FILE *fp;
    int *matriz_adj;
    int *caminho;

    /*abre o arquivo*/
    fp = fopen("2_arquivo_entrada.txt", "r");

    if (fp == NULL)
    {
        printf("Erro na abertura do arquivo\n");
        return 1;
    }
    /*le o tamanho da matriz*/
    fscanf(fp, "%d", &n);
    /*le a matriz*/
    matriz_adj = matriz(fp, n);

    fclose(fp);

    //print_vetor(matriz_adj, n);

    //printf("%d\n", matriz_adj[0][0]);
    MPI_Comm_rank(comm, &rank);
    //printf("\nrank master: %d\n",rank);
    MPI_Comm_size(master_comm, &size);
    for (i = 1; i < n; i++)
    {
        MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Send(&(matriz_adj[0]), n * n, MPI_INT, i, 1, MPI_COMM_WORLD);
    }

    /* retorno de menor caminho anterior*/
    int *retorno_aux = (int *)calloc(n + 2, sizeof(int));
    int *retorno;
    /*soma dos custos*/
    int menor_custo = 9999999;
    /*custo para ir do no atual para o prox_no*/
    int custo;

    int **matriz_rz = fenix_matriz(matriz_adj, n);

    /* percorrerah todos os nos possiveis */
    /* nos_seg = TAM|NOS_SEGS*/
    for (j = 1; j < n; j++)
    {
        // recebe o caminho de menor custo
        MPI_Recv(&retorno_aux[0], n + 2, MPI_INT, j, 5, master_comm, &status);

        /*printf("\ncaminho de retorno do rank %d:\n", j);
        for (i = 0; i < retorno_aux[0]; i++)
            printf(" %d ", retorno_aux[i]);
        printf("\n");*/

        custo = matriz_rz[rank][j];

        // verifica se caminho recebido possui o menor custo
        if (menor_custo > retorno_aux[1] + custo)
        {
            retorno = copiar(retorno_aux);
            menor_custo = retorno[1] + custo;
        }
    }

    /* aumenta o tamanho do vetor*/
    retorno = (int *)realloc(retorno, (retorno[0] + 1) * sizeof(int));
    /*adicionar o custo */
    retorno[1] = menor_custo;
    /*adiciona o no atual*/
    retorno[retorno[0]] = rank;
    /*atualiza o tamanho do vetor*/
    retorno[0]++;

    /* Print custo*/
    printf("%d\n", retorno[1]);

    /* caminho */
    for (i = retorno[0] - 1; i >= 2; i--)
        printf("%d ", retorno[i]);

    printf("\n");

    return 0;
}

/* This is the slave */
int slave_io(MPI_Comm master_comm, MPI_Comm comm)
{
    //char buf[256];
    int rank, n, i, j;
    MPI_Status status;
    MPI_Comm_rank(comm, &rank);
    //sprintf( buf, "Hello from slave %d\n", rank );

    //Recebe o quantidade de cidades a serem visitadas
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    //printf("rank %d     n = %d\n",rank,n);

    int *vetor = (int *)calloc(n * n, sizeof(int));
    MPI_Recv(&vetor[0], n * n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

    //print_vetor(vetor, n);

    int **matriz = fenix_matriz(vetor, n);

    //if(rank==1) print_matriz(matriz,n);

    /*inicializa vetor de nos seguintes*/
    int *nos_seg = (int *)calloc(n, sizeof(int));
    //printf("\na percorrer no rank %d:\n", rank);
    for (i = 0; i < n; i++)
        nos_seg[i] = i;

    nos_seg[0] = n;

    int no_atual = quebra_vet(nos_seg, rank - 1);

    //printf("\nno atual no rank %d: %d\n\n",rank, no_atual);

    /*for (i = 0; i < nos_seg[0]; i++)
        printf(" %d  ", nos_seg[i]);
    printf("\n");*/

    int *caminho = pcv(matriz, nos_seg, no_atual);

    /*printf("\ncaminho no rank %d:\n", rank);
    for (i = 0; i < caminho[0]; i++)
        printf(" %d  ", caminho[i]);
    printf("\n");*/

    MPI_Send(&caminho[0], caminho[0], MPI_INT, 0, 5, MPI_COMM_WORLD);

    return 0;
}