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
#if 0
    struct stat buf;
    FILE *ff = fopen("axxaxx","r");
    int filedes = fileno(ff);
    printf("filedes %d\n", filedes);
    int status  = fstat(filedes, &buf);
    printf("Size is %ld\n", buf.st_size);
    close(filedes);
    exit(0);
#endif

    if(my_rank==0)
    {
        const char *message="Hello from 0";
        
        FILE *f;
        char buffer[1024];

        f = fopen64("axxaxx", "w");
        fseeko64(f, 0, SEEK_SET);
        ferror(f);
        ftello64(f);
        setvbuf(f, buffer, _IONBF, 1024);
        fwrite(message, 1, strlen(message), f);
        fflush(f);
        fclose(f);

        fprintf(stdout,"Done\n");
        f = fopen64("axxaxx", "r");
        int n = fread(buffer, 1, 1024, f);
        buffer[n] = 0;
        fclose(f);
        printf("Read '%s'.\n", buffer);

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
