// to compile: mpicc 01-hello_mpi_omp_stanford.c -o 01-hello_mpi_omp_stanford -fopenmp
// para rodar: mpirun -np 2 01-hello_mpi_omp_stanford

#include <stdio.h>
#include "mpi.h"
#include <omp.h>

int main(int argc, char *argv[]) 
{
  int numprocs, rank, namelen;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int iam = 0, np = 1, provided;

  MPI_Init(&argc, &argv);
// MPI_Init_thread(&argc, &argv, MPI_THREAD_SINGLE, &provided);

  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Get_processor_name(processor_name, &namelen);

  #pragma omp parallel default(shared) private(iam, np) num_threads(4)
  {
    np = omp_get_num_threads();
    iam = omp_get_thread_num();
    printf("Hello from thread %d/%d, from process %d/%d on %s\n",
           iam, np, rank, numprocs, processor_name);
  } // end of parallel region
  MPI_Finalize();
}
