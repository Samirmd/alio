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

#ifndef HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP
#define HEADER_DEBUG_FILE_OBJECT_DECORATOR_HPP

#include "client/i_file_object_decorator.hpp"
#include "tools/os.hpp"

#include <errno.h>
#include <sys/time.h>

namespace ALIO
{
class XMLNode;

class DebugFileObjectDecorator : public I_FileObjectDecorator
{
private:
    static double m_start_time;

public:
    DebugFileObjectDecorator(I_FileObject *parent, const XMLNode *info)
        : I_FileObjectDecorator(parent, info)
    {
    };

    // ------------------------------------------------------------------------
    virtual ~DebugFileObjectDecorator() {};

    // ------------------------------------------------------------------------
    /** Returns the time in seconds since epoch. */
    static double getSecondsSinceStart()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec*1.0e-6 - m_start_time;
    }   // getSecondsSinceStart

    // ------------------------------------------------------------------------
    /** Static initialisation function, which sets the start time so that
     *  all other times can be printed in seconds since start. 
     */
    static void init()
    {
        m_start_time = 0.0;  // now getSecodsSinceStart will return current time
        m_start_time = getSecondsSinceStart();
    }   // init


    // ------------------------------------------------------------------------
    /** Prints the filename and the time stamp.
     */
    void header()
    {
        printf("%s @ %lf ", I_FileObjectDecorator::getFilename().c_str(), 
               getSecondsSinceStart());
    }   // header

    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    // Now come all wrappers. Consult the man pages to find out the details
    // about those functions. The wrapper here print the header, function
    // name and parameters, before calling the original function. When the
    // original function returns, this function prints the exit core/error
    // number.
    // ------------------------------------------------------------------------
    // ------------------------------------------------------------------------
    virtual FILE* fopen(const char *mode)
    {
        header(); printf("fopen(\"%s\")", mode);
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
        header(); printf("fopen64(\"%s\")", mode);
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
        header(); printf(" setvbuf(%lx, %d, %ld)", buf, mode, size);
        int result = I_FileObjectDecorator::setvbuf(buf, mode, size);
        printf(" = %d\n", result);
        return result;
    }   // setvbuf

    // ------------------------------------------------------------------------
    virtual int fseek(long offset, int whence)
    {
        header(); printf("fseek(%ld, %d)", offset, whence);
        int result = I_FileObjectDecorator::fseek(offset, whence);
        printf(" = %d\n", result);
        return result;
    }   // fseek

    // ------------------------------------------------------------------------
    virtual int fseeko(off_t offset, int whence)
    {
        header(); printf("fseeko(%ld, %d)", offset, whence);
        int result = I_FileObjectDecorator::fseeko(offset, whence);
        printf(" = %d\n", result);
        return result;
    }   // fseeko

    // ------------------------------------------------------------------------
    virtual int fseeko64(off64_t offset, int whence)
    { 
        header(); printf("fseeko64(%ld, %d)", offset, whence);
        int result = I_FileObjectDecorator::fseeko64(offset, whence);
        printf(" = %d\n", result);
        return result;
    }   // fseeko64

    // ------------------------------------------------------------------------
    virtual long ftell()
    {
        header(); printf("ftell()");
        long result = I_FileObjectDecorator::ftell();
        printf(" = %ld\n", result);
        return result;
    }   // ftell

    // ------------------------------------------------------------------------
    virtual off_t ftello()
    {
        header(); printf("ftello()");
        long result = I_FileObjectDecorator::ftello();
        printf(" = %ld\n", result);
        return result;
    }   // ftell

    // ------------------------------------------------------------------------
    virtual off64_t ftello64()
    {
        header(); printf("ftello64()");
        long result = I_FileObjectDecorator::ftello64();
        printf(" = %ld\n", result);
        return result;
    }   // ftello64

    // ------------------------------------------------------------------------
    virtual int fflush()
    {
        header(); printf("fflush()");
        int result = I_FileObjectDecorator::fflush();
        printf(" = %d\n", result);
        return result;
    }   // fflush

