#ifndef HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP
#define HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP

#include "client/i_file_object_decorator.hpp"
#include "tools/os.hpp"

#include <errno.h>

namespace AIO
{

class DebugFileObjectDecorator : public I_FileObjectDecorator
{
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
        printf("[%s] fopen(\"%s\")", I_FileObjectDecorator::getFilename().c_str(), 
               mode);
        FILE *file = I_FileObjectDecorator::fopen(mode);
        if(!file)
            printf(" = errno(%d).\n", errno);
        else
            printf(" = %lx\n", file);
        return (FILE*)this;
    }
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        printf("[%s] fwrite(%lx, %ld, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), ptr, size, nmemb);
        fflush(stdout);
        size_t n = I_FileObjectDecorator::fwrite(ptr, size, nmemb);
        printf(" = %ld.\n", n);
        return n;
    }
    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) 
    { 
        printf("[%s] fread(%lx, %ld, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), ptr, size, nmemb);
        fflush(stdout);
        size_t n = I_FileObjectDecorator::fread(ptr, size, nmemb);
        printf(" = %ld\n", n);
        return n;
    }
    // ------------------------------------------------------------------------
    virtual int feof() 
    { 
        printf("[%s] feof()", 
               I_FileObjectDecorator::getFilename().c_str());
        fflush(stdout);
        int n = I_FileObjectDecorator::feof();
        printf(" = %d\n", n);
        return n;
    }   // feof
    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size) 
    { 
        printf("[%s] fgets(%lx, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), s, size);
        fflush(stdout);
        char *result = I_FileObjectDecorator::fgets(s, size);
        printf(" = %lx\n", result);
        return result;
    }   // fgets
    // ------------------------------------------------------------------------
    virtual int fclose() 
    {
        printf("[%s] fclose()", 
               I_FileObjectDecorator::getFilename().c_str());
        fflush(stdout);
        int error = I_FileObjectDecorator::fclose(); 
        printf(" = %d\n", error);
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        printf("[%s] open(%d, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), flags, mode);
        fflush(stdout);
        int filedes = I_FileObjectDecorator::open(flags, mode);
        printf(" = %d\n", filedes);
        return filedes;
    }   // open
    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        printf("[%s] stat(%lx)", 
               I_FileObjectDecorator::getFilename().c_str(), buf);
        fflush(stdout);
        int error = I_FileObjectDecorator::__xstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        printf("[%s] fstat(%lx)", 
               I_FileObjectDecorator::getFilename().c_str(), buf);
        fflush(stdout);
        int error = I_FileObjectDecorator::__fxstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        printf("[%s] lstat(%lx)", 
               I_FileObjectDecorator::getFilename().c_str(), buf);
        fflush(stdout);
        int error = I_FileObjectDecorator::__lxstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        printf("[%s] lseek(%ld, %d)", 
               I_FileObjectDecorator::getFilename().c_str(), offset, whence);
        fflush(stdout);
        int error = I_FileObjectDecorator::lseek(offset, whence);
        printf(" = %d\n", error);
        return error;
    }   // lseek
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        printf("[%s] write(%lx, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), buf, nbyte);
        fflush(stdout);
        ssize_t result = I_FileObjectDecorator::write(buf, nbyte);
        printf(" = %ld\n", result);
        return result;
    }   // write
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        printf("[%s] read(%lx, %ld)", 
               I_FileObjectDecorator::getFilename().c_str(), buf, count);
        fflush(stdout);
        ssize_t result = I_FileObjectDecorator::read(buf, count);
        printf(" = %ld\n", result);
        return result;
    }   // read
    // ------------------------------------------------------------------------
    virtual int close()
    {
        printf("[%s] close()", 
               I_FileObjectDecorator::getFilename().c_str());
        fflush(stdout);
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

}   // namespace AIO
#endif
