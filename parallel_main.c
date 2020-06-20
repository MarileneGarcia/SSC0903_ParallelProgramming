#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "pcv.h"

int master_io( MPI_Comm, MPI_Comm);

int slave_io( MPI_Comm, MPI_Comm);

int main(int  argc, char **argv )
{
    int rank, size;
    MPI_Comm new_comm;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    //Cria um grupo para o master comunicar todos os escravos
    MPI_Comm_split( MPI_COMM_WORLD, 0, 0, &new_comm );

    if (rank == 0) 
        master_io( MPI_COMM_WORLD, new_comm );
    else
        slave_io( MPI_COMM_WORLD, new_comm );

    MPI_Finalize( );
    return 0;
}

/* This is the master */
int master_io( MPI_Comm master_comm, MPI_Comm comm )
{
    int        i,n,size,rank;
    char       buf[256];
    MPI_Status status;

    /*leitura da matriz de adjacencia*/
    FILE *fp;
    int **matriz_adj;
    int *caminho;

    /*abre o arquivo*/
    fp = fopen("arquivo_entrada.txt","r");

    if (fp == NULL){
        printf("Erro na abertura do arquivo\n");
        return 1;
    }
    /*le o tamanho da matriz*/
    fscanf(fp, "%d", &n);
    /*le a matriz*/
    matriz_adj = matriz(fp,n);
    /*fecha o arquivo*/
    /*fclose(fp);*/
    //printf("%d\n", matriz_adj[0][0]);
    MPI_Comm_rank( comm, &rank );
    //printf("\nrank master: %d\n",rank);
    MPI_Comm_size( master_comm, &size );
    for (i = 1; i < n; i++) {
        MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        //Recebe vetor com custo e caminho de cada escravo
        MPI_Send(&(matriz_adj[0][0]), n*n, MPI_INT, i, 1, MPI_COMM_WORLD);
        //MPI_Recv( buf, 256, MPI_CHAR, i, 0, master_comm, &status );
        //fputs( buf, stdout );
    }

    return 0;
}

        /* This is the slave */
int slave_io( MPI_Comm master_comm, MPI_Comm comm )
{
    //char buf[256];
    int  rank, n,i;
    int** matriz;
    MPI_Status status;
    MPI_Comm_rank( comm, &rank );
    //sprintf( buf, "Hello from slave %d\n", rank );

    //Recebe o quantidade de cidades a serem visitadas
    MPI_Recv( &n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status );
    //printf("rank %d     n = %d\n",rank,n);

    matriz = (int**)malloc(n*sizeof(int*));
    for(i = 0; i<n; i++)
        matriz[i] = (int*)malloc(n*sizeof(int));
    MPI_Recv( &matriz[0][0], n*n, MPI_INT, 0, 1, MPI_COMM_WORLD, &status );
    //printf("%d\n", matriz[0][0]);

    /*inicializa vetor de nos seguintes*/
    int *nos_seg = (int*)malloc(n*sizeof(int));
    printf("\na percorrer no rank %d:\n",rank);
    for(i=0;i<n;i++)
        nos_seg[i] = i;

    nos_seg[0] = n;

    int no_atual = quebra_vet(nos_seg, rank-1);

    printf("\nno atual no rank %d: %d\n\n",rank, no_atual);

    for(i=0;i<nos_seg[0];i++) 
        printf(" %d  ",nos_seg[i]);
    printf("\n");

    //3 2 3
    int *caminho = pcv(matriz, nos_seg, no_atual);

    printf("\ncaminho no rank %d:\n",rank);
    for(i=0;i<caminho[0];i++)
        printf(" %d  ",caminho[i]);
    printf("\n");

    return 0;
}