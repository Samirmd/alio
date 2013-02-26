#ifndef HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP
#define HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP

#include "client/i_file_object_decorator.hpp"
#include "tools/os.hpp"


namespace AIO
{

class DebugFileObjectDecorator : public I_FileObjectDecorator
{
private:
    /** Counter so that operations on various files can be grouped. */
    static int m_count;
public:
    DebugFileObjectDecorator(I_FileObject *parent)
        : I_FileObjectDecorator(parent)
    {
    };
    // ------------------------------------------------------------------------
    virtual ~DebugFileObjectDecorator() {};
    // ------------------------------------------------------------------------
    virtual FILE* fopen(const char *mode)
    {
        printf("[%s] fopen.\n", I_FileObjectDecorator::getFilename().c_str());
        FILE *file = I_FileObjectDecorator::fopen(mode);
        if(!file)
            printf("[%s] fopen error, error code %d.\n", 
                   I_FileObjectDecorator::getFilename().c_str(), errno);
        else
            printf("[%s] fopen ok.\n", 
                  I_FileObjectDecorator::getFilename().c_str());
        return (FILE*)this;
    }
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        printf("[%s] fwrite(%lx, %ld %d.\n", 
               I_FileObjectDecorator::getFilename().c_str(), ptr, size, nmemb);
        size_t n = I_FileObjectDecorator::fwrite(ptr, size, nmemb);
        printf("[%s] fwrite(%lx, %ld %d)=%d.\n", 
               I_FileObjectDecorator::getFilename().c_str(), ptr, size, nmemb,
               n);
        return n;
    }
    // ------------------------------------------------------------------------
    virtual int    fclose() { return I_FileObjectDecorator::fclose(); }
    // ------------------------------------------------------------------------
    virtual off_t  lseek(int fildes, off_t offset, int whence)
    {
        return I_FileObjectDecorator::lseek(fildes, offset, whence); 
    }
};   // IFileObject

}   // namespace AIO
#endif
