#ifndef HEADER_I_FILE_OBJECT_HPP
#define HEADER_I_FILE_OBJECT_HPP

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


namespace AIO
{

class I_FileObject
{
public:
             I_FileObject(const char *filename) {};
    virtual ~I_FileObject() {};
    virtual FILE*  fopen(const char *mode) = 0;
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb) = 0;
    virtual int    fclose() = 0;
    virtual off_t  lseek(int fildes, off_t offset, int whence) = 0;
};   // IFileObject

}   // namespace AIO
#endif
