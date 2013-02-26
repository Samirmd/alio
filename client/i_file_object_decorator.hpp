#ifndef HEADER_I_FILE_OBJECT_DECORATOR_HPP
#define HEADER_I_FILE_OBJECT_DECORATOR_HPP

#include "client/i_file_object.hpp"

#include <stdio.h>
#include <string>
#include <sys/types.h>

#ifndef WIN32
#  include <unistd.h>
#endif


namespace AIO
{

class I_FileObjectDecorator : public I_FileObject
{
private:
    I_FileObject *m_parent;
public:
    I_FileObjectDecorator(I_FileObject *parent) 
    {
        m_parent = parent;

    };
    virtual ~I_FileObjectDecorator() {};
    virtual void setFilename(const std::string &filename)
    {
        m_parent->setFilename(filename);
    }
    // ------------------------------------------------------------------------
    virtual const std::string &getFilename() const
    {
        return m_parent->getFilename();
    }
    // ------------------------------------------------------------------------
    /** Sets the index of this object in config's m_file_object array. */
    void setIndex(int index) { m_parent->setIndex(index); }
    // ------------------------------------------------------------------------
    /** Returns the index of this object in config's m_file_object array. */
    int getIndex() const { return m_parent->getIndex(); }
    // ------------------------------------------------------------------------
    virtual FILE*  fopen(const char *mode) { return m_parent->fopen(mode); }
    // ------------------------------------------------------------------------
    virtual FILE*  fopen64(const char *mode) { return m_parent->fopen64(mode);}
    // ------------------------------------------------------------------------
    virtual int setvbuf(char *buf, int mode, size_t size)
    {
        return m_parent->setvbuf(buf, mode, size);
    }
    // ------------------------------------------------------------------------
    virtual int fseek(long offset, int whence)
    {
        return m_parent->fseek(offset, whence);
    }
    // ------------------------------------------------------------------------
    virtual int fseeko(off_t offset, int whence)
    {
        return m_parent->fseeko(offset, whence);
    }
    // ------------------------------------------------------------------------
    virtual int fseeko64(off64_t offset, int whence)
    {
        return m_parent->fseeko64(offset, whence);
    }
    // ------------------------------------------------------------------------
    virtual long ftell() { return m_parent->ftell(); }
    // ------------------------------------------------------------------------
    virtual off_t ftello() { return m_parent->ftello(); }
    // ------------------------------------------------------------------------
    virtual off64_t ftello64() { return m_parent->ftello64(); }
    // ------------------------------------------------------------------------
    virtual int fflush() { return m_parent->fflush(); }
    // ------------------------------------------------------------------------
    virtual int ferror() { return m_parent->ferror(); }
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        return m_parent->fwrite(ptr, size, nmemb);
    }
    // ------------------------------------------------------------------------
    virtual size_t fread(void *ptr,size_t size, size_t nmemb)
    {
        return m_parent->fread(ptr, size, nmemb);
    }
    // ------------------------------------------------------------------------
    virtual int feof() { return m_parent->feof(); }
    // ------------------------------------------------------------------------
    virtual char *fgets(char *s, int size) { return m_parent->fgets(s, size); }
    // ------------------------------------------------------------------------
    virtual int fclose() { return m_parent->fclose(); }
    // ------------------------------------------------------------------------
    virtual int open(int flags, mode_t mode)
    {
        return m_parent->open(flags, mode); 
    }
    // ------------------------------------------------------------------------
    virtual int open64(int flags, mode_t mode)
    {
        return m_parent->open64(flags, mode); 
    }
    // ------------------------------------------------------------------------
    virtual int __xstat(int ver, struct stat *buf) 
    {
        return m_parent->__xstat(ver, buf); 
    } 
    // ------------------------------------------------------------------------
    virtual int __fxstat(int ver, struct stat *buf) 
    {
        return m_parent->__fxstat(ver, buf); 
    } 
    // ------------------------------------------------------------------------
    virtual int __lxstat(int ver, struct stat *buf) 
    {
        return m_parent->__lxstat(ver, buf); 
    } 
    // ------------------------------------------------------------------------
    virtual off_t lseek(off_t offset, int whence)
    {
        return m_parent->lseek(offset, whence); 
    }
    // ------------------------------------------------------------------------
    virtual ssize_t write(const void *buf, size_t nbyte)
    {
        return m_parent->write(buf, nbyte);
    }
    // ------------------------------------------------------------------------
    virtual ssize_t read(void *buf, size_t count)
    {
        return m_parent->read(buf, count);
    }
    // ------------------------------------------------------------------------
    virtual int close() { return m_parent->close(); }
    // ------------------------------------------------------------------------
    virtual int rename(const char *newpath)
    {
        return m_parent->rename(newpath);
    }

};   // IFileObject

}   // namespace AIO
#endif
