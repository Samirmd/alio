#ifndef HEADER_MIRROR_FILE_OBJECT_DECORATOR_HPP
#define HEADER_MIRROR_FILE_OBJECT_DECORATOR_HPP

#include "client/standard_file_object.hpp"
#include "tools/os.hpp"

#include <errno.h>

namespace AIO
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
    MirrorFileObjectDecorator(I_FileObject *parent)
        : I_FileObjectDecorator(parent)
    {
        m_mirror = new StandardFileObject();
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
    virtual int __xstat(int ver, struct stat *buf)
    {
        m_mirror->__xstat(ver, buf);
        int error = I_FileObjectDecorator::__xstat(ver, buf);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        m_mirror->__fxstat(ver, buf);
        int error = I_FileObjectDecorator::__fxstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        m_mirror->__lxstat(ver, buf);
        int error = I_FileObjectDecorator::__lxstat(ver, buf);
        printf(" = %d\n", error);
        return error;
    }   // fstat
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        m_mirror->lseek(offset, whence);
        int error = I_FileObjectDecorator::lseek(offset, whence);
        return error;
    }   // lseek
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

}   // namespace AIO
#endif
