#ifndef HEADER_STANDARD_FILE_OBJECT_HPP
#define HEADER_STANDARD_FILE_OBJECT_HPP

#include "client/base_file_object.hpp"
#include "tools/os.hpp"

#include <string>

namespace AIO
{
class StandardFileObject : public BaseFileObject
{
protected:
    /** The original stream pointer in case of stream objects. */
    FILE *m_file;

    /** The original file descriptor. */
    int  m_filedes;

public:

    StandardFileObject()
    {
        m_file     = NULL; 
    };   // StandardFileObject

    // ------------------------------------------------------------------------
    virtual ~StandardFileObject()
    {
    };   // ~StandardFileObject

    // ------------------------------------------------------------------------
    virtual FILE*  fopen(const char *mode)
    {
        m_file=OS::fopen(getFilename().c_str(), mode);
        if(!m_file)
            return NULL;
        return (FILE*)this;
    }   // fopen

    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        return OS::fwrite(ptr, size, nmemb, m_file);
    }   // f_write

    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb)
    {
        return OS::fread(ptr, size, nmemb, m_file);
    }   // f_read

    // ------------------------------------------------------------------------
    virtual int feof()
    {
        return OS::feof(m_file);
    }   // feof
    // ------------------------------------------------------------------------
    virtual char *fgets(char *s, int size)
    {
        return OS::fgets(s, size, m_file);
    }   // fgets
    // ------------------------------------------------------------------------
    virtual int fclose()
    {
        int error = OS::fclose(m_file);
        if(!error)
            m_file = NULL;
        return error;
    }   // fclose

    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        m_filedes = OS::open(getFilename().c_str(), flags, mode);
        return getIndex()+1024;
    }   // open
    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf)
    {
        return OS::__xstat(ver, getFilename().c_str(), buf);
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf)
    {
        return OS::__fxstat(ver, m_filedes, buf);
    }   // fstat
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf)
    {
        return OS::__lxstat(ver, getFilename().c_str(), buf);
    }   // fstat
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        return OS::lseek(m_filedes, offset, whence);
    }   // lseek
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        return OS::write(m_filedes, buf, nbyte);
    }   // write
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        return OS::read(m_filedes, buf, count);
    }   // read
    // ------------------------------------------------------------------------
    virtual int close()
    {
        return OS::close(m_filedes);
    }   // close
    // ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        return OS::rename(getFilename().c_str(), newpath);
    }
    // ------------------------------------------------------------------------


};   // StandardFileObject

};   // namespace AIO
#endif
