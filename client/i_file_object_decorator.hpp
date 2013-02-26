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
    virtual void setData(const std::string &filename, int index)
    {
        m_parent->setData(filename, index);
    }
    // ------------------------------------------------------------------------
    virtual const std::string &getFilename() const
    {
        return m_parent->getFilename();
    }
    // ------------------------------------------------------------------------
    virtual const int getIndex() const
    {
        return m_parent->getIndex();
    }

    // ------------------------------------------------------------------------
    virtual FILE*  fopen(const char *mode) { return m_parent->fopen(mode); }
    // ------------------------------------------------------------------------
    virtual size_t fwrite(const void *ptr,size_t size, size_t nmemb)
    {
        return m_parent->fwrite(ptr, size, nmemb);
    }
    // ------------------------------------------------------------------------
    virtual int    fclose() { return m_parent->fclose(); }
    // ------------------------------------------------------------------------
    virtual off_t  lseek(int fildes, off_t offset, int whence)
    {
        return m_parent->lseek(fildes, offset, whence); 
    }
};   // IFileObject

}   // namespace AIO
#endif
