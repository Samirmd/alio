#ifndef HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP
#define HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP

#include "client/i_file_object_decorator.hpp"
#include "tools/os.hpp"

#include <errno.h>

namespace ALIO
{
class XMLNode;

class DebugFileObjectDecorator : public I_FileObjectDecorator
{
public:
    DebugFileObjectDecorator(I_FileObject *parent, const XMLNode *info)
        : I_FileObjectDecorator(parent, info)
    {
    };

    // ------------------------------------------------------------------------
    virtual ~DebugFileObjectDecorator() {};

    // ------------------------------------------------------------------------
    virtual FILE* fopen(const char *mode)
    {
        printf("[%s] fopen(\"%s\")", I_FileObjectDecorator::getFilename().c_str(), 
               mode);
        FILE *file = I_FileObjectDecorator::fopen(mode);
        if(!file)
            printf(" = errno(%d).\n", errno);
        else
            printf(" = %lx\n", file);
        return (FILE*)this;
    }   // fopen

    // ------------------------------------------------------------------------
    virtual FILE* fopen64(const char *mode)
    {
        printf("[%s] fopen64(\"%s\")", I_FileObjectDecorator::getFilename().c_str(), 
               mode);
        FILE *file = I_FileObjectDecorator::fopen64(mode);
        if(!file)
            printf(" = errno(%d).\n", errno);
        else
            printf(" = %lx\n", file);
        return (FILE*)this;
    }   // fopen64

    // ------------------------------------------------------------------------
    virtual int setvbuf(char *buf, int mode, size_t size)
    {
        printf("[%s] setvbuf(%lx, %d, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), 
               buf, mode, size);
        int result = I_FileObjectDecorator::setvbuf(buf, mode, size);
        printf(" = %d\n", result);
        return result;
    }   // setvbuf

