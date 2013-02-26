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
    static int init();

    Remote();
    virtual ~Remote();
    virtual FILE*  fopen(const char *mode);
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb);
    virtual size_t fread(void *ptr,size_t size, size_t nmemb);
    virtual int feof();
    virtual char * fgets(char *s, int size);
    virtual int fclose();
    virtual off_t lseek(int fildes, off_t offset, int whence);

};   // Remote

};   // namespace AIO
#endif
