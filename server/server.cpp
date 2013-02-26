#include "mpi.h"

#include "tools/os.hpp"
#include "tools/message.hpp"

bool handleRequests(MPI_Comm intercomm);

FILE *m_file = NULL;

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

    MPI_Comm intercomm;
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm); 
    printf("Accepted.\n");
    
    while(1)
    {
        if(handleRequests(intercomm))
            break;
    }
    

}   // main

// ----------------------------------------------------------------------------
bool handleRequests(MPI_Comm intercomm)
{
    MPI_Status status;

    MPI_Probe(0, 1, intercomm, &status);

    int len;
    MPI_Get_count(&status, MPI_CHAR, &len);
    char *buffer = new char[len];

    MPI_Recv(buffer, len, MPI_CHAR, 0, 1, intercomm, &status);
    Message m(buffer, len);

    switch(m.getType())
    {
    case Message::MSG_FOPEN:
        {
            std::string name, mode;
            name = m.getString();
            mode = m.getString();
            name = "/tmp/aio_"+name;
            m_file = fopen(name.c_str(), mode.c_str());
            return false;
        }
    case Message::MSG_FWRITE:
        {
            size_t size, nmemb;
            size = m.getInt();
            nmemb = m.getInt();
            char * buffer = m.getCurrentBuffer();
            fwrite(buffer, size, nmemb, m_file);
            return false;
        }
    case Message::MSG_FCLOSE:
        {
            fclose(m_file);
            return false;
        }
    case Message::MSG_QUIT:
        return true;
    }   // switch
    return m.getType()==Message::MSG_QUIT;
}   // handleRequests