    // ------------------------------------------------------------------------
    virtual int fseek(long offset, int whence)
    {
        printf("[%s] fseek(%ld, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), offset, whence);
        int result = I_FileObjectDecorator::fseek(offset, whence);
        printf(" = %d\n", result);
        return result;
    }   // fseek

    // ------------------------------------------------------------------------
    virtual int fseeko(off_t offset, int whence)
    {
        printf("[%s] fseeko(%ld, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), offset, whence);
        int result = I_FileObjectDecorator::fseeko(offset, whence);
        printf(" = %d\n", result);
        return result;
    }   // fseeko

    // ------------------------------------------------------------------------
    virtual int fseeko64(off64_t offset, int whence)
    {
        printf("[%s] fseeko64(%ld, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), offset, whence);
        int result = I_FileObjectDecorator::fseeko64(offset, whence);
        printf(" = %d\n", result);
        return result;
    }   // fseeko64

    // ------------------------------------------------------------------------
    virtual long ftell()
    {
        printf("[%s] ftell()", I_FileObjectDecorator::getFilename().c_str());
        long result = I_FileObjectDecorator::ftell();
        printf(" = %ld\n", result);
        return result;
    }   // ftell

    // ------------------------------------------------------------------------
    virtual off_t ftello()
    {
        printf("[%s] ftello()", I_FileObjectDecorator::getFilename().c_str());
        long result = I_FileObjectDecorator::ftello();
        printf(" = %ld\n", result);
        return result;
    }   // ftell

    // ------------------------------------------------------------------------
    virtual off64_t ftello64()
    {
        printf("[%s] ftello64()", I_FileObjectDecorator::getFilename().c_str());
        long result = I_FileObjectDecorator::ftello64();
        printf(" = %ld\n", result);
        return result;
    }   // ftello64

    // ------------------------------------------------------------------------
    virtual int fflush()
    {
        printf("[%s] fflush()", I_FileObjectDecorator::getFilename().c_str());
        int result = I_FileObjectDecorator::fflush();
        printf(" = %d\n", result);
        return result;
    }   // fflush

    // ------------------------------------------------------------------------
    virtual int ferror()
    {
        printf("[%s] ferror()", I_FileObjectDecorator::getFilename().c_str());
        int result = I_FileObjectDecorator::ferror();
        printf(" = %d\n", result);
        return result;
    }   // ferror

    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        printf("[%s] fwrite(%lx, %ld, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), ptr, size, nmemb);
        std::fflush(stdout);
        size_t n = I_FileObjectDecorator::fwrite(ptr, size, nmemb);
        printf(" = %ld.\n", n);
        return n;
    }   // fwrite

    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) 
    { 
        printf("[%s] fread(%lx, %ld, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), ptr, size, nmemb);
        std::fflush(stdout);
        size_t n = I_FileObjectDecorator::fread(ptr, size, nmemb);
        printf(" = %ld\n", n);
        return n;
    }   // fread

    // ------------------------------------------------------------------------
    virtual int feof() 
    { 
        printf("[%s] feof()", 
               I_FileObjectDecorator::getFilename().c_str());
        std::fflush(stdout);
        int n = I_FileObjectDecorator::feof();
        printf(" = %d\n", n);
        return n;
    }   // feof

    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size) 
    { 
        printf("[%s] fgets(%lx, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), s, size);
        std::fflush(stdout);
        char *result = I_FileObjectDecorator::fgets(s, size);
        printf(" = %lx\n", result);
        return result;
    }   // fgets

    // ------------------------------------------------------------------------
    virtual int fclose() 
    {
        printf("[%s] fclose()", 
               I_FileObjectDecorator::getFilename().c_str());
        std::fflush(stdout);
        int error = I_FileObjectDecorator::fclose(); 
        printf(" = %d\n", error);
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        printf("[%s] open(%d, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), flags, mode);
        std::fflush(stdout);
        int filedes = I_FileObjectDecorator::open(flags, mode);
        printf(" = %d\n", filedes);
        return filedes;
    }   // open

    // ------------------------------------------------------------------------
    virtual int open64(int flags, mode_t mode)
    {
        printf("[%s] open64(%d, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), flags, mode);
        std::fflush(stdout);
        int filedes = I_FileObjectDecorator::open64(flags, mode);
        printf(" = %d\n", filedes);
        return filedes;
    }   // open64

    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        printf("[%s] stat(%lx)", 
               I_FileObjectDecorator::getFilename().c_str(), buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__xstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // __xstat

    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        printf("[%s] fstat(%lx)", 
               I_FileObjectDecorator::getFilename().c_str(), buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__fxstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // __fxstat

    // ------------------------------------------------------------------------
    virtual int __fxstat64(int ver, struct stat64 *buf)
    {
        printf("[%s] fstat64(%lx)", 
               I_FileObjectDecorator::getFilename().c_str(), buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__fxstat64(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // __fxstat64

    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        printf("[%s] lstat(%lx)", 
               I_FileObjectDecorator::getFilename().c_str(), buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__lxstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // __lxstat

    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        printf("[%s] lseek(%ld, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), offset, whence);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::lseek(offset, whence);
        printf(" = %d\n", error);
        return error;
    }   // lseek

    // ------------------------------------------------------------------------
    virtual off64_t lseek64(off64_t offset, int whence)
    {
        printf("[%s] lseek64(%ld, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), offset, whence);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::lseek64(offset, whence);
        printf(" = %d\n", error);
        return error;
    }   // lseek64

    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        printf("[%s] write(%lx, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), buf, nbyte);
        std::fflush(stdout);
        ssize_t result = I_FileObjectDecorator::write(buf, nbyte);
        printf(" = %ld\n", result);
        return result;
    }   // write

    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        printf("[%s] read(%lx, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), buf, count);
        std::fflush(stdout);
        ssize_t result = I_FileObjectDecorator::read(buf, count);
        printf(" = %ld\n", result);
        return result;
    }   // read

    // ------------------------------------------------------------------------
    virtual int close()
    {
        printf("[%s] close()", 
               I_FileObjectDecorator::getFilename().c_str());
        std::fflush(stdout);
        int result = I_FileObjectDecorator::close();
        printf(" = %d\n", result);
        return result;
    }   // close

    // ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        printf("[%s] rename(%s)", 
               I_FileObjectDecorator::getFilename().c_str(), newpath);
        int result = I_FileObjectDecorator::rename(newpath);
        printf(" = %d\n", result);
        return result;
    }

    // ------------------------------------------------------------------------

};   // IFileObject

}   // namespace ALIO
#endif
