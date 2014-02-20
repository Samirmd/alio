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

#ifdef USE_MPI
#include "client/remote.hpp"

#include "client/config.hpp"
#include "tools/message.hpp"

#include "mpi.h"

#include <errno.h>

namespace ALIO
{

bool     Remote::m_connected = false;
MPI_Comm Remote::m_intercomm;

/** Static init functions called once at startup.  Note that we CAN NOT call 
 *  Remote::connectToServer here: This would call MPI_Init, which in turns 
 *  starts the mpi daemon orted. This then preloads ALIO again, which calls 
 *  Remote::init, which calls MPI_Init, and another daemon is started ... 
 */
int Remote::init()
{
    m_connected = false;
    return 0;
}   // init

// ----------------------------------------------------------------------------
int Remote::atExit()
{
    if(!m_connected)
        return 0;

    int flag;
    MPI_Initialized(&flag);
    if(flag)
    {
        MPI_Finalize();
        printf("Disconnected.\n");
    }
    else
        printf("Not connected.\n");
    return 0;
};   // atExit

// ------------------------------------------------------------------------
/** This is a static function that is called the first time an object
 *  of type Remote is created.
 */
int Remote::connectToServer()
{
    assert(!m_connected);
 
    // No access to argc/argv here - so just make some fields up
    int argc=1;
    char **argv;
    argv = new char*[2];
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

    m_connected = true;
    return 0;
    
}   // connectToServer

// ------------------------------------------------------------------------
/** The constructor of the Remote file object. The first time a Remote
 *  file object is created it will connect to the MPI server.
 *  \param info XML data for the remote file.
 */
Remote::Remote(const XMLNode *info) : BaseFileObject(info)
{
    if(!m_connected)
        connectToServer();
};   // Remote

// ----------------------------------------------------------------------------
 Remote::~Remote()
{
}   // ~Remote

// ----------------------------------------------------------------------------
FILE* Remote:: fopen(const char *mode)
{ 
    Message_fopen m(getFilename(), mode);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return (FILE*)this;
}   // fopen
// ----------------------------------------------------------------------------
FILE* Remote:: fopen64(const char *mode)
{
    Message_fopen64 m(getFilename(), mode);

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

#define FSEEK(NAME, TYPE, MESSAGE_TYPE)                               \
int Remote::NAME(TYPE offset, int whence)                             \
{                                                                     \
    MESSAGE_TYPE m(offset, whence);                                   \
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);   \
    MPI_Status status;                                                \
                                                                      \
    int result;                                                       \
                                                                      \
    int recv_len = m.getSize(result)+m.getSize(errno)+1;              \
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

FSEEK(fseek,    long,    Message_fseek_long   );
FSEEK(fseeko,   off_t,   Message_fseek_off_t  );
FSEEK(fseeko64, off64_t, Message_fseek_off64_t);


// ----------------------------------------------------------------------------
#define TELL(NAME, TYPE, MESSAGE)                                    \
TYPE Remote::NAME()                                                  \
{                                                                    \
    MESSAGE m;                                                       \
    m.allocate(0);                                                   \
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);  \
                                                                     \
    MPI_Status status;                                               \
    TYPE result=0;                                                   \
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

TELL(ftell,    long,    Message_ftell   )
TELL(ftello,   off_t,   Message_ftello  )
TELL(ftello64, off64_t, Message_ftello64)

// ----------------------------------------------------------------------------
int Remote::fflush()
{
    // Ignored for now.
    return 0;
}   // fflush
// ----------------------------------------------------------------------------
int Remote::ferror()
{
    Message_ferror m;
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
int Remote::fileno()
{
    printf("Fileno still to be implemented.\n");
    assert(false);
}   // fileno
// ----------------------------------------------------------------------------
size_t Remote::fwrite(const void *ptr,size_t size, size_t nmemb)
{
    Message_fwrite m(size, nmemb, ptr, size*nmemb);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return nmemb;
}   // fwrite

// ----------------------------------------------------------------------------
size_t Remote::fread(void *ptr,size_t size, size_t nmemb)
{
    assert("Not yet implemented"==NULL);
    Message_fread  m(size, nmemb);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    //    Message m_recv;
    return nmemb;
}   // fread

// ----------------------------------------------------------------------------
int Remote::fclose()
{
    Message_fclose msg_close;
    MPI_Send(msg_close.getData(), msg_close.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    
    Message_quit m;
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return 0;
}   // fclose

// ----------------------------------------------------------------------------
int Remote::feof()
{
    Message_feof msg_eof;
    MPI_Send(msg_eof.getData(), msg_eof.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    assert(false);
    return false;
}   // feof
// ----------------------------------------------------------------------------
char *Remote::fgets(char *s, int size)
{
    Message_fgets msg_fgets(size);

    MPI_Send(msg_fgets.getData(), msg_fgets.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    assert(false);
    return NULL;
}   // fgets

// ----------------------------------------------------------------------------
int Remote::open(int flags, mode_t mode)
{
    Message_open m(getFilename(), flags, mode);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return getIndex()+Config::get()->getMaxFiles();
}   // open
// ----------------------------------------------------------------------------
int Remote::open64(int flags, mode_t mode)
{
    Message_open64 m(getFilename(), flags, mode);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return getIndex()+Config::get()->getMaxFiles();
}   // open

// ----------------------------------------------------------------------------
#define XSTAT(NAME, TYPE, TAG)                                       \
int Remote::NAME(int ver, TYPE *buf)                                 \
{                                                                    \
    Message_stat m;                                                  \
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);  \
                                                                     \
    MPI_Status status;                                               \
    char *msg = new char[sizeof(TYPE) + 2*sizeof(int) + 1];          \
    MPI_Recv(msg, sizeof(TYPE)+2*sizeof(int), MPI_CHAR, 0, 9,        \
             m_intercomm, &status);                                  \
    int *p    = (int *)msg;                                          \
    memcpy(buf, p+2, sizeof(TYPE));                                  \
    int result = p[0];                                               \
    if(result)                                                       \
        errno = p[1];                                                \
    delete msg;                                                      \
    return result;                                                   \
}                                                                    \

    //FIXME: stat takes a filename as parameter - 
XSTAT(__xstat,    struct stat,   MSG___XSTAT );
XSTAT(__fxstat,   struct stat,   MSG___FXSTAT);
XSTAT(__fxstat64, struct stat64, MSG___FXSTAT64);

// ----------------------------------------------------------------------------
int Remote::__lxstat(int ver, struct stat *buf)
{
    Message_stat m;
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
#define LSEEK(NAME, TYPE, TAG, MESSAGE_TYPE)                         \
TYPE Remote::NAME(TYPE offset, int whence)                           \
{                                                                    \
    MESSAGE_TYPE m(offset, whence);                                  \
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);  \
    MPI_Status status;                                               \
    TYPE result;                                                     \
                                                                     \
    int recv_len = m.getSize(result)+m.getSize(errno);               \
    char *msg = new char[recv_len];                                  \
    off_t *p = (TYPE *)msg;                                          \
    MPI_Recv(msg, recv_len, MPI_CHAR, 0, 9, m_intercomm, &status);   \
                                                                     \
    result = p[0];                                                   \
    if(result==(off_t)-1)                                            \
        errno = (int)(p[1]);                                         \
    delete msg;                                                      \
    return result;                                                   \
}   // SEEK

LSEEK(lseek,   off_t,   MSG_LSEEK  , Message_lseek_off_t);
LSEEK(lseek64, off64_t, MSG_LSEEK64, Message_lseek_off64_t);

// ----------------------------------------------------------------------------
ssize_t Remote::write(const void *buf, size_t nbyte)
{
    Message_write m(nbyte, buf, nbyte);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return nbyte;
}   // write

// ----------------------------------------------------------------------------
ssize_t Remote::read(void *buf, size_t count)
{
    Message_read m(count);
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
    Message_close m;
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
    Message_rename m(getFilename(), newpath);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return 0;
}   // rename


}   // namespace ALIO

// USE_MPI
#endif
