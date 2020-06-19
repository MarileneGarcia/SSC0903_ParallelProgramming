// to compile: mpicc 01-mmatrix-slave.c -o 01-mmatrix-slave -Wall
// to run: initiated from 01-mmatrix-slave code by means of MPI_Comm_spawn

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"mpi.h"

int main(int argc, char **argv) {
    int         root, my_rank, num_proc, count, row_color, column_color, parent_rank;
    int 	*send_buffer=0;
    int         a_oper, b_oper, sendbuf;
    
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;

//    char 	filename[20]; // used just when debugging
//    FILE 	*trace;  // used just when debugging

    
    MPI_Comm    intercomm;
    MPI_Status	status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Get_processor_name(processor_name, &name_len);

    printf("I'm the process slave %d on processor %s.\n", my_rank, processor_name);
    fflush(0);

    // recouvering intercommunicator to interact to "father"! 
    MPI_Comm_get_parent(&intercomm);

    // these receives are not necessary. They are here for debugging purposes and to exemplify point-ot-point messagens.
    // it always receives these messages from "father" (remote process (rank 0) in the intercomm).
    count = 1;
    MPI_Recv(&parent_rank, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, intercomm, &status);
    MPI_Recv(&row_color, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, intercomm, &status);
    MPI_Recv(&column_color, count, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, intercomm, &status);
	
//  These files are used for tracing purposes. By default, just process 0 has right to print on stdout.
//  Observe the file names. There will be a different file for each process, according to their names.
//  sprintf(filename,"rank_%d_r%d_c%d_myrank%d.trace", parent_rank, row_color, column_color, my_rank);
//  trace = fopen(filename, "w");
	

    // receiving a and b operands for this process
    // they are responbible for multiplication and after reduction
    root = 0;
    MPI_Scatter(send_buffer, 1, MPI_INT, &a_oper, 1, MPI_INT, root, intercomm);
    MPI_Scatter(send_buffer, 1, MPI_INT, &b_oper, 1, MPI_INT, root, intercomm);

//    printing just for debugging purposes
//    fprintf(trace, "Rank %d => a_oper=%d   b_oper=%d sendbuf=%d \n", my_rank, a_oper, b_oper, sendbuf);
//    fflush(0);

    //making the multiplication
    sendbuf = a_oper * b_oper;

//  printing just for debugging purposes
//  fprintf(trace, "Rank %d => a_oper=%d   b_oper=%d sendbuf=%d \n", my_rank, a_oper, b_oper, sendbuf);
//  fflush(0);

    // contributing to the sum in a reduce operation. This is an intecommunicator.
    root = 0;
    count = 1;
    MPI_Reduce(&sendbuf, (int *) 0, count, MPI_INT, MPI_SUM, root, intercomm);

    //It's time to say bye-bye baby! Thanks Gosh!
    MPI_Finalize();
    exit(0);
}
