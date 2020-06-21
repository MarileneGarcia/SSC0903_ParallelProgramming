#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
    FILE *fp;
    int n;

    /*abre o arquivo*/
    fp = fopen(argv[1],"r");

    if (fp == NULL){
        printf("Erro na abertura do arquivo\n");
        return 1;
    }

    /*le o tamanho da matriz*/
    fscanf(fp, "%d", &n);

    fclose(fp);


    char string[33];
    sprintf(string, "%d",n);

    char mpi[] = "mpirun -np ";
    strcat(mpi, string);
    strcat(mpi, " exe ");

    strcat(mpi, argv[1]);

    //printf("%s\n", mpi);
    system(mpi);

    return 0;
}