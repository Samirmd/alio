#ifndef HEADER_REMOTE_HPP
#define HEADER_REMOTE_HPP

#include "client/base_file_object.hpp"
#include "tools/os.hpp"

#include "mpi.h"

#include <string>

namespace AIO
{
class Remote : public BaseFileObject
{
    static bool m_was_init;
    static MPI_Comm m_intercomm;

public:
    static int      init();

                    Remote();
    virtual        ~Remote();
    virtual FILE   *fopen(const char *mode);
    virtual size_t  fwrite(const void *ptr,size_t size, size_t nmemb);
    virtual size_t  fread(void *ptr,size_t size, size_t nmemb);
    virtual int     feof();
    virtual char   *fgets(char *s, int size);
    virtual int     fclose();

    virtual int     open(int flags, mode_t mode);
    virtual int     __xstat(int ver, struct stat *buf);
    virtual int     __fxstat(int ver, struct stat *buf);
    virtual int     __lxstat(int ver, struct stat *buf);
    virtual off_t   lseek(off_t offset, int whence);
    virtual ssize_t write(const void *buf, size_t nbyte);
    virtual ssize_t read(void *buf, size_t count);
    virtual int     close();
    virtual int     rename(const char *newpath);

};   // Remote

};   // namespace AIO
#endif
