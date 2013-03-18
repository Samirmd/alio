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

#include "client/remote.hpp"
#include "tools/message.hpp"


#include "mpi.h"

#include <errno.h>

namespace ALIO
{

bool     Remote::m_was_init = false;
MPI_Comm Remote::m_intercomm;
// ------------------------------------------------------------------------
int Remote::init()
{
    if(m_was_init) return 0;
    m_was_init = true;
 
    // No access to argc/argv here - so just make some fields up
    int argc=1;
    char **argv;
    argv = (char**)malloc(2*sizeof(char*));
    argv[0]="name";
    argv[1]=NULL;
    MPI_Init(&argc, &argv);

    FILE *port_file = ALIO::OS::fopen("alio_config.dat", "r");
    char port_name[MPI_MAX_PORT_NAME];
    bzero(port_name, MPI_MAX_PORT_NAME);
    ALIO::OS::fread(port_name, 1, MPI_MAX_PORT_NAME, port_file);
    ALIO::OS::fclose(port_file);

    printf("Got portname '%s'.\n", port_name);

    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &m_intercomm); 
    printf("Connected.\n");

    return 0;
    
}   // init

// ------------------------------------------------------------------------
Remote::Remote(const XMLNode *info) : BaseFileObject(info)
{
};   // Remote

// ----------------------------------------------------------------------------
 Remote::~Remote()
{
};   // ~Remote

// ----------------------------------------------------------------------------
FILE* Remote:: fopen(const char *mode)
{
    Message m(Message::MSG_FOPEN);
    int mode_len = strlen(mode);
    m.allocate(m.getSize(getFilename()) + m.getSize(mode));
    m.add(getFilename());
    m.add(mode);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return (FILE*)this;
}   // fopen
// ----------------------------------------------------------------------------
FILE* Remote:: fopen64(const char *mode)
{
    Message m(Message::MSG_FOPEN64);
    int mode_len = strlen(mode);
    m.allocate(m.getSize(getFilename()) + m.getSize(mode));
    m.add(getFilename());
    m.add(mode);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return (FILE*)this;
}   // fopen



// ----------------------------------------------------------------------------
int Remote::setvbuf(char *buf, int mode, size_t size)
{
    // for now ignore this, it has no real impact on functionality
    return 0;
}   // setvbuf

// ----------------------------------------------------------------------------

#define SEEK(NAME, TYPE, TAG)                                         \
int Remote::NAME(TYPE offset, int whence)                             \
{                                                                     \
    Message m(Message::TAG);                                          \
    m.allocate(m.getSize(offset)+m.getSize(whence));                  \
    m.add(offset);                                                    \
    m.add(whence);                                                    \
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);   \
    MPI_Status status;                                                \
                                                                      \
    int result;                                                       \
                                                                      \
    int recv_len = m.getSize(result)+m.getSize(errno);                \
    char *msg    = new char[recv_len];                                \
    int *p       = (int *)msg;                                        \
    MPI_Recv(msg, recv_len, MPI_CHAR, 0, 9, m_intercomm, &status);    \
                                                                      \
    result = p[0];                                                    \
    if(result==-1)                                                    \
        errno = (int)(p[1]);                                          \
    delete msg;                                                       \
    return result;                                                    \
}   

SEEK(fseek,    long,    MSG_FSEEK   )
SEEK(fseeko,   off_t,   MSG_FSEEKO  )
SEEK(fseeko64, off64_t, MSG_FSEEKO64)
// ----------------------------------------------------------------------------
#define TELL(NAME, TYPE, TAG)                                        \
TYPE Remote::NAME()                                                  \
{                                                                    \
    Message m(Message::TAG);                                         \
    m.allocate(0);                                                   \
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);  \
                                                                     \
    MPI_Status status;                                               \
    TYPE result;                                                     \
                                                                     \
    int recv_len = m.getSize(result)+m.getSize(errno);               \
    char *msg = new char[recv_len];                                  \
    TYPE *p = (TYPE*)msg;                                            \
    MPI_Recv(msg, recv_len, MPI_CHAR, 0, 9, m_intercomm, &status);   \
                                                                     \
    result = p[0];                                                   \
    if(result==-1)                                                   \
        errno = (int)(p[1]);                                         \
    delete msg;                                                      \
    return result;                                                   \
}   

