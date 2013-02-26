#ifndef HEADER_I_FILE_OBJECT_HPP
#define HEADER_I_FILE_OBJECT_HPP

#include <stdio.h>
#include <string>
#include <sys/types.h>

#ifndef WIN32
#  include <unistd.h>
#endif


namespace AIO
{

class I_FileObject
{
public:
    I_FileObject() 
    {
    };
    // ------------------------------------------------------------------------
    virtual ~I_FileObject() {};
    // ------------------------------------------------------------------------
    virtual void setData(const std::string &filename, int index) = 0;
    // ------------------------------------------------------------------------
    virtual const std::string &getFilename() const = 0;
    // ------------------------------------------------------------------------
    virtual int getIndex() const = 0;
    // ------------------------------------------------------------------------

    virtual FILE*  fopen(const char *mode) = 0;
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb) = 0;
    virtual int    fclose() = 0;
    virtual off_t  lseek(int fildes, off_t offset, int whence) = 0;
};   // IFileObject

}   // namespace AIO
#endif
