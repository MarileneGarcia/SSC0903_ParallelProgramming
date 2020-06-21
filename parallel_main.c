#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "pcv.h"

int main( argc, argv )
int argc;
char **argv;
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
int master_io( master_comm, comm )
MPI_Comm comm;
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
    printf("%d\n", matriz_adj[0][0]);
    MPI_Comm_rank( comm, &rank );
    printf("rank master: %d\n",rank);
    MPI_Comm_size( master_comm, &size );
    for (i = 1; i < n; i++) {
        MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        //Recebe vetor com custo e caminho de cada escravo
        MPI_Send(&(matriz_adj[0][0]), n*n, MPI_INT, i, 1, MPI_COMM_WORLD);
        //MPI_Recv( buf, 256, MPI_CHAR, i, 0, master_comm, &status );
        //fputs( buf, stdout );
    }
}

        /* This is the slave */
int slave_io( master_comm, comm )
MPI_Comm comm;
{
    char buf[256];
    int  rank, n,i;
    int** matriz;
    MPI_Status status;
    MPI_Comm_rank( comm, &rank );
    //sprintf( buf, "Hello from slave %d\n", rank );

    //Recebe o quantidade de cidades a serem visitadas
    MPI_Recv( &n, 1, MPI_INT, 0, 0, master_comm, &status );

    printf("rank %d     n = %d\n",rank,n);
    matriz = (int**)malloc(n*sizeof(int*));
    for(i = 0; i<n; i++)
        matriz[i] = (int*)malloc(n*sizeof(int));
    MPI_Recv( &matriz, n*n, MPI_INT, 0, 1, master_comm, &status );

    printf("%d\n", matriz[0][0]);
    //int nos_seg[n];
    /*inicializa vetor de nos seguintes*/
    //for(i=0;i<n;i++)
    //    nos_seg[i] = i;
    //0 1 2 3 
    //nos_seg[0] = n;
    //4 1 2 3
    //no_atual = quebra_vet(nos_seg, rank-1);
    //3 2 3
    //caminho = pcv(matriz, nos_seg, no_atual);
    //Enviar o vetor com custo e caminho para o mestre
    //MPI_Send( buf, strlen(buf) + 1, MPI_CHAR, 0, 0, master_comm );
    
    //sprintf( buf, "Goodbye from slave %d\n", rank );
    //MPI_Send( buf, strlen(buf) + 1, MPI_CHAR, 0, 0, master_comm );

    return 0;
}