TELL(ftell,    long,    MSG_FTELL   )
TELL(ftello,   off_t,   MSG_FTELLO  )
TELL(ftello64, off64_t, MSG_FTELLO64)

// ----------------------------------------------------------------------------
int Remote::fflush()
{
    // Ignored for now.
    return 0;
}   // fflush
// ----------------------------------------------------------------------------
int Remote::ferror()
{
    Message m(Message::MSG_FERROR);
    m.allocate(0);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);

    MPI_Status status;
    int result;

    int recv_len = m.getSize(result)+m.getSize(errno);
    char *msg = new char[recv_len];
    long *p = (off_t*)msg;
    MPI_Recv(msg, recv_len, MPI_CHAR, 0, 9, m_intercomm, &status);
    
    result = p[0];
    if(result==-1)
        errno = (int)(p[1]);
    delete msg;
    return result;
}   // ferror
// ----------------------------------------------------------------------------
size_t Remote::fwrite(const void *ptr,size_t size, size_t nmemb)
{
    Message m(Message::MSG_FWRITE);
    m.allocate(size*nmemb + m.getSize(size)+m.getSize(nmemb));
    m.add(size);
    m.add(nmemb);
    m.add((char*)ptr, size*nmemb);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return nmemb;
}   // f_write

// ----------------------------------------------------------------------------
size_t Remote::fread(void *ptr,size_t size, size_t nmemb)
{
    assert("Not yet implemented"==NULL);
    Message m(Message::MSG_FREAD);
    m.allocate(m.getSize(size)+m.getSize(nmemb));
    m.add(size);
    m.add(nmemb);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    //    Message m_recv;
    return nmemb;
}   // f_write

// ----------------------------------------------------------------------------
int Remote::fclose()
{
    Message msg_close(Message::MSG_FCLOSE);
    msg_close.allocate(0);
    MPI_Send(msg_close.getData(), msg_close.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    
    Message m(Message::MSG_QUIT);
    m.allocate(0);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return 0;
}   // fclose

// ----------------------------------------------------------------------------
int Remote::feof()
{
    Message msg_eof(Message::MSG_FEOF);
    MPI_Send(msg_eof.getData(), msg_eof.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    assert(false);
    return false;
}   // feof
// ----------------------------------------------------------------------------
char *Remote::fgets(char *s, int size)
{
    Message msg_fgets(Message::MSG_FGETS);
    MPI_Send(msg_fgets.getData(), msg_fgets.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    assert(false);
    return NULL;
}   // fgets

// ----------------------------------------------------------------------------
int Remote::open(int flags, mode_t mode)
{
    Message m(Message::MSG_OPEN);
    m.allocate(m.getSize(getFilename()) + m.getSize(flags)+m.getSize(mode));
    m.add(getFilename());
    m.add(flags);
    m.add((int)mode);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return getIndex()+1024;
}   // open
// ----------------------------------------------------------------------------
int Remote::open64(int flags, mode_t mode)
{
    Message m(Message::MSG_OPEN64);
    m.allocate(m.getSize(getFilename()) + m.getSize(flags)+m.getSize(mode));
    m.add(getFilename());
    m.add(flags);
    m.add((int)mode);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return getIndex()+1024;
}   // open

// ----------------------------------------------------------------------------
int Remote::__xstat(int ver, struct stat *buf)
{
    Message m(Message::MSG___XSTAT);
    m.allocate(0);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);

    MPI_Status status;
    char *msg = new char[sizeof(struct stat) + 2*sizeof(int)];
    MPI_Recv(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, 
             m_intercomm, &status);
    int *p    = (int *)msg;
    memcpy(buf, p+2, sizeof(struct stat));
    int result = p[0];
    if(result)
        errno = p[1];
    delete msg;
    return result;
}   // fstat

// ----------------------------------------------------------------------------
int Remote::__fxstat(int ver, struct stat *buf)
{
    Message m(Message::MSG___FXSTAT);
    m.allocate(0);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);

    MPI_Status status;
    char *msg = new char[sizeof(struct stat) + 2*sizeof(int)];
    MPI_Recv(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, 
             m_intercomm, &status);
    int *p    = (int *)msg;
    memcpy(buf, p+2, sizeof(struct stat));
    int result = p[0];
    if(result)
        errno = p[1];
    delete msg;
    return result;
}   // __fxstat

// ----------------------------------------------------------------------------
int Remote::__fxstat64(int ver, struct stat64 *buf)
{
    Message m(Message::MSG___FXSTAT64);
    m.allocate(0);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);

    MPI_Status status;
    char *msg = new char[sizeof(struct stat) + 2*sizeof(int)];
    MPI_Recv(msg, sizeof(struct stat64)+2*sizeof(int), MPI_CHAR, 0, 9, 
             m_intercomm, &status);
    int *p    = (int *)msg;
    memcpy(buf, p+2, sizeof(struct stat64));
    int result = p[0];
    if(result)
        errno = p[1];
    delete msg;
    return result;
}   // __fxstat64

