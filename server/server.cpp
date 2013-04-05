//
//    ALIO - ALternative IO library
//    Copyright (C) 2013  Joerg Henrichs
//
//    ALIO is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    ALIO is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with ALIO.  If not, see <http://www.gnu.org/licenses/>.
//

#include <stdlib.h>


#include "mpi.h"

#include "tools/os.hpp"
#include "tools/message.hpp"

#include <errno.h>

bool handleRequests(MPI_Comm intercomm);

FILE *m_file = NULL;int  m_filedes = 0;

std::string m_filename("");

int main(int argc, char **argv)
{
    ALIO::OS::init();

    MPI_Init(&argc, &argv);

    char port_name[MPI_MAX_PORT_NAME];
    bzero(port_name, MPI_MAX_PORT_NAME);
    MPI_Open_port(MPI_INFO_NULL, port_name); 
    printf("Server: open port '%s'.\n", port_name);

    FILE *port_file = ALIO::OS::fopen("alio_config.dat", "w");
    ALIO::OS::fwrite(port_name, 1, strlen(port_name), port_file);
    ALIO::OS::fclose(port_file);

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
    printf("probe.\n");fflush(stdout);
    MPI_Probe(0, 1, intercomm, &status);

    int len;
    MPI_Get_count(&status, MPI_CHAR, &len);
    char *buffer = new char[len];
    
    MPI_Recv(buffer, len, MPI_CHAR, 0, 1, intercomm, &status);
    printf("Len=%d type %d\n", len, buffer[0]);
    //TODO can be removed
    

    switch((Message::MessageType)buffer[0])
    {
    case Message::MSG_FOPEN:
    case Message::MSG_FOPEN64:
        {
            std::string mode;
            Message_fopen m(buffer, len, &m_filename, &mode);
            if(m.getType()==Message::MSG_FOPEN)
                m_file = fopen(m_filename.c_str(), mode.c_str());
            else
                m_file = fopen64(m_filename.c_str(), mode.c_str());
            break;
        }
    case Message::MSG_FSEEK:
        {
#define FSEEK(NAME, TYPE, MESSAGE_TYPE)                          \
            TYPE offset;                                         \
            int whence;                                          \
            MESSAGE_TYPE m(buffer, len, &offset, &whence);       \
            int send_len = m.getSize(whence)+m.getSize(errno);   \
            int result = NAME(m_file, offset, whence);           \
            Message_fseek_answer m_answer(Message::MSG_FSEEK,    \
                                          result, errno);        \
            MPI_Send(m_answer.getData(), m_answer.getLen(),      \
                     MPI_CHAR, 0, 9, intercomm);


            FSEEK(fseek, long, Message_fseek_long);
            break;
        }   // switch

    case Message::MSG_FSEEKO:
        {
            FSEEK(fseeko, off_t, Message_fseek_off_t);
            break;
        }   // switch

    case Message::MSG_FSEEKO64:
        {
            FSEEK(fseeko64, off64_t, Message_fseek_off64_t);
            break;
        }   // switch

    case Message::MSG_FTELL:
        {
#define FTELL(NAME, TYPE)                                      \
            Message_ftell m(buffer, len);                      \
            TYPE result;                                       \
            int send_len = m.getSize(result)+m.getSize(errno); \
            char *msg    = new char[send_len];                 \
            TYPE *p      = (TYPE*)msg;                         \
            p[0]         = NAME(m_file);                       \
            memcpy(p+1, &errno, sizeof(errno));                \
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);\
            delete msg;

            FTELL(ftell, long);
            break;
        }   // switch

    case Message::MSG_FTELLO:
        {
            FTELL(ftello, off_t);
            break;
        }   // switch

    case Message::MSG_FTELLO64:
        {
            FTELL(ftello64, off64_t);
            break;
        }   // switch

    case Message::MSG_FERROR:
        {
            Message_ferror m(buffer, len);
            int n;
            int send_len = m.getSize(n)+m.getSize(errno);
            char *msg    = new char[send_len];
            int *p       = (int*)msg;
            p[0]         = ferror(m_file);
            p[1]         = errno;
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);
            delete msg;
            break;
        }   // switch

    case Message::MSG_FWRITE:
        {
            size_t size, nmemb;
            Message_fwrite m(buffer, len, &size, &nmemb);

            char * buffer = m.get();
            fwrite(buffer, size, nmemb, m_file);
            break;
        }
    case Message::MSG_FREAD:
        {
            size_t size, nmemb;
            Message_fread m(buffer, len, &size, &nmemb);
            assert(false);  // not yet implemented
            break;
        }
    case Message::MSG_FCLOSE:
        {
            Message_fclose m(buffer, len);
            fclose(m_file);
            break;
        }
    case Message::MSG_FEOF:
        {
            Message_feof m(buffer, len);
            assert(false);  // not yet implemented
            break;
        }
    case Message::MSG_FGETS:
        {
            int size;
            Message_fgets m(buffer, len, &size);
            assert(false);  // not yet implemented
            break;
        }
    case Message::MSG_OPEN:
    case Message::MSG_OPEN64:
        {
            int flags;
            mode_t mode;
            Message_open m(buffer, len, &m_filename, &flags, &mode);
            if(m.getType()==Message::MSG_OPEN)
                m_filedes = open(m_filename.c_str(), flags, mode);
            else
                m_filedes = open64(m_filename.c_str(), flags, mode);
            break;
        }
    case Message::MSG___XSTAT:
        {
#ifdef XX
#define XSTAT(TYPE, FUNC)                          \
            Message_stat m(buffer, len);      \
            int send_len = sizeof(TYPE) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = FUNC(m_filename.c_str(), (struct stat*)(p+2) );
            p[1]         = errno;
            MPI_Send(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, intercomm);
            delete msg;
            return false;
            XSTAT(struct stat, );
#endif
            Message m(buffer, len);
            int send_len = sizeof(struct stat) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = stat(m_filename.c_str(), (struct stat*)(p+2) );
            p[1]         = errno;
            MPI_Send(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, intercomm);
            delete msg;
            break;
        }
        
    case Message::MSG___FXSTAT:
        {
            Message m(buffer, len);
            int send_len = sizeof(struct stat) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = fstat(m_filedes, (struct stat*)(p+2) );
            p[1]         = errno;
            MPI_Send(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, intercomm);
            delete msg;
            break;
        }

    case Message::MSG___FXSTAT64:
        {
            Message m(buffer, len);
            int send_len = sizeof(struct stat64) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = fstat64(m_filedes, (struct stat64*)(p+2) );
            p[1]         = errno;
            MPI_Send(msg, sizeof(struct stat64)+2*sizeof(int), MPI_CHAR, 0, 9, intercomm);
            delete msg;
            break;
        }
        
    case Message::MSG___LXSTAT:
        {
            Message_stat m(buffer, len);
            int send_len = sizeof(struct stat) + 2*sizeof(int);
            char *msg    = new char[send_len];
            int *p       = (int *)msg;
            p[0]         = lstat(m_filename.c_str(), (struct stat*)(p+2) );
            p[1]         = errno;
            MPI_Send(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, intercomm);
            delete msg;
            break;
        }
        
    case Message::MSG_LSEEK:
        {
            Message m(buffer, len);
            off_t offset;
            m.get(&offset);
            int whence;
            m.get(&whence);
            int send_len = m.getSize(offset)+m.getSize(errno);
            char *msg    = new char[send_len];
            off_t *p     = (off_t*)msg;
            p[0]         = lseek(m_filedes, offset, whence);
            p[1]         = errno;
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);
            delete msg;
            break;
        }   // switch

    case Message::MSG_LSEEK64:
        {
            Message m(buffer, len);
            off64_t offset;
            m.get(&offset);
            int whence;
            m.get(&whence);
            int send_len = m.getSize(offset)+m.getSize(errno);
            char *msg    = new char[send_len];
            off_t *p     = (off_t*)msg;
            p[0]         = lseek(m_filedes, offset, whence);
            p[1]         = errno;
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);
            delete msg;
            break;
        }   // switch

    case Message::MSG_WRITE:
        {
            size_t nbyte;
            Message_write m(buffer, len, &nbyte);

            char * buffer = m.get();
            write(m_filedes, buffer, nbyte);
            break;
        }
    case Message::MSG_READ:
        {
            Message m(buffer, len);
            size_t count;
            m.get(&count);
            ssize_t result;
            int send_len = m.getSize(result) + m.getSize(errno) + count;
            char *msg = new char[send_len];
            ssize_t *p = (ssize_t*)msg;
            p[0] = read(m_filedes, msg+ m.getSize(result) + m.getSize(errno), count);
            // We can't assign to p[1], since then 8 bytes would be written
            memcpy(p+1, &errno, sizeof(errno));
            MPI_Send(msg, send_len, MPI_CHAR, 0, 9, intercomm);
            delete msg;
            break;
        }
    case Message::MSG_CLOSE:
        {
            Message m(buffer, len);
            int msg[2];
            msg[0] = close(m_filedes);
            msg[1] = errno;
            MPI_Send(msg, 2, MPI_INT, 0, 9, intercomm);
            break;
        }
    case Message::MSG_QUIT:
        {
            Message_quit m(buffer, len);
            return true;
        }
    }   // switch
    return false;
}   // handleRequests
