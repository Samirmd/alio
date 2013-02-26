#include <stdlib.h>


#include "mpi.h"

#include "tools/os.hpp"
#include "tools/message.hpp"

#include <errno.h>

bool handleRequests(MPI_Comm intercomm);

FILE *m_file = NULL;
int  m_filedes = 0;

int main(int argc, char **argv)
{
    AIO::OS::init();

    MPI_Init(&argc, &argv);

    char port_name[MPI_MAX_PORT_NAME];
    bzero(port_name, MPI_MAX_PORT_NAME);
    MPI_Open_port(MPI_INFO_NULL, port_name); 
    printf("Server: open port '%s'.\n", port_name);

    FILE *port_file = AIO::OS::fopen("aio_config.dat", "w");
    AIO::OS::fwrite(port_name, 1, strlen(port_name), port_file);
    AIO::OS::fclose(port_file);

    MPI_Comm intercomm;
    printf("Server: accepting.\n");
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
            m.get(name);
            m.get(mode);
            name = "/tmp/aio_"+name;
            m_file = fopen(name.c_str(), mode.c_str());
            return false;
        }
    case Message::MSG_FWRITE:
        {
            size_t size, nmemb;
            m.get(size);
            m.get(nmemb);

            char * buffer = m.get();
            fwrite(buffer, size, nmemb, m_file);
            return false;
        }
    case Message::MSG_FCLOSE:
        {
            fclose(m_file);
            return false;
        }
    case Message::MSG_OPEN:
        {
            std::string name;
            m.get(name);
            int flags;
            m.get(flags);
            mode_t mode;
            m.get(mode);
            name = "/tmp/aio_"+name;
            m_filedes = open(name.c_str(), flags, mode);
            return false;
        }
    case Message::MSG_FSTAT:
        {
            int send_len = sizeof(struct stat) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = fstat(m_filedes, (struct stat*)(p+2) );
            p[1]         = errno;
            MPI_Send(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, intercomm);
            delete msg;
            return false;
        }
        
    case Message::MSG_LSEEK:
        {
            off_t offset;
            m.get(offset);
            int whence;
            m.get(whence);
            int send_len = m.getSize(offset)+m.getSize(errno);
            char *msg    = new char[send_len];
            off_t *p     = (off_t*)msg;
            p[0]         = lseek(m_filedes, offset, whence);
            p[1]         = errno;
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);
            delete msg;
            return false;
        }   // switch

    case Message::MSG_WRITE:
        {
            size_t nbyte;
            m.get(nbyte);

            char * buffer = m.get();
            write(m_filedes, buffer, nbyte);
            return false;
        }
    case Message::MSG_READ:
        {
            size_t count;
            m.get(count);
            ssize_t result;
            int send_len = m.getSize(result) + m.getSize(errno) + count;
            char *msg = new char[send_len];
            ssize_t *p = (ssize_t*)msg;
            p[0] = read(m_filedes, p+2, count);
            p[1] = errno;
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);
            delete msg;
            return false;
        }
    case Message::MSG_CLOSE:
        {
            int msg[2];
            msg[0] = close(m_filedes);
            msg[1] = errno;
            MPI_Send(msg, 2, MPI_INT, 0, 9, intercomm);
            return false;
        }
    case Message::MSG_QUIT:
        return true;
    }   // switch
    return m.getType()==Message::MSG_QUIT;
}   // handleRequests
