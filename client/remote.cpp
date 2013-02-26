#include "client/remote.hpp"
#include "tools/message.hpp"

#include "mpi.h"

namespace AIO
{

bool     Remote::m_was_init = false;
MPI_Comm Remote::m_intercomm;
// ------------------------------------------------------------------------
int Remote::init()
{
    if(m_was_init) return 0;

    // No access to argc/argv here - so just make some fields up
    int argc=1;
    char **argv;
    argv = (char**)malloc(2*sizeof(char*));
    argv[0]="name";
    argv[1]=NULL;
    MPI_Init(&argc, &argv);

    FILE *port_file = AIO::OS::fopen("aio_config.dat", "r");
    char port_name[MPI_MAX_PORT_NAME];
    bzero(port_name, MPI_MAX_PORT_NAME);
    AIO::OS::fread(port_name, 1, MPI_MAX_PORT_NAME, port_file);
    AIO::OS::fclose(port_file);

    printf("Got portname '%s'.\n", port_name);

    MPI_Comm_connect(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &m_intercomm); 
    printf("Connected.\n");

    return 0;
    
}   // init

// ------------------------------------------------------------------------
Remote::Remote()
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
    int name_len = getFilename().size();
    m.allocate(m.getStringLength(getFilename()) + mode_len+1);
    m.addString(getFilename());
    m.addCharArray(mode, mode_len);

    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return (FILE*)this;
}   // fopen

// ----------------------------------------------------------------------------
size_t Remote::fwrite(const void *ptr,size_t size, size_t nmemb)
{
    Message m(Message::MSG_FWRITE);
    m.allocate(size*nmemb + 2*m.getSizeTLength());
    m.add(size);
    m.add(nmemb);
    m.addCharArray((char*)ptr, size*nmemb);
    MPI_Send(m.getData(), m.getLen(), MPI_CHAR, 0, 1, m_intercomm);
    return nmemb;
}   // f_write

// ----------------------------------------------------------------------------
size_t Remote::fread(void *ptr,size_t size, size_t nmemb)
{
    assert("Not yet implemented"==NULL);
    Message m(Message::MSG_FREAD);
    m.allocate(2*m.getSizeTLength());
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

}   // open

// ----------------------------------------------------------------------------
    int Remote::__fxstat(int ver, struct stat *buf)
{
}   // fstat

// ----------------------------------------------------------------------------
off_t Remote::lseek(off_t offset, int whence)
{
}   // lseek

// ----------------------------------------------------------------------------
ssize_t Remote::write(const void *buf, size_t nbyte)
{
}   // write

// ----------------------------------------------------------------------------
ssize_t Remote::read(void *buf, size_t count)
{
}   // read

// ----------------------------------------------------------------------------
int Remote::close()
{
}   // close

// ----------------------------------------------------------------------------


}   // namespace AIO
