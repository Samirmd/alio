#include "mpi.h"

#include "tools/os.hpp"
#include "tools/comm_header.hpp"

void handleRequests(MPI_Comm intercomm);


int main(int argc, char **argv)
{
    AIO::OS::init();

    MPI_Init(&argc, &argv);

    char port_name[MPI_MAX_PORT_NAME];
    bzero(port_name, MPI_MAX_PORT_NAME);
    MPI_Open_port(MPI_INFO_NULL, port_name); 
    
    FILE *port_file = AIO::OS::fopen("aio_config.dat", "w");
    AIO::OS::fwrite(port_name, 1, strlen(port_name), port_file);
    AIO::OS::fclose(port_file);

    printf("Accepting.\n");
    MPI_Comm intercomm;
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm); 
    printf("Accepted.\n");
    
    handleRequests(intercomm);
    

}   // main

// ----------------------------------------------------------------------------
void handleRequests(MPI_Comm intercomm)
{
    
}   // handleRequests