// ----------------------------------------------------------------------------
int Remote::__lxstat(int ver, struct stat *buf)
{
    Message m(Message::MSG___LXSTAT);
    m.allocate(0);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);

    MPI_Status status;
    char *msg = new char[sizeof(struct stat) + 2*sizeof(int)];
    MPI_Recv(msg, sizeof(struct stat)+2*sizeof(int), MPI_CHAR, 0, 9, 
             m_intercomm, &status);
    int *p    = (int *)msg;
    memcpy(buf, p+2, sizeof(struct stat));
    int result = p[0];
    if(result)
        errno = p[1];
    delete msg;
    return result;
}   // fstat

// ----------------------------------------------------------------------------
off_t Remote::lseek(off_t offset, int whence)
{
    Message m(Message::MSG_LSEEK);
    m.allocate(m.getSize(offset)+m.getSize(whence));
    m.add(offset);
    m.add(whence);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    MPI_Status status;
    off_t result;

    int recv_len = m.getSize(result)+m.getSize(errno);
    char *msg = new char[recv_len];
    off_t *p = (off_t*)msg;
    MPI_Recv(msg, recv_len, MPI_CHAR, 0, 9, m_intercomm, &status);
    
    result = p[0];
    if(result==(off_t)-1)
        errno = (int)(p[1]);
    delete msg;
    return result;
}   // lseek
// ----------------------------------------------------------------------------
off64_t Remote::lseek64(off64_t offset, int whence)
{
    Message m(Message::MSG_LSEEK64);
    m.allocate(m.getSize(offset)+m.getSize(whence));
    m.add(offset);
    m.add(whence);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    MPI_Status status;
    off_t result;

    int recv_len = m.getSize(result)+m.getSize(errno);
    char *msg = new char[recv_len];
    off_t *p = (off_t*)msg;
    MPI_Recv(msg, recv_len, MPI_CHAR, 0, 9, m_intercomm, &status);
    
    result = p[0];
    if(result==(off_t)-1)
        errno = (int)(p[1]);
    delete msg;
    return result;
}   // lseek64

// ----------------------------------------------------------------------------
ssize_t Remote::write(const void *buf, size_t nbyte)
{
    Message m(Message::MSG_WRITE);
    m.allocate(nbyte + m.getSize(nbyte));
    m.add(nbyte);
    m.add(buf, nbyte);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return nbyte;
}   // write

// ----------------------------------------------------------------------------
ssize_t Remote::read(void *buf, size_t count)
{
    Message m(Message::MSG_READ);
    m.allocate(m.getSize(count));
    m.add(count);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);

    ssize_t result;
    MPI_Status status;
    int recv_len = m.getSize(errno)+m.getSize(result)+count;
    char *msg    = new char[recv_len];
    ssize_t *p   = (ssize_t*)msg;
    MPI_Recv(msg, recv_len, MPI_CHAR, 0, 9, m_intercomm, &status);
    result = p[0];
    if(result ==-1)
        memcpy(p+1, &errno, sizeof(errno));
    memcpy(buf, msg+m.getSize(errno)+m.getSize(result), result);
    delete msg;
    return result;
}   // read

// ----------------------------------------------------------------------------
int Remote::close()
{
    Message m(Message::MSG_CLOSE);
    m.allocate(0);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    
    int result;
    int msg[2];
    MPI_Status status;
    MPI_Recv(msg, 2, MPI_INT, 0, 9, m_intercomm, &status);
    result = msg[0];
    if(result ==-1)
        errno = msg[1];
    
    return result;
}   // close

// ----------------------------------------------------------------------------
int Remote::rename(const char *newpath)
{
    Message m(Message::MSG_RENAME);
    m.allocate(m.getSize(getFilename()) + m.getSize(newpath));
    m.add(getFilename());
    m.add(newpath);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return 0;
}   // rename


}   // namespace ALIO
