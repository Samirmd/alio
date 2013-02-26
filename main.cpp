#include <string.h>
#include <stdio.h>

#include "mpi.h"

int main(int argc, char **argv)
{

    MPI_Init(&argc, &argv);
    int my_rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    //if(my_rank==0)
    if(0==0)
    {
        const char *message="Hello from 0\n";
        
        FILE *f = fopen64("a", "w");
        fseeko64(f, 0, SEEK_SET);
        ferror(f);
        ftello64(f);
        char buffer[1024];
        setvbuf(f, buffer, _IONBF, 1024);
        fwrite(message, 1, strlen(message), f);
        fflush(f);
        fclose(f);
    }
    else
    {
        const char *message="Hello from others\n";
        FILE *f = fopen64("b", "w");
        fwrite(message, 1, strlen(message), f);
        fclose(f);
    }
    MPI_Finalize();

    exit(0);
}   // main
