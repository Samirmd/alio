#include <stdlib.h>


#include "mpi.h"

#include "tools/os.hpp"
#include "tools/message.hpp"

#include <errno.h>

bool handleRequests(MPI_Comm intercomm);

FILE *m_file = NULL;
int  m_filedes = 0;

std::string m_filename("");

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
    printf("Server received %d\n", m.getType());
    fflush(stdout);
    switch(m.getType())
    {
    case Message::MSG_FOPEN:
    case Message::MSG_FOPEN64:
        {
            std::string mode;
            m.get(m_filename);
            m.get(mode);
            if(m.getType()==Message::MSG_FOPEN)
                m_file = fopen(m_filename.c_str(), mode.c_str());
            else
                m_file = fopen64(m_filename.c_str(), mode.c_str());
            return false;
        }
    case Message::MSG_FSEEK:
        {
#define SEEK(NAME, TYPE)                                         \
            TYPE offset;                                         \
            m.get(offset);                                       \
            int whence;                                          \
            m.get(whence);                                       \
            int send_len = m.getSize(whence)+m.getSize(errno);   \
            char *msg    = new char[send_len];                   \
            int *p       = (int*)msg;                            \
            p[0]         = NAME(m_file, offset, whence);         \
            p[1]         = errno;                                \
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);  \
            delete msg;                                          \
            return false;

            SEEK(fseek, long);
        }   // switch

    case Message::MSG_FSEEKO:
        {
            SEEK(fseeko, off_t);
            return false;
        }   // switch

    case Message::MSG_FSEEKO64:
        {
            SEEK(fseeko64, off64_t);
            return false;
        }   // switch

    case Message::MSG_FTELL:
        {
#define TELL(NAME, TYPE)                                       \
            TYPE result;                                       \
            int send_len = m.getSize(result)+m.getSize(errno); \
            char *msg    = new char[send_len];                 \
            TYPE *p      = (TYPE*)msg;                         \
            p[0]         = NAME(m_file);                       \
            memcpy(p+1, &errno, sizeof(errno));                \
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);\
            delete msg;                                        \
            return false;
            TELL(ftell, long);
        }   // switch

    case Message::MSG_FTELLO:
        {
            TELL(ftello, off_t);
        }   // switch

    case Message::MSG_FTELLO64:
        {
            TELL(ftello64, off64_t);
        }   // switch

    case Message::MSG_FERROR:
        {
            int n;
            int send_len = m.getSize(n)+m.getSize(errno);
            char *msg    = new char[send_len];
            int *p       = (int*)msg;
            p[0]         = ferror(m_file);
            p[1]         = errno;
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);
            delete msg;
            return false;
        }   // switch

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
    case Message::MSG_OPEN64:
        {
            m.get(m_filename);
            int flags;
            m.get(flags);
            mode_t mode;
            m.get(mode);
            if(m.getType()==Message::MSG_OPEN)
                m_filedes = open(m_filename.c_str(), flags, mode);
            else
                m_filedes = open64(m_filename.c_str(), flags, mode);
            return false;
        }
    case Message::MSG___XSTAT:
        {
            int send_len = sizeof(struct stat) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = stat(m_filename.c_str(), (struct stat*)(p+2) );
            p[1]         = errno;
            MPI_Send(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, intercomm);
            delete msg;
            return false;
        }
        
    case Message::MSG___FXSTAT:
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

    case Message::MSG___FXSTAT64:
        {
            int send_len = sizeof(struct stat64) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = fstat64(m_filedes, (struct stat64*)(p+2) );
            p[1]         = errno;
            MPI_Send(msg, sizeof(struct stat64)+2*sizeof(int), MPI_CHAR, 0, 9, intercomm);
            delete msg;
            return false;
        }
        
    case Message::MSG___LXSTAT:
        {
            int send_len = sizeof(struct stat) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = lstat(m_filename.c_str(), (struct stat*)(p+2) );
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

    case Message::MSG_LSEEK64:
        {
            off64_t offset;
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
            p[0] = read(m_filedes, msg+ m.getSize(result) + m.getSize(errno), count);
            // We can't assign to p[1], since then 8 bytes would be written
            memcpy(p+1, &errno, sizeof(errno));
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
            return true;
        }
    case Message::MSG_QUIT:
        return true;
    }   // switch
    return m.getType()==Message::MSG_QUIT;
}   // handleRequests
