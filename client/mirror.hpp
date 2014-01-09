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

#ifndef HEADER_MIRROR_FILE_OBJECT_DECORATOR_HPP
#define HEADER_MIRROR_FILE_OBJECT_DECORATOR_HPP

#include "client/standard_file_object.hpp"
#include "tools/os.hpp"

#include <errno.h>

namespace ALIO
{

/** This class mirrors all operations on a second file object
 *  (for now a StandardFileObject, with a '.mirror' appended to
 *  the name).
 */
class MirrorFileObjectDecorator : public I_FileObjectDecorator
{
private:
    I_FileObject *m_mirror;

public:
    /** No static init function needed. */
    static int init() { return 0; }
    // ------------------------------------------------------------------------
    /** No static atExit function needed. */
    static int atExit() { return 0; }
    // ------------------------------------------------------------------------
    MirrorFileObjectDecorator(I_FileObject *parent, const XMLNode *info)
        : I_FileObjectDecorator(parent, info)
    {
        m_mirror = new StandardFileObject(info);
    };
    // ------------------------------------------------------------------------
    virtual ~MirrorFileObjectDecorator() 
    {
        delete m_mirror;
    };
    // ------------------------------------------------------------------------
    virtual void setFilename(const std::string &filename)
    {
        I_FileObjectDecorator::setFilename(filename+".mirror");
        m_mirror->setFilename(filename);
    }
    // ------------------------------------------------------------------------
    virtual FILE* fopen(const char *mode)
    {
        m_mirror->fopen(mode);
        return I_FileObjectDecorator::fopen(mode);
    }
    // ------------------------------------------------------------------------
    virtual FILE* fopen64(const char *mode)
    {
        m_mirror->fopen64(mode);
        return I_FileObjectDecorator::fopen64(mode);
    }