    // ------------------------------------------------------------------------
    virtual int ferror()
    {
        header(); printf("ferror()");
        int result = I_FileObjectDecorator::ferror();
        printf(" = %d\n", result);
        return result;
    }   // ferror

    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        header(); printf("fwrite(%lx, %ld, %ld)", ptr, size, nmemb);
        std::fflush(stdout);
        size_t n = I_FileObjectDecorator::fwrite(ptr, size, nmemb);
        printf(" = %ld.\n", n);
        return n;
    }   // fwrite

    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) 
    { 
        header(); printf("fread(%lx, %ld, %ld)", ptr, size, nmemb);
        std::fflush(stdout);
        size_t n = I_FileObjectDecorator::fread(ptr, size, nmemb);
        printf(" = %ld\n", n);
        return n;
    }   // fread

    // ------------------------------------------------------------------------
    virtual int feof() 
    { 
        header(); printf("feof()");
        std::fflush(stdout);
        int n = I_FileObjectDecorator::feof();
        printf(" = %d\n", n);
        return n;
    }   // feof

    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size) 
    { 
        header(); printf("fgets(%lx, %d)", s, size);
        std::fflush(stdout);
        char *result = I_FileObjectDecorator::fgets(s, size);
        printf(" = %lx\n", result);
        return result;
    }   // fgets

    // ------------------------------------------------------------------------
    virtual int fclose() 
    {
        header(); printf("fclose()");
        std::fflush(stdout);
        int error = I_FileObjectDecorator::fclose(); 
        printf(" = %d\n", error);
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        header(); printf("open(%d, %d)", flags, mode);
        std::fflush(stdout);
        int filedes = I_FileObjectDecorator::open(flags, mode);
        printf(" = %d\n", filedes);
        return filedes;
    }   // open

    // ------------------------------------------------------------------------
    virtual int open64(int flags, mode_t mode)
    {
        header(); printf("open64(%d, %d)", flags, mode);
        std::fflush(stdout);
        int filedes = I_FileObjectDecorator::open64(flags, mode);
        printf(" = %d\n", filedes);
        return filedes;
    }   // open64

    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        header(); printf("stat(%lx)", buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__xstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // __xstat

    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        header(); printf("fstat(%lx)", buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__fxstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // __fxstat

    // ------------------------------------------------------------------------
    virtual int __fxstat64(int ver, struct stat64 *buf)
    {
        header(); printf("fstat64(%lx)", buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__fxstat64(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // __fxstat64

    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        header(); printf("lstat(%lx)", buf);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::__lxstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // __lxstat

    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        header(); printf("lseek(%ld, %d)", offset, whence);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::lseek(offset, whence);
        printf(" = %d\n", error);
        return error;
    }   // lseek

    // ------------------------------------------------------------------------
    virtual off64_t lseek64(off64_t offset, int whence)
    {
        header(); printf("lseek64(%ld, %d)", offset, whence);
        std::fflush(stdout);
        int error = I_FileObjectDecorator::lseek64(offset, whence);
        printf(" = %d\n", error);
        return error;
    }   // lseek64

    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        header(); printf("write(%lx, %ld)", buf, nbyte);
        std::fflush(stdout);
        ssize_t result = I_FileObjectDecorator::write(buf, nbyte);
        printf(" = %ld\n", result);
        return result;
    }   // write

    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        header(); printf("read(%lx, %ld)", buf, count);
        std::fflush(stdout);
        ssize_t result = I_FileObjectDecorator::read(buf, count);
        printf(" = %ld\n", result);
        return result;
    }   // read

    // ------------------------------------------------------------------------
    virtual int close()
    {
        header(); printf("close()");
        std::fflush(stdout);
        int result = I_FileObjectDecorator::close();
        printf(" = %d\n", result);
        return result;
    }   // close

    // ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        header(); printf("rename(%s)", newpath);
        int result = I_FileObjectDecorator::rename(newpath);
        printf(" = %d\n", result);
        return result;
    }

    // ------------------------------------------------------------------------

};   // IFileObject

}   // namespace ALIO
#endif
