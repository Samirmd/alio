#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef DO_MPI
#include "mpi.h"
#endif

int main(int argc, char **argv)
{

    int my_rank;
#ifdef DO_MPI
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
#else
    my_rank = 0;
#endif
    struct stat buf;
    FILE *ff = fopen("a","r");
    int filedes = fileno(ff);
    printf("filedes %d\n", filedes);
    int status  = fstat(filedes, &buf);
    printf("Size is %ld\n", buf.st_size);
    close(filedes);
    exit(0);
    if(my_rank==0)
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
//    else
    {
        const char *message="Hello from others\n";
        FILE *f = fopen64("b", "w");
        fwrite(message, 1, strlen(message), f);
        fclose(f);
    }

#ifdef DO_MPI
    MPI_Finalize();
#endif

    //    exit(0);
}   // main
