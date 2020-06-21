#include <stdio.h>
#include <stdlib.h>

int main(){
    FILE *fp;
    int n;

    /*abre o arquivo*/
    fp = fopen("arquivo_entrada.txt","r");

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
    strcat(mpi, " exe");

    //printf("%s\n", mpi);
    system(mpi);

    return 0;
}