    // ------------------------------------------------------------------------
    virtual int setvbuf(char *buf, int mode, size_t size)
    {
        m_mirror->setvbuf(buf, mode, size);
        return I_FileObjectDecorator::setvbuf(buf, mode, size);
    }   // setvbuf
    // ------------------------------------------------------------------------
    virtual int fseek(long offset, int whence)
    {
        m_mirror->fseek(offset, whence);
        return I_FileObjectDecorator::fseek(offset, whence);
    }   // fseek
    // ------------------------------------------------------------------------
    virtual int fseeko(off_t offset, int whence)
    {
        m_mirror->fseeko(offset, whence);
        return I_FileObjectDecorator::fseeko(offset, whence);
    }   // fseeko
    // ------------------------------------------------------------------------
    virtual int fseeko64(off64_t offset, int whence)
    {
        m_mirror->fseeko64(offset, whence);
        return I_FileObjectDecorator::fseeko64(offset, whence);
    }   // fseeko64
    // ------------------------------------------------------------------------
    virtual long ftell()
    {
        m_mirror->ftell();
        return I_FileObjectDecorator::ftell();
    }   // ftell
    // ------------------------------------------------------------------------
    virtual off_t ftello()
    {
        m_mirror->ftello();
        return I_FileObjectDecorator::ftello();
    }   // ftello
    // ------------------------------------------------------------------------
    virtual off64_t ftello64()
    {
        m_mirror->ftello64();
        return I_FileObjectDecorator::ftello64();
    }   // ftello
    // ------------------------------------------------------------------------
    virtual int fflush()
    {
        m_mirror->fflush();
        return I_FileObjectDecorator::fflush();
    }   // fflush
    // ------------------------------------------------------------------------
    virtual int ferror()
    {
        m_mirror->ferror();
        return I_FileObjectDecorator::ferror();
    }   // ferror
    // ------------------------------------------------------------------------
    virtual int fileno()
    {
        m_mirror->fileno();
        return I_FileObjectDecorator::fileno();
    }   // fileno
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        m_mirror->fwrite(ptr, size, nmemb);
        return I_FileObjectDecorator::fwrite(ptr, size, nmemb);
    }
    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb) 
    { 
        char *ptr_mirror = new char[size*nmemb];
        size_t n_mirror = m_mirror->fread(ptr, size, nmemb);
        size_t n        = I_FileObjectDecorator::fread(ptr, size, nmemb);
        if(n!=n_mirror)
        {
            printf("[%s] fread(%lx, %ld, %ld) inconsistent results: %ld vs %ld (mirror)",
                   getFilename().c_str(), ptr, size, nmemb, n, n_mirror);
        }
        for(unsigned int i=0; i<n; i++)
        {
            if(ptr_mirror[i]!=((char*)ptr)[i])
                printf("[%s] read error @ %d : %d (mirrored) %d\n", 
                       getFilename().c_str(), i, ptr_mirror[i], ((char*)ptr)[i]);
        }
        delete ptr_mirror;
        return n;
    }
    // ------------------------------------------------------------------------
    virtual int feof() 
    { 
        m_mirror->feof();
        int n = I_FileObjectDecorator::feof();
        return n;
    }   // feof
    // ------------------------------------------------------------------------
    virtual char * fgets(char *s, int size) 
    { 
        m_mirror->fgets(s, size);
        char *result = I_FileObjectDecorator::fgets(s, size);
        return result;
    }   // fgets
    // ------------------------------------------------------------------------
    virtual int fclose() 
    {
        m_mirror->fclose();
        int error = I_FileObjectDecorator::fclose(); 
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        m_mirror->open(flags, mode);
        int filedes = I_FileObjectDecorator::open(flags, mode);
        return filedes;
    }   // open
    // ------------------------------------------------------------------------
    virtual int open64(int flags, mode_t mode)
    {
        m_mirror->open64(flags, mode);
        int filedes = I_FileObjectDecorator::open64(flags, mode);
        return filedes;
    }   // open64
    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        m_mirror->__xstat(ver, buf);
        int error = I_FileObjectDecorator::__xstat(ver, buf);
        return error;
    }   // _xstat
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        m_mirror->__fxstat(ver, buf);
        int error = I_FileObjectDecorator::__fxstat(ver, buf);
        return error;
    }   // __fxstat
    // ------------------------------------------------------------------------
    virtual int __fxstat64(int ver, struct stat64 *buf)
    {
        m_mirror->__fxstat64(ver, buf);
        int error = I_FileObjectDecorator::__fxstat64(ver, buf);
        return error;
    }   // __fxstat64
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        m_mirror->__lxstat(ver, buf);
        int error = I_FileObjectDecorator::__lxstat(ver, buf);
        return error;
    }   // _lxstat
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        m_mirror->lseek(offset, whence);
        int error = I_FileObjectDecorator::lseek(offset, whence);
        return error;
    }   // lseek
    // ------------------------------------------------------------------------
    virtual off64_t lseek64(off64_t offset, int whence)
    {
        m_mirror->lseek64(offset, whence);
        int error = I_FileObjectDecorator::lseek64(offset, whence);
        return error;
    }   // lseek64
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        m_mirror->write(buf, nbyte);
        ssize_t result = I_FileObjectDecorator::write(buf, nbyte);
        return result;
    }   // write
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {

        char *buf_mirror     = new char[count];
        size_t result_mirror = m_mirror->read(buf_mirror, count);
        ssize_t result       = I_FileObjectDecorator::read(buf, count);

        if(result!=result_mirror)
        {
            printf("[%s] read(%lx, %ld) inconsistent results: %ld vs %ld (mirror)",
                   getFilename().c_str(), buf, count, result, result_mirror);
        }
        for(unsigned int i=0; i<result; i++)
        {
            if(((char*)buf)[i]!=buf_mirror[i])
            {
                printf("[%s] read error @ %d : %d %d\n", 
                       getFilename().c_str(), i, ((char*)buf)[i], 
                       buf_mirror[i]);
            }
        }
        delete buf_mirror;
        return result;
    }   // read
    // ------------------------------------------------------------------------
    virtual int close()
    {
        m_mirror->close();
        int result = I_FileObjectDecorator::close();
        return result;
    }   // close
    // ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        m_mirror->rename(newpath);
        int result = I_FileObjectDecorator::rename(newpath);
        return result;
    }
    // ------------------------------------------------------------------------

};   // IFileObject

}   // namespace ALIO
#endif
