/*    PROBLEMA DO CAIXEIRO VIAJANTE    */
// to compile: mpicc parallel_main.c -o exe -lm -Wall
// to run: mpirun exe

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "pcv.h"

#define TAG_RESULT 0
#define TAG_ASK_FOR_JOB 1
#define TAG_JOB_DATA 2
#define TAG_STOP 3

void master();

void slave();

int main(int argc, char **argv)
{
    int rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
        master();
    else
        slave();

    MPI_Finalize();
}

void master()
{
    MPI_Status stat, stat2;

    while (/* there are jobs unprocessed */ || /* there are slaves still working on jobs */)
    {

        // Wait for any incomming message
        MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);

        // Store rank of receiver into slave_rank
        int slave_rank = stat.MPI_SOURCE;

        // Decide according to the tag which type of message we have got
        if (stat.MPI_TAG == TAG_ASK_FOR_JOB)
        {

            MPI_Recv(buff, slave_rank, TAG_ASK_FOR_JOB, MPI_COMM_WORLD, &stat2);

            if (/* there are unprocessed jobs */)
            {

                // here we have unprocessed jobs , we send one job to the slave
                job = /* compute job */
                    /* pack data of job into the buffer msg_buffer */
                    MPI_Send(msg_buffer, /*...*/, slave_rank, TAG_JOB_DATA, MPI_COMM_WORLD);
                /* mark slave with rank my_rank as working on a job */
            }
            else
            {

                // send stop msg to slave
                MPI_Send(/*...*/, slave_rank, TAG_STOP, MPI_COMM_WORLD);
            }
        }
        else
        {

            // We got a result message
            MPI_Recv(result_data_buffer, /*...*/, slave_rank, TAG_RESULT, MPI_COMM_WORLD, &stat2);
            /* put data from result_data_buffer into a global result */
            /* mark slave with rank slave_rank as stopped */
        }
    }
}

void slave()
{
    int stopped = 0;
    MPI_Status stat, stat2;
    do
    {

        // Here we send a message to the master asking for a job
        MPI_Send(/*...*/, 0, TAG_ASK_FOR_JOB, MPI_COMM_WORLD);
        MPI_Probe(0, /*...*/, MPI_COMM_WORLD, &stat);
        if (stat.MPI_TAG == TAG_JOB_DATA)
        {

            // Retrieve job data from master into msg_buffer
            MPI_Recv(msg_buffer, 0, TAG_JOB_DATA, MPI_COMM_WORLD, &stat2);
            /* work on job_data to get a result and store the result into
            result_buffer */

            // send result to master
            MPI_Send(result_buffer, /*...*/, 0, TAG_RESULT, MPI_COMM_WORLD);
        }
        else
        {
            // We got a stop message we have to retrieve it by using MPI_Recv
            // But we can ignore the data from the MPI_Recv call
            MPI_Recv(/*...*/, 0, TAG_STOP, MPI_COMM_WORLD, &stat2);
            stopped = 1;
        }
    } while (stopped == 0);
}

/*int main(int argc, char **argv)
{
    MPI_Init(NULL, NULL);

    int i;

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    if (world_rank == 0)
    {
        //leitura da matriz de adjacencia
        FILE *fp;
        int n;
        int **matriz_adj;
        int *caminho;

        printf("Processo mestre no processador %s (rank %d) \n", 
            processor_name, world_rank);

        //abre o arquivo
        fp = fopen("arquivo_entrada.txt", "r");

        if (fp == NULL)
        {
            printf("Erro na abertura do arquivo\n");
            return 1;
        }
        //le o tamanho da matriz
        fscanf(fp, "%d", &n);
        //le a matriz
        matriz_adj = matriz(fp, n);
        //fecha o arquivo
        //fclose(fp);

        int no_inicial = 0;
        int *nos_seg = (int *)malloc(n * sizeof(int));

        //inicializa vetor de nos seguintes
        for (i = 0; i < n; i++)
            nos_seg[i] = i;

        nos_seg[0] = n;

        caminho = pcv(matriz_adj, nos_seg, no_inicial);

        // for (i=0; i < n; i++)
        //   free(matriz_adj[i]);

        //free(matriz_adj);

        // Print custo
        printf("%d\n", caminho[1]);

        // caminho
        for (i = caminho[0] - 1; i >= 2; i--)
            printf("%d ", caminho[i]);

        printf("\n");

        free(nos_seg);
        free(caminho);
    }
    else
    {
        
    }

    MPI_Finalize();

    exit(0);
}